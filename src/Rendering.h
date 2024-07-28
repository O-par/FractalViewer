#ifndef RENDERING_H
#define RENDERING_H

#include <SFML/Graphics.hpp>

#define WIDTH  800
#define HEIGHT 600

class Rendering
{
public:
    sf::Vector2f computeJulia(sf::Vector2f current, sf::Vector2f constant);
    int computeIterations(sf::Vector2f z0, sf::Vector2f constant, int max_iteration);
    sf::Texture render(sf::Vector2f constant, int max_iteration, float zoomFactor);
};

#endif // RENDERING_H
