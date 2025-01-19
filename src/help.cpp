#pragma once
#include <SFML/System/Vector2.hpp>
#include <cmath>
sf::Vector2f mult(sf::Vector2f vec, float f){
    return
        sf::Vector2f{
            vec.x * f,
            vec.y * f
        };
}

float eucl_distance(const sf::Vector2f &obj1, const sf::Vector2f &obj2){
    float sum = 0;
    sum += (obj2.x - obj1.x) * (obj2.x - obj1.x);
    sum += (obj2.y - obj1.y) * (obj2.y - obj1.y);
    return std::sqrt( sum );
}

float dot_product(const sf::Vector2f &obj1, const sf::Vector2f &obj2){
    return obj1.x * obj2.x + obj1.y * obj2.y;
}
