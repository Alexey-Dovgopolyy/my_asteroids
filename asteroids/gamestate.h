#ifndef GAMESTATE_H
#define GAMESTATE_H

#include <SFML/Graphics.hpp>
#include <SFML/System/Time.hpp>

#include <QDebug>

#include "aircraft.h"
#include "world.h"

class GameState
{
public:

    GameState(sf::RenderWindow* renderContext);
    bool update(sf::Time dt);
    void draw();

private:
    World               mWorld;
    sf::RenderWindow*   mContext;
};

#endif // GAMESTATE_H
