#include "world.h"
#include <QDebug>
#include <algorithm>

World::World(sf::RenderWindow *window)
    : mWindow(window)
    , mWorldView(window->getDefaultView())
{
    mPlayerAircraft.setCoordinates(window->getSize().x / 2.f,
                                   window->getSize().y / 2.f);
    qDebug() << "World init" << mWorldView.getSize().x << " " << mWorldView.getSize().y;
    qDebug() << "Player init" << mPlayerAircraft.getCoordinates().x << " " <<
                mPlayerAircraft.getCoordinates().y;
}

void World::update(sf::Time dt)
{
    handleInput();
    handleRealTimeInput(dt);
    adaptPlayerPosition();
}

void World::draw()
{
    mWindow->draw(mPlayerAircraft);
}

void World::handleInput()
{
    sf::Event event;
    while (mWindow->pollEvent(event)) {

        if (event.type == sf::Event::Closed)
        mWindow->close();

    }
}

void World::handleRealTimeInput(sf::Time dt)
{
    float x = 0;
    float y = 0;

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
        y = -0.05f;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
        y = 0.05f;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
        x = -0.05f;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
        x = 0.05f;
    }
    mPlayerAircraft.updateCoordinates(x, y);

    //qDebug() << x << " " << y;
    sf::Vector2i mousePosition = sf::Mouse::getPosition(*mWindow);
    mPlayerAircraft.updateDirection(mousePosition);    
}

void World::adaptPlayerPosition()
{
    sf::Vector2f worldViewSize = mWorldView.getSize();
    sf::Vector2f playersCoords = mPlayerAircraft.getCoordinates();

    //qDebug() << "World" << worldViewSize.x << " " << worldViewSize.y;
    //qDebug() << "Player" << playersCoords.x << " " << playersCoords.y;

    if (playersCoords.x > worldViewSize.x + 20.f)  playersCoords.x = 0.f - 20.f;
    if (playersCoords.x < 0.f - 20.f)              playersCoords.x = worldViewSize.x + 20.f;
    if (playersCoords.y > worldViewSize.y + 20.f)  playersCoords.y = 0.f - 20.f;
    if (playersCoords.y < 0.f - 20.f)              playersCoords.y = worldViewSize.y + 20.f;

    mPlayerAircraft.setCoordinates(playersCoords.x, playersCoords.y);
}
