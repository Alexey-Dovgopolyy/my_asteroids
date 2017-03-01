#include "application.h"

const sf::Time Application::TimePerFrame = sf::seconds(1.f/60.f);

Application::Application()
    : mWindow(sf::VideoMode(800, 600), "Test")
    , mStatisticsText()
    , mStatisticsUpdateTime()
    , mStatisticsNumFrames(0)
{
    mFont.loadFromFile("Media/Sansation.ttf");
    mStatisticsText.setFont(mFont);
    mStatisticsText.setPosition(5.f, 5.f);
    mStatisticsText.setCharacterSize(10u);
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
//			if (mStateStack.isEmpty())
//				mWindow.close();
        }

        updateStatistics(dt);
        render();
    }
}

void Application::processInput()
{
    sf::Event event;
    while (mWindow.pollEvent(event)) {
        if (event.type == sf::Event::Closed)
        mWindow.close();
    }
}

void Application::update(sf::Time dt)
{
    //mStateStack.update(dt);
}

void Application::render()
{
    mWindow.clear();
    //mStateStack.draw();
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
