#include "playersinput.h"
#include "aircraft.h"

struct AircraftMover {

    AircraftMover(float vx, float vy)
        : velocity(vx, vy)
    {
    }

    void operator() (Aircraft& aircraft, sf::Time) const
    {
        aircraft.setVelocity(velocity);
    }

    sf::Vector2f velocity;
};

PlayersInput::PlayersInput()
    : mCurrentMissionStatus(MissionRunning)
{
    mKeyBinding[sf::Keyboard::A] = MoveLeft;
    mKeyBinding[sf::Keyboard::D] = MoveRight;
    mKeyBinding[sf::Keyboard::W] = MoveUp;
    mKeyBinding[sf::Keyboard::S] = MoveDown;

    initializeActions();

    for (auto& pair : mActionBinding) {
        pair.second.category = Category::PlayerAircraft;
    }
}

void PlayersInput::handleEvent(const sf::Event& event,
                            CommandQueue& commands)
{

}

void PlayersInput::handleRealtimeInput(CommandQueue& command)
{

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
                                        //a.fire();
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
