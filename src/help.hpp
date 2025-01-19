#pragma once
#include <SFML/System/Vector2.hpp>

/// multiplies 2d vector on a constant
sf::Vector2f mult(sf::Vector2f vec, float f);

/// calculate euclidian distance between two 2d vectors
float eucl_distance(const sf::Vector2f &obj1, const sf::Vector2f &obj2);

/// calculates dot product of two 2d vectors
float dot_product(const sf::Vector2f &obj1, const sf::Vector2f &obj2);
