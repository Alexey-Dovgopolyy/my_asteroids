#include "playersinput.h"
#include "aircraft.h"
#include "commandqueue.h"
#include <QDebug>

#include <SFML/Graphics/RenderWindow.hpp>

struct AircraftMover {

    AircraftMover(float vx, float vy)
        : velocity(vx, vy)
    {
    }

    void operator() (Aircraft& aircraft, sf::Time) const
    {
        aircraft.accelerate(velocity * 2.f);
    }

    sf::Vector2f velocity;
};

struct AircraftRotor {
    AircraftRotor(sf::Vector2i mousePos)
        : mousePosition(mousePos)
    {
    }

    void operator() (Aircraft& aircraft, sf::Time) const
    {
        float dx = mousePosition.x - aircraft.getPosition().x;
        float dy = aircraft.getPosition().y - mousePosition.y;

        float angle = std::atan2(dx, dy);
        float degree = toDegree(angle);
        //qDebug() << degree;

        aircraft.setDirection(degree);
        //aircraft.setRotation(degree);
    }

    sf::Vector2i mousePosition;
};

PlayersInput::PlayersInput(sf::RenderTarget &window)
    : mWindow(window)
    , mCurrentMissionStatus(MissionRunning)
{
    mKeyBinding[sf::Keyboard::A] = MoveLeft;
    mKeyBinding[sf::Keyboard::D] = MoveRight;
    mKeyBinding[sf::Keyboard::W] = MoveUp;
    mKeyBinding[sf::Keyboard::S] = MoveDown;

    mMouseKeyBinding[sf::Mouse::Left] = Fire;

    initializeActions();

    for (auto& pair : mActionBinding) {
        pair.second.category = Category::PlayerAircraft;
    }
}

void PlayersInput::handleEvent(const sf::Event& event,
                            CommandQueue& commands)
{

}

void PlayersInput::handleRealtimeInput(CommandQueue& commands)
{
    for (auto pair : mKeyBinding) {

        if (sf::Keyboard::isKeyPressed(pair.first) &&
                isRealtimeAction(pair.second)) {
            commands.push(mActionBinding[pair.second]);
        }
    }

    for (auto pair : mMouseKeyBinding) {

        if (sf::Mouse::isButtonPressed(pair.first) &&
                isRealtimeAction(pair.second)) {
            commands.push(mActionBinding[pair.second]);
        }
    }

    hanleMouseMoves(commands);
}

void PlayersInput::hanleMouseMoves(CommandQueue &commands)
{
    sf::Vector2i mousePosition =
            sf::Mouse::getPosition(static_cast<sf::RenderWindow&>(mWindow));
    Command command;
    command.category = Category::PlayerAircraft;
    command.action = derivedAction<Aircraft>(AircraftRotor(mousePosition));
    commands.push(command);
}

void PlayersInput::setMissionStatus(MissionStatus status)
{
    mCurrentMissionStatus = status;
}

PlayersInput::MissionStatus PlayersInput::getMissionStatus() const
{
    return mCurrentMissionStatus;
}

void PlayersInput::initializeActions()
{
    mActionBinding[MoveLeft].action  = derivedAction<Aircraft>(AircraftMover(-1,  0));
    mActionBinding[MoveRight].action = derivedAction<Aircraft>(AircraftMover(+1,  0));
    mActionBinding[MoveUp].action    = derivedAction<Aircraft>(AircraftMover( 0, -1));
    mActionBinding[MoveDown].action  = derivedAction<Aircraft>(AircraftMover( 0, +1));
    mActionBinding[Fire].action      = derivedAction<Aircraft>(
                                    [] (Aircraft& a, sf::Time)
                                    {
                                        a.fire();
                                    });

}

bool PlayersInput::isRealtimeAction(Action action)
{
    switch (action)
    {
        case MoveLeft:
        case MoveRight:
        case MoveDown:
        case MoveUp:
        case Fire:
            return true;

        default:
            return false;
    }
}
