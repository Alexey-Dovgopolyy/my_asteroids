#ifndef GAMESTATE_H
#define GAMESTATE_H

#include <SFML/Graphics.hpp>
#include <SFML/System/Time.hpp>

#include <QDebug>

#include "state.h"
#include "world.h"
#include "playersinput.h"


class GameState : public State
{
public:
    GameState(StateStack& stack, Context context);

    void draw();
    bool update(sf::Time dt);
    bool handleEvent(const sf::Event &event);


private:
    World           mWorld;
    PlayersInput&   mPlayer;
};

#endif // GAMESTATE_H
