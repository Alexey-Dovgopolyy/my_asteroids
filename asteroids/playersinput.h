#ifndef PLAYERSINPUT_H
#define PLAYERSINPUT_H

#include "command.h"
#include "levelidentifiers.h"

#include <SFML/Window/Event.hpp>

#include <map>

class CommandQueue;

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
                    PlayersInput(sf::RenderTarget& window);

    void            handleEvent(const sf::Event& event,
                                CommandQueue& commands);
    void            handleRealtimeInput(CommandQueue& commands);
    void            hanleMouseMoves(CommandQueue& commands);

    void            setMissionStatus(MissionStatus status);
    MissionStatus   getMissionStatus() const;

    void            setLevel(Levels::ID level);
    Levels::ID      getLevel() const;

    void            setScore(int score);
    int             getScore() const;

    void            initializeActions();
    bool            isRealtimeAction(Action action);

    void            assignKey(Action action, sf::Keyboard::Key key);
    void            assignKey(Action action, sf::Mouse::Button button);
    sf::Keyboard::Key getAssignedKey(Action action) const;

private:
    sf::RenderTarget&                   mWindow;

    std::map<sf::Keyboard::Key, Action> mKeyBinding;
    std::map<sf::Mouse::Button, Action> mMouseKeyBinding;
    std::map<Action, Command>           mActionBinding;
    MissionStatus                       mCurrentMissionStatus;
    Levels::ID                          mLevel;
    int                                 mScore;
};

#endif // PLAYERSINPUT_H
