#ifndef STATE_H
#define STATE_H

#include "resourceidentifiers.h"
#include "stateidentifiers.h"

#include <SFML/System/Time.hpp>
#include <SFML/Window/Event.hpp>

#include <memory>

namespace sf {
    class RenderWindow;
}

class StateStack;
class PlayersInput;
class MusicPlayer;
class SoundPlayer;

class State
{
public:
    typedef std::unique_ptr<State> Ptr;

    struct Context {

        Context(sf::RenderWindow& window, TextureHolder& textures,
                FontHolder& fonts, PlayersInput& player,
                MusicPlayer& musicPlayer, SoundPlayer& sound);

        sf::RenderWindow*   window;
        TextureHolder*      textures;
        FontHolder*         fonts;
        PlayersInput*       player;
        MusicPlayer*        music;
        SoundPlayer*        sounds;
    };

public:
                    State(StateStack& stack, Context context);
    virtual         ~State();

    virtual void    draw() = 0;
    virtual bool    update(sf::Time dt) = 0;
    virtual bool    handleEvent(const sf::Event& event) = 0;

protected:
    void            requestStackPush(States::ID stateID);
    void            requestStackPop();
    void            requestStackClear();

    Context         getContext() const;

private:
    StateStack*     mStack;
    Context         mContext;
};

#endif // STATE_H
