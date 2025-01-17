#pragma once
#include "particle.hpp"
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/System/Vector3.hpp>
#include <cmath>

class Solver{
    private:
        std::vector<Particle> particles;
        sf::Vector2f boundary_pos;
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
        void applyBoundary(Particle& particle) {
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
                    particle.setVelocity(2.0f * (vel.x * perp.x + vel.y * perp.y) * perp - vel, 1.0f);
                }
        }
        void applyGravity(Particle& particle){
            particle.accelerate({0.0f, GRAVITY});
        }
    public:
        Solver (): 
            particles{},  
            boundary_pos{0.0f, 0.0f},
            boundary_radius(0)
        {}
        std::vector<Particle>& getObjects() { return particles; }

        void addObject(const Particle& particle){
            particles.push_back(particle);
        }
        void update(float dt){
            for(Particle& particle: particles){
                for(int i = 0; i < SUB_STEP_NUM; i++){
                    applyGravity(particle);
                    applyBoundary(particle);
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
        void render(sf::RenderWindow& window) const{
            this->drawBoundary(window);
            this->drawParticles(window);
        }
};
