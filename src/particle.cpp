#pragma once
#include "particle.hpp"
#include "help.hpp"
#include <SFML/System/Vector2.hpp>

void Particle::updateParticle(float dt){
    sf::Vector2f temp_pos = this->getPosition();
    this->position = 
        mult(this->position, 2.0) - this->getPrevPosition() + ( this->getAcceleration() * dt * dt );
    this->prev_position = temp_pos;
    this->acceleration = {};
}
void Particle::accelerate(sf::Vector2f a){
    this->acceleration += a;
}
void Particle::setVelocity(sf::Vector2f v, float dt){
    this->prev_position = position - ( v * dt );
}
void Particle::setPosition(sf::Vector2f v){
    this->position = v;
}
void Particle::addVelocity(sf::Vector2f v, float dt){
    this->prev_position -= v * dt;
}
sf::Vector2f Particle::getVelocity() const{
    return this->position - this->prev_position;
}
