#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Color.hpp>
#include <SFML/System/Vector2.hpp>
#include <cstdint>
#include <vector>
#include <iostream>

#define GRAVITY 300.0f
#define SUB_STEP_NUM 8

class Particle{
private:
    sf::Vector2f position; 
    sf::Vector2f prev_position; 
    sf::Vector2f acceleration; 
    sf::Vector2f velocity; 
    float radius = 20.0f;
    sf::Color color = sf::Color::Cyan;

public:
    Particle() {}
    Particle(sf::Vector2f _pos, sf::Vector2f _prev_pos, sf::Vector2f _acc, sf::Vector2f _vel, float _radius, sf::Color _color) 
        : 
            position{_pos},
            prev_position{_prev_pos},
            acceleration{_acc},
            velocity{_vel},
            radius{_radius},
            color{_color}
    {}
    sf::Vector2f getPosition() const { return position; }
    sf::Vector2f getPrevPosition() const { return prev_position; }
    sf::Vector2f getAcceleration() const { return acceleration; }
   /*sf::Vector2f getVelocity() const { return velocity; }*/
    float getRadius() const { return radius; }
    sf::Color getColor() const { return color; }

    void updateParticle(float dt);
    void accelerate(sf::Vector2f a);
    void setVelocity(sf::Vector2f v, float dt);
    void setPosition(sf::Vector2f);
    void addVelocity(sf::Vector2f v, float dt);
    sf::Vector2f getVelocity() const;

};
