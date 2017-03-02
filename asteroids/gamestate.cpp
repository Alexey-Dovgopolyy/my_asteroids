#include "gamestate.h"



GameState::GameState(StateStack &stack, Context context)
    : State(stack, context)
    , mWorld(*(context.window), *(context.fonts))
    , mPlayer(*(context.player))
{
    mPlayer.setMissionStatus(PlayersInput::MissionRunning);
}

void GameState::draw()
{
    mWorld.draw();
}

bool GameState::update(sf::Time dt)
{
    mWorld.update(dt);
    CommandQueue& commands = mWorld.getCommandQueue();
    mPlayer.handleRealtimeInput(commands);
    return true;
}

bool GameState::handleEvent(const sf::Event &event)
{
    mPlayer.handleEvent(event, mWorld.getCommandQueue());
    return true;
}
