#include <SFML/Graphics.hpp>
#include <cmath>
#include <iostream>

#include "Rendering.h"

#define WIDTH  800
#define HEIGHT 600

// g++ -c main.cpp Rendering.cpp
// g++ main.o Rendering.o -o sfml-app -lsfml-graphics -lsfml-window -lsfml-system
// ./sfml-app

int main() {

    Rendering Renderer;

    sf::RenderWindow window(sf::VideoMode(WIDTH, HEIGHT), "Fractal Viewer");

    float zoom = 1.0f;
    sf::Texture juliaTex = Renderer.render(sf::Vector2f(-0.7f, 0.27015f), 500, zoom);
    sf::Sprite Julia(juliaTex);

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
        }

        window.clear();

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
            zoom *= 2.5f;
            std::cout << "up" << "\n";
            juliaTex = Renderer.render(sf::Vector2f(-0.7f, 0.27015f), 500, zoom);
        }
        
        Julia.setTexture(juliaTex);
        window.draw(Julia);

        window.display();
    }

    return 0;
}
