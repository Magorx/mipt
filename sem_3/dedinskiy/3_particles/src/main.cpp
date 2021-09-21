#include <SFML/Graphics.hpp>
#include <cmath>

const size_t SCR_W = 1200;
const size_t SCR_H =  800;

int main()
{
    sf::RenderWindow window(sf::VideoMode(SCR_W, SCR_H), "*~ Harry Plotter ~*");

    sf::Clock clock;

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        window.clear();

        // ====================================================================
        
        // ====================================================================

        window.display();
    }

    return 0;
}
