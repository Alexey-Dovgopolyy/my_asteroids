#include "state.h"
#include "statestack.h"

State::Context::Context(sf::RenderWindow& window, TextureHolder& textures,
        FontHolder& fonts, PlayersInput& player)
    : window    (&window)
    , textures  (&textures)
    , fonts     (&fonts)
    , player    (&player)
{

}

State::State(StateStack &stack, Context context)
    : mStack(&stack)
    , mContext(context)
{
}

State::~State()
{
}

void State::requestStackPush(States::ID stateID)
{
    mStack->pushState(stateID);
}

void State::requestStackPop()
{
    mStack->popState();
}

void State::requestStackClear()
{
    mStack->clearStates();
}

State::Context State::getContext() const
{
    return mContext;
}
