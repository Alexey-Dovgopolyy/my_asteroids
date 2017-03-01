#ifndef PLAYERSINPUT_H
#define PLAYERSINPUT_H

#include "command.h"

#include <SFML/Window/Event.hpp>

#include <map>

class PlayersInput
{
public:
    enum Action {

        MoveLeft,
        MoveRight,
        MoveUp,
        MoveDown,
        Fire,
        ActionCount
    };

    enum MissionStatus {

        MissionRunning,
        MissionSuccess,
        MissionFailue
    };

public:
    PlayersInput();

    void            handleEvent(const sf::Event& event,
                                CommandQueue& commands);
    void            handleRealtimeInput(CommandQueue& command);

    void            setMissionStatus(MissionStatus status);
    MissionStatus   getMissionStatus() const;

    void            initializeActions();
    bool            isRealtimeAction(Action action);

private:
    std::map<sf::Keyboard::Key, Action> mKeyBinding;
    std::map<Action, Command>           mActionBinding;
    MissionStatus                       mCurrentMissionStatus;
};

#endif // PLAYERSINPUT_H
