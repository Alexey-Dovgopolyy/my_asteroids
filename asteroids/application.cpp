#include "application.h"
#include "state.h"
#include "gamestate.h"
#include "titlestate.h"
#include "menustate.h"
#include "settingstate.h"
#include "statechanginglevel.h"
#include "stateidentifiers.h"

#include <QTime>

const sf::Time Application::TimePerFrame = sf::seconds(1.f/60.f);

Application::Application()
    : mWindow(sf::VideoMode(800, 600), "Test", sf::Style::Close)
    , mTextures()
    , mFonts()
    , mPlayersInput(mWindow)
    , mStateStack(State::Context(mWindow, mTextures,
                                 mFonts, mPlayersInput))
    , mStatisticsText()
    , mStatisticsUpdateTime()
    , mStatisticsNumFrames(0)
{
    mTextures.load(Textures::TitleScreen,
                                "Media/Textures/TitleScreen.png");
    mTextures.load(Textures::Buttons,
                                "Media/Textures/Buttons.png");

    mFonts.load(Fonts::Main, "Media/Sansation.ttf");
    mStatisticsText.setFont(mFonts.get(Fonts::Main));
    mStatisticsText.setPosition(5.f, 5.f);
    mStatisticsText.setCharacterSize(10u);

    registerStates();
    mStateStack.pushState(States::Title);
}

void Application::run()
{


    sf::Clock clock;
    sf::Time timeSinceLastUpdate = sf::Time::Zero;

    while (mWindow.isOpen()) {
        sf::Time dt = clock.restart();
        timeSinceLastUpdate += dt;
        while (timeSinceLastUpdate > TimePerFrame) {

            timeSinceLastUpdate -= TimePerFrame;

            processInput();
            update(TimePerFrame);

            // Check inside this loop, because stack might be empty before update() call
            if (mStateStack.isEmpty())
                mWindow.close();

        }

        updateStatistics(dt);
        render();
    }
}

void Application::processInput()
{
    sf::Event event;

    while (mWindow.pollEvent(event))
    {
        mStateStack.handleEvent(event);

        if (event.type == sf::Event::Closed)
            mWindow.close();
    }
}

void Application::update(sf::Time dt)
{
    mStateStack.update(dt);
}

void Application::render()
{
    mWindow.clear();
    mStateStack.draw();
    mWindow.setView(mWindow.getDefaultView());
    mWindow.draw(mStatisticsText);
    mWindow.display();
}

void Application::updateStatistics(sf::Time dt)
{
    mStatisticsUpdateTime += dt;
    mStatisticsNumFrames += 1;
    if (mStatisticsUpdateTime >= sf::seconds(1.0f)) {

        mStatisticsText.setString("FPS: "
            + QString::number(mStatisticsNumFrames).toStdString());

        mStatisticsUpdateTime -= sf::seconds(1.0f);
        mStatisticsNumFrames = 0;
    }
}

void Application::registerStates()
{
    mStateStack.registerState<TitleState>(States::Title);
    mStateStack.registerState<MenuState>(States::Menu);
    mStateStack.registerState<SettingState>(States::Settings);
    mStateStack.registerState<GameState>(States::Game);
    mStateStack.registerState<ChangingLevelState>(States::Changing);
}
