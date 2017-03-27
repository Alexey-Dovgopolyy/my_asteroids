#ifndef PAUSESTATE_H
#define PAUSESTATE_H

#include <SFML/System/Time.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Text.hpp>

#include "state.h"
#include "playersinput.h"
#include "container.h"

class PauseState : public State
{
public:
    PauseState(StateStack& stack, Context context);

    virtual void draw();
    virtual bool update(sf::Time dt);
    virtual bool handleEvent(const sf::Event &event);

private:
    GUI::Container  mGUIContainer;
    sf::Text        mPauseText;
};

#endif // PAUSESTATE_H
