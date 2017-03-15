#ifndef STATECHANGINGLEVEL_H
#define STATECHANGINGLEVEL_H

#include "state.h"

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Text.hpp>

class ChangingLevelState : public State
{
public:
    ChangingLevelState(StateStack& stack, Context context);

    virtual void draw();
    virtual bool update(sf::Time dt);
    virtual bool handleEvent(const sf::Event &event);

private:
    sf::Text            mText;
    sf::Time            mElapsedTime;
    PlayersInput&       mPlayer;
};

#endif // STATECHANGINGLEVEL_H
