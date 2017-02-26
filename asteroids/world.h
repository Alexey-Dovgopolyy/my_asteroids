#ifndef WORLD_H
#define WORLD_H

#include "aircraft.h"

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Time.hpp>

class World
{
public:
    World(sf::RenderWindow* window);

    void update(sf::Time dt);
    void draw();
    void handleInput();
    void handleRealTimeInput(sf::Time dt);
    void adaptPlayerPosition();

private:
    sf::RenderWindow*   mWindow;
    sf::View            mWorldView;
    sf::FloatRect       mWorldBounds;
    Aircraft            mPlayerAircraft;
};

#endif // WORLD_H
