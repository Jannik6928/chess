
#include <SFML/Graphics.hpp>

#include <map>

#include "Position.h"
#include "Interface.h"

using namespace std;

int main() {

    Interface interface(standardSetup(), standardTextures());
    interface.setDoFlip(false);

    sf::RenderWindow window(sf::VideoMode(1000, 800), "Chess");
    window.setFramerateLimit(20);
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) window.close();
            if (event.type == sf::Event::MouseButtonPressed) {
                if (event.mouseButton.button == sf::Mouse::Left) {
                    int x = floor(event.mouseButton.x / 100);
                    int y = floor(event.mouseButton.y / 100);

                    if (interface.isFlipped()) {
                        interface.tryMove(Point(x, 7 - y));
                        interface.toggleSelected(Point(x, 7 - y));
                    }
                    else {
                        interface.tryMove(Point(x, y));
                        interface.toggleSelected(Point(x, y));
                    }
                }
            }
            if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::M) {
                    delete interface.getPosition();
                    interface.setPosition(standardSetup());
                }
            }
        }

        window.clear();
        window.draw(interface);
        window.display();
    }

    return 0;
}