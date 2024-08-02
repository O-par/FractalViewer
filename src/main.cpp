#include <SFML/Graphics.hpp>
#include <cmath>
#include <algorithm>
#include <fstream>


#include "helper.h"


#define Vec2 sf::Vector2f
#define IVec2 sf::Vector2i

// Global Var
IVec2 RENDERSIZE{1920/2, 1080/2};
Vec2 CENTER{0.0f,0.0f};
float ZOOM = 1.0f;
Vec2 CONSTANT = {-0.4, 0.6};
int MAX_ITERATIONS = 500;


// FUNCTIONS:
// Zn² + C int main()


// Computes the next iteration of the Julia set
Vec2 computeNext(Vec2 vec, Vec2 constant)
{
    // (x + yi)² = x² + 2xyi - y²
    const float zr = vec.x * vec.x - vec.y * vec.y;
    const float zi = 2.0 * vec.x * vec.y;

    return Vec2{zr, zi} + constant;
}

// Returns # of iterations reached before max ModSquared or max iterations is reached 
int computeIterations(Vec2 z0, Vec2 constant, int max_iterations)
{
    Vec2 zn = z0;
    int iteration = 0;

    while (mod2(zn) < 4.0 && iteration < max_iterations)
    {
        zn = computeNext(zn, constant);
        iteration++;
    }

    return iteration;
}

float computeIterationsSmooth(Vec2 z0, Vec2 constant, int max_iterations)
{
    Vec2 zn = z0;
    int iteration = 0;

    while (mod2(zn) < 4.0 && iteration < max_iterations)
    {
        zn = computeNext(zn, constant);
        iteration++;
    }

    const float mod = sqrt(mod2(zn));
    const float smooth_iteration = float(iteration) + 1.0f - log2(log2(mod) / log2(4.0f));
    return smooth_iteration;
}

// AI generated
sf::Color getColorSmooth(float iterations, int max_iterations)
{
    float t = iterations / max_iterations;
    int r = static_cast<int>(9 * (1 - t) * t * t * t * 255);
    int g = static_cast<int>(15 * (1 - t) * (1 - t) * t * t * 255);
    int b = static_cast<int>(8.5 * (1 - t) * (1 - t) * (1 - t) * t * 255);
    return sf::Color(r, g, b);
}


void render(sf::RenderWindow* window, IVec2 render_size, Vec2 constant, Vec2 center,int max_iterations, float zoom)
{
    // compute scale of coords
    const float scale = 1.0f /float(render_size.y / 2) / zoom;

    sf::Image image;
    image.create(render_size.x, render_size.y);


    for(int y{0}; y < render_size.y ; ++y)
    {
        for(int x{0}; x < render_size.x; ++x)
        {
               // pixel's coords
            // as the scale gets smaller the domain/range gets smaller therefore zooming
            // inversly as it gets bigger, it zooms out 
            // adding to the pixels shifts the image left, right, up, or down
            const float px = float(x - render_size.x / 2) * (scale) + center.x;
            const float py = float(y - render_size.y / 2) * (scale) + center.y;

            //const float iterations = computeIterations({px, py}, constant, max_iterations);
            const float iterations = computeIterationsSmooth({px, py}, constant, max_iterations);
            //sf::Uint8 colorValue = static_cast<sf::Uint8>(255 * iterations / 50.0f);
            sf::Color color = getColorSmooth(iterations, max_iterations);
            //sf::Color color(colorValue, colorValue, colorValue);

            image.setPixel(x, y, color);
        }
    }

    sf::Texture texture;
    texture.loadFromImage(image);
    sf::Sprite sprite(texture);
    window->draw(sprite);

}



