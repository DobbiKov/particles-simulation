#include <SFML/Graphics.hpp>
#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <SFML/Window/Mouse.hpp>
#include <cstdint>
#include "help.hpp"
#include "solver.cpp"
#include <iostream>
#include "particle.hpp"

#define DEFAULT_WIDTH 1000 
#define DEFAULT_HEIGHT 600 
#define DEFAULT_FRAMERATE 60

static sf::Color getColor(float t) {
    const float r = sin(t);
    const float g = sin(t + 0.33f * 2.0f * M_PI);
    const float b = sin(t + 0.66f * 2.0f * M_PI);
    return {static_cast<uint8_t>(255.0f * r * r),
            static_cast<uint8_t>(255.0f * g * g),
            static_cast<uint8_t>(255.0f * b * b)};
}

int main()
{
    auto window = sf::RenderWindow(sf::VideoMode({DEFAULT_WIDTH, DEFAULT_HEIGHT}), "simulation");

    uint32_t width = DEFAULT_WIDTH;
    uint32_t height = DEFAULT_HEIGHT;
    uint32_t frameRate = DEFAULT_FRAMERATE;
    window.setFramerateLimit(frameRate);

   Solver solver = Solver(); 

    solver.setBoundary({width/2.0f, height/2.0f}, 300.0f);
    /*solver.addObject(Particle(*/
    /*        sf::Vector2f(400.0f, 100.0f), */
    /*        sf::Vector2f(400.0f, 100.0f), */
    /*        sf::Vector2f(0,0), */
    /*        sf::Vector2f(0, 0), */
    /*        10.0f, */
    /*        sf::Color::Red)*/
    /*        );*/
    /*solver.addObject(Particle(*/
    /*        sf::Vector2f(550.0f, 50), */
    /*        sf::Vector2f(550.0f, 50), */
    /*        sf::Vector2f(0, 0), */
    /*        sf::Vector2f(0, 0), */
    /*        10.0f, */
    /*        sf::Color::Red)*/
    /*        );*/
    float time = 0.0f;
    float dt = 1.0f/frameRate;

    bool n_press = false;
    bool n_click = false;
    while (window.isOpen())
    {
        dt = 1.0f/frameRate;
        time += dt;
        dt = dt/SUB_STEP_NUM;
        //apply gravity
        solver.update(dt);

        /*std::cout << "Default window size: "<< window.getSize().x << " " << window.getSize().y << "\n";*/
        /*sf::View defaultView = window.getDefaultView();*/
        /**/
        /*std::cout << "Default View Size: " << defaultView.getSize().x << " x " << defaultView.getSize().y << std::endl;*/
        /*std::cout << "Default View Center: " << defaultView.getCenter().x << ", " << defaultView.getCenter().y << std::endl;*/


        while (const std::optional event = window.pollEvent())
        {
            if(sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)){
                float ratio = (float)DEFAULT_WIDTH / window.getSize().x;
                sf::Vector2f res = static_cast<sf::Vector2f>(sf::Mouse::getPosition(window)) * ratio;
                solver.pullObjects(res);
            }
            if(sf::Mouse::isButtonPressed(sf::Mouse::Button::Right)){
                float ratio = (float)DEFAULT_WIDTH / window.getSize().x;
                sf::Vector2f res = static_cast<sf::Vector2f>(sf::Mouse::getPosition(window)) * ratio;
                solver.pushObjects(res);
            }
            if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::N)){
                n_press = true;
            }

            if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left)){
                solver.setLeftGravity();
            }
            if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up)){
                solver.setTopGravity();
            }
            if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right)){
                solver.setRightGravity();
            }
            if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down)){
                solver.setBottomGravity();
            }

            if(event->is<sf::Event::KeyReleased>()){
                const sf::Event::KeyReleased* ev = event->getIf<sf::Event::KeyReleased>();
                if(ev->code == sf::Keyboard::Key::N){
                    if(n_press) n_click = true; 
                    n_press = false;
                } 
            }
            if (event->is<sf::Event::Closed>())
            {
                window.close();
            }
        }

    
        window.clear();

        if(n_click) {
            n_click = false;
            //spawing a ball
            float ratio = (float)DEFAULT_WIDTH / window.getSize().x;
            sf::Vector2f res = static_cast<sf::Vector2f>(sf::Mouse::getPosition(window)) * ratio;
            solver.new_ball(res, getColor(time));
        }

        solver.render(window);

        window.display();
    }
}
