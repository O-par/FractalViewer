#include <SFML/Graphics.hpp>
#include <cmath>
#include <iostream>

#define WIDTH  800
#define HEIGHT 600

// g++ -c main.cpp
// g++ main.o -o sfml-app -lsfml-graphics -lsfml-window -lsfml-system
// ./sfml-app

float mod2(sf::Vector2f vec) {
    return vec.x * vec.x + vec.y * vec.y;
}

sf::Vector2f computeJulia(sf::Vector2f current, sf::Vector2f constant) {
    float real = current.x * current.x - current.y * current.y;
    float imag = 2.0f * current.x * current.y;
    return sf::Vector2f(real, imag) + constant; 
}

int computeIterations(sf::Vector2f z0, sf::Vector2f constant, int max_iteration) {
    int iteration = 0;
    sf::Vector2f zn = z0;

    while (mod2(zn) < 16 && iteration < max_iteration) {
        zn = computeJulia(zn, constant);
        iteration++;
    }

    return iteration;
}

sf::Texture render(sf::RenderTarget* window, sf::Vector2f constant, int max_iteration, float zoomFactor) {
    sf::Uint8 pixels[WIDTH * HEIGHT * 4];
    sf::Image img;
    sf::Texture tex;

    for (int y = 0; y < HEIGHT; y++) {
        for (int x = 0; x < WIDTH; x++) {
            // Map pixel to complex plane coordinates
            float real = ((float)x / WIDTH * 3.0f - 1.5f) / zoomFactor;
            float imag = ((float)y / HEIGHT * 3.0f - 1.5f) / zoomFactor;

            sf::Vector2f z0(real, imag);
            int it = computeIterations(z0, constant, max_iteration);

            float t = static_cast<float>(it) / max_iteration;

            sf::Uint8 r, g, b;
            
            if (t < 0.5f) {
                // Interpolate between yellow and orange
                t *= 2;
                r = 255;
                g = 255 - static_cast<sf::Uint8>((255 - 165) * t);
                b = 0;
            } else {
                // Interpolate between orange and red
                t = (t - 0.5f) * 2;
                r = 255;
                g = 165 - static_cast<sf::Uint8>(165 * t);
                b = 0;
            }            
            
            

            int color = 255 * it / max_iteration;
            pixels[(y * WIDTH + x) * 4]     =  r; // R
            pixels[(y * WIDTH + x) * 4 + 1] =  g; // G
            pixels[(y * WIDTH + x) * 4 + 2] =  b; // B
            pixels[(y * WIDTH + x) * 4 + 3] = 255; // A
        }
    }

    img.create(WIDTH, HEIGHT, pixels);
    tex.create(WIDTH, HEIGHT);
    tex.update(img);

    

    return tex;
    
}

int main() {
    sf::RenderWindow window(sf::VideoMode(WIDTH, HEIGHT), "Fractal Viewer");

    float zoom = 1.0f;
    sf::Texture juliaTex = render(&window, sf::Vector2f(-0.7f, 0.27015f), 500, zoom);
    sf::Sprite Julia(juliaTex);

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
        }

        

        window.clear();

        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
        {
            zoom *= 2.5f;
            std::cout<<"up"<< "\n";
            juliaTex = render(&window, sf::Vector2f(-0.7f, 0.27015f), 500, zoom);
            
        }
        
        Julia.setTexture(juliaTex);
        window.draw(Julia);

        window.display();
    }

    return 0;
}