int main()
{
    sf::RenderWindow window(sf::VideoMode(RENDERSIZE.x, RENDERSIZE.y), "SFML works!");
    
    

    

    

    sf::Font font;
    if (!font.loadFromFile("../FractalViewer/Minecraft.ttf"))
    {
        return -1;
    }

    
    
    sf::Text text_zoom("Zoom: 1.000000", font);
    text_zoom.setCharacterSize(30);  // Set a larger character size
    text_zoom.setFillColor(sf::Color::White);
    text_zoom.setPosition(10, 5);
    
    sf::Text text_center("[0, 0]", font);
    text_center.setCharacterSize(30);
    text_center.setFillColor(sf::Color::White);
    text_center.setPosition(10, 40);
    
    sf::Text text_constant("C: {-0.4, 0.6}", font);
    text_constant.setCharacterSize(30);
    text_constant.setFillColor(sf::Color::White);
    text_constant.setPosition(10, 75);

    sf::Text text_iterations("it: 500", font);
    text_iterations.setCharacterSize(30);
    text_iterations.setFillColor(sf::Color::White);
    text_iterations.setPosition(10, 110);
    

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();

            // mouse clicks
            if (event.type == sf::Event::MouseButtonPressed)
            {
                if (event.mouseButton.button == sf::Mouse::Left)
                {
                    
                    float scale = 1.0f / float(RENDERSIZE.y / 2) / ZOOM;
                    float mouseX = float(event.mouseButton.x - RENDERSIZE.x / 2) * scale;
                    float mouseY = float(event.mouseButton.y - RENDERSIZE.y / 2) * scale;
                    CENTER.x += mouseX;
                    CENTER.y += mouseY;
                    text_center.setString("[" + std::to_string(event.mouseButton.x) + ", " + std::to_string(event.mouseButton.y) + "]" );

                }
            }
            
            // keyboard clicks
            if (event.type == sf::Event::KeyPressed)
            {
                // zoom
                if (event.key.code == sf::Keyboard::Up)
                    ZOOM /= 0.5f;
                    text_zoom.setString("Zoom: " + std::to_string(ZOOM));
                if (event.key.code == sf::Keyboard::Down)
                    ZOOM *= 0.5f;
                    text_zoom.setString("Zoom: " + std::to_string(ZOOM));

                // real++
                if (event.key.code == sf::Keyboard::Q)
                {
                    CONSTANT.x += 0.005;
                    text_constant.setString("C: {" + std::to_string(CONSTANT.x) + ", " + std::to_string(CONSTANT.y) + "}");
                }
                // real--
                if (event.key.code == sf::Keyboard::A)
                {
                    CONSTANT.x -= 0.005;
                    text_constant.setString("C: {" + std::to_string(CONSTANT.x) + ", " + std::to_string(CONSTANT.y) + "}");
                }
                // im++
                if (event.key.code == sf::Keyboard::W)
                {
                    CONSTANT.y += 0.005;
                    text_constant.setString("C: {" + std::to_string(CONSTANT.x) + ", " + std::to_string(CONSTANT.y) + "}");
                }
                // im--
                if (event.key.code == sf::Keyboard::S)
                {
                    CONSTANT.y -= 0.005;
                    text_constant.setString("C: {" + std::to_string(CONSTANT.x) + ", " + std::to_string(CONSTANT.y) + "}");
                }

                // it++
                if (event.key.code == sf::Keyboard::E)
                {
                    MAX_ITERATIONS += 100;
                    text_iterations.setString("it: " + std::to_string(MAX_ITERATIONS));
                }
                // it--
                if (event.key.code == sf::Keyboard::D)
                {
                    MAX_ITERATIONS -= 100;
                    text_iterations.setString("it: " + std::to_string(MAX_ITERATIONS));
                }
            }

        
        }

        window.clear();
        
        render(&window,RENDERSIZE, CONSTANT, CENTER, MAX_ITERATIONS, ZOOM);

        
        window.draw(text_zoom);
        window.draw(text_center);
        window.draw(text_constant);
        window.draw(text_iterations);
        
        window.display();
    }

    return 0;
} 