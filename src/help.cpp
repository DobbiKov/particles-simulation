#pragma once
#include <SFML/System/Vector2.hpp>
sf::Vector2f mult(sf::Vector2f vec, float f){
    return
        sf::Vector2f{
            vec.x * f,
            vec.y * f
        };
}

