#include <SFML/Graphics.hpp>
#include <cmath>
#include <iostream>

float mod2(sf::Vector2f vec) {
    return vec.x * vec.x + vec.y * vec.y;
}