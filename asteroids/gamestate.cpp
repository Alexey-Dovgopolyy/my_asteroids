#include "gamestate.h"



GameState::GameState(sf::RenderWindow *renderContext)
    : mWorld(renderContext)
{

}

bool GameState::update(sf::Time dt)
{
    mWorld.update(dt);
    return true;
}

void GameState::draw()
{
    mWorld.draw();
}
