#pragma once
#include "particle.hpp"
#include "help.hpp"
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/System/Vector3.hpp>
#include <cmath>
#include <cstdint>

#define DEFAULT_GRAVITY 500.0f

class Solver{
    private:
        std::vector<Particle> particles;
        sf::Vector2f boundary_pos;
        sf::Vector2f gravity;
        float boundary_radius;


        sf::CircleShape getCircleFromParticle(const Particle& particle) const {
            sf::CircleShape first_circle = sf::CircleShape(particle.getRadius(), 300);
            float r = particle.getRadius();
            first_circle.setPosition(particle.getPosition());
            first_circle.setOrigin(sf::Vector2f{r, r});
            /*first_circle.setOrigin(particle.getPosition() - sf::Vector2f{r, r});*/
            first_circle.setFillColor(particle.getColor());
            first_circle.setScale({1.0f, 1.0f});
            return first_circle;
        }
        
        void drawParticles(sf::RenderWindow& window) const{
            for(const Particle& particle: particles){
                window.draw(getCircleFromParticle(particle));
            }
        }
        void drawBoundary(sf::RenderWindow& window) const{
            sf::CircleShape first_circle = sf::CircleShape(this->boundary_radius, 1000);
            float r = this->boundary_radius;
            first_circle.setPosition(this->boundary_pos);
            first_circle.setOrigin(sf::Vector2f{r, r});
            first_circle.setFillColor(sf::Color::White);
            first_circle.setScale({1.0f, 1.0f});
            window.draw(first_circle);
        }
        void applyBoundary(Particle& particle, float dt) {
                float px = particle.getPosition().x;
                float py = particle.getPosition().y;

                sf::Vector3f bound = this->getBoundary();
                float bx = bound.x;
                float by = bound.y;

                float distance = std::sqrt((bx - px)*(bx - px) + (by - py)*(by - py));
                if( distance > bound.z - particle.getRadius() ){
                    const sf::Vector2f n = sf::Vector2f(bx - px, by - py) / distance;
                    const sf::Vector2f perp = {-n.y, n.x};
                    const sf::Vector2f vel = particle.getVelocity();
                    particle.setPosition(sf::Vector2f(bx, by)- n * (bound.z - particle.getRadius()));
                    particle.setVelocity(2.0f * (vel.x * perp.x + vel.y * perp.y) * perp - vel, 0.8f);
                }
        }
        void applyGravity(Particle& particle){
            particle.accelerate(this->gravity);
        }
    public:
        void setLeftGravity() {
            this->gravity = {-DEFAULT_GRAVITY, 0.f};
        }
        void setRightGravity() {
            this->gravity = {DEFAULT_GRAVITY, 0.f};
        }
        void setTopGravity() {
            this->gravity = {0.f, -DEFAULT_GRAVITY};
        }
        void setBottomGravity(){
            this->gravity = {0.f, DEFAULT_GRAVITY};
        }

        Solver (): 
            particles{},  
            boundary_pos{0.0f, 0.0f},
            boundary_radius(0),
            gravity({0.0f, DEFAULT_GRAVITY})
        {}
        std::vector<Particle>& getObjects() { return particles; }

        void new_ball(sf::Vector2f mouse_pos, sf::Color color){ 
            addObject(Particle(
                        mouse_pos,
                        mouse_pos,
                sf::Vector2f(0,0), 
                sf::Vector2f(0, 0), 
                10.0f, 
                color)
            );
        }

        void addObject(const Particle& particle){
            particles.push_back(particle);
        }
        void update(float dt){
            collideParticles(dt);
            for(Particle& particle: particles){
                for(int i = 0; i < SUB_STEP_NUM; i++){
                    applyGravity(particle);
                    applyBoundary(particle, dt);
                    particle.updateParticle(dt);
                }
            }
        }
        sf::Vector3f getBoundary() const {
            return {boundary_pos.x, boundary_pos.y, boundary_radius};
        }
        void setBoundary(sf::Vector2f pos, float radius){
            boundary_pos = pos;
            boundary_radius = radius;
        }
        void pullObject(const sf::Vector2f& pos, Particle& particle){
            sf::Vector2f dir = pos - particle.getPosition();
            float length = sqrt(dir.x * dir.x + dir.y * dir.y);
            particle.accelerate(dir * std::max(0.0f, 10 * (120 - length)));
        }
        void pushObject(const sf::Vector2f& pos, Particle& particle){
            sf::Vector2f dir = pos - particle.getPosition();
            float length = sqrt(dir.x * dir.x + dir.y * dir.y);
            particle.accelerate(dir * std::min(0.0f, -10 * (120 - length)));
        }
        void pullObjects(sf::Vector2f pos) {
           for(Particle& particle : getObjects()){
               pullObject(pos, particle);
           } 
        }
        void pushObjects(sf::Vector2f pos) {
           for(Particle& particle : getObjects()){
               pushObject(pos, particle);
           } 
        }
        int objectsNum() const {
            return ( &( this->particles ) )->size();
        }
        void collideParticles(float dt){
            for(int i = 0; i < objectsNum(); i++){
                for(int j = 0; j < objectsNum(); j++){
                    if(i == j) continue; //same object, don't count it
                    Particle& p1 = this->getObjects()[i];
                    Particle& p2 = this->getObjects()[j];

                    float distance_cent = eucl_distance(p1.getPosition(), p2.getPosition());  
                    float min_distance = p1.getRadius() + p2.getRadius();
                    if(distance_cent < min_distance){
                        sf::Vector2f n = p1.getPosition() - p2.getPosition();
                        float length_n = std::sqrt(n.x * n.x + n.y * n.y);

                        if(0 == length_n) continue;

                        n = n * (1 / length_n);

                        float total_mass = p1.getRadius() + p2.getRadius();
                        float mass_ration = ( p1.getRadius() ) / total_mass;

                        float delta = 0.1f * (min_distance - distance_cent);
                        
                        p1.setPosition(p1.getPosition() + ( n * (1 - mass_ration) * delta ));
                        p2.setPosition(p2.getPosition() - ( n * (mass_ration) * delta ));
                    }
                }
            }
        }
        void render(sf::RenderWindow& window) const{
            this->drawBoundary(window);
            this->drawParticles(window);
        }
};
