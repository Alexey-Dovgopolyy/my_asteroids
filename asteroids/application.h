#ifndef APPLICATION_H
#define APPLICATION_H

#include "gamestate.h"
#include "playersinput.h"

#include <SFML/System/Time.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Text.hpp>

class Application
{
public:
    Application();
    void run();

private:
    void processInput();
    void update(sf::Time dt);
    void render();

    void updateStatistics(sf::Time dt);

private:
    static const sf::Time   TimePerFrame;

    sf::Font                mFont;
    sf::RenderWindow        mWindow;

    GameState               mStateStack;
    sf::Text                mStatisticsText;
    sf::Time                mStatisticsUpdateTime;
    std::size_t             mStatisticsNumFrames;

    PlayersInput            mPlayersInput;

};

#endif // APPLICATION_H
