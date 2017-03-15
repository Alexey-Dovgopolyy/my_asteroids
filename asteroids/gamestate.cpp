#include "gamestate.h"
#include <QDebug>


GameState::GameState(StateStack &stack, Context context)
    : State(stack, context)
    , mWorld(*(context.window), *(context.fonts), context.player->getLevel())
    , mPlayer(*(context.player))
{
    mPlayer.setMissionStatus(PlayersInput::MissionRunning);
    //mWorld.setLevel(mPlayer.getLevel());
}

void GameState::draw()
{
    mWorld.draw();
}

bool GameState::update(sf::Time dt)
{
    mWorld.update(dt);

    if (!mWorld.isAnyAsteroidAlive()) {

        if (mWorld.getLevel() == Levels::Level5) {
            mPlayer.setMissionStatus(PlayersInput::MissionSuccess);
        }
        else {
            mPlayer.setLevel(static_cast<Levels::ID>(mWorld.getLevel() + 1));
        }

        //requestStackPop();
        requestStackPush(States::Changing);
    }

    CommandQueue& commands = mWorld.getCommandQueue();
    mPlayer.handleRealtimeInput(commands);
    return true;
}

bool GameState::handleEvent(const sf::Event &event)
{
    mPlayer.handleEvent(event, mWorld.getCommandQueue());
    return true;
}
