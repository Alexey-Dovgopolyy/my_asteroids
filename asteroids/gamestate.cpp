#include "gamestate.h"
#include <QDebug>


GameState::GameState(StateStack &stack, Context context)
    : State(stack, context)
    , mWorld(*(context.window),
             *(context.fonts),
             *(context.sounds),
             context.player->getLevel(),
             context.player->getScore(),
             context.player->getHitpoints())
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

    if (mWorld.isPlayerDead()) {
        mPlayer.setMissionStatus(PlayersInput::MissionFailue);
        requestStackPush(States::Changing);
        mPlayer.setLevel(Levels::Level1);
        mPlayer.setScore(0);
    }
    if (!mWorld.isAnyAsteroidAlive()) {

        if (mWorld.getLevel() == Levels::Level5) {
            mPlayer.setMissionStatus(PlayersInput::MissionSuccess);
            mPlayer.setLevel(Levels::Level1);
            mPlayer.setScore(0);            
        }
        else {
            mPlayer.setLevel(static_cast<Levels::ID>(mWorld.getLevel() + 1));
            mPlayer.setScore(mWorld.getScore());
            mPlayer.setHitpoints(mWorld.getHitpoints() >= 150 ?
                                            200 : mWorld.getHitpoints() + 50);
        }

        requestStackPush(States::Changing);
    }

    CommandQueue& commands = mWorld.getCommandQueue();
    mPlayer.handleRealtimeInput(commands);
    return true;
}

bool GameState::handleEvent(const sf::Event &event)
{
    if (event.type == sf::Event::KeyReleased && event.key.code == sf::Keyboard::Escape) {
        requestStackPush(States::Pause);
    }

    mPlayer.handleEvent(event, mWorld.getCommandQueue());
    return true;
}
