#ifndef STATESTACK_H
#define STATESTACK_H

#include "state.h"

#include <SFML/Window/Event.hpp>
#include <SFML/System/Time.hpp>
#include <SFML/System/NonCopyable.hpp>

#include <vector>
#include <map>
#include <functional>

class StateStack : public sf::NonCopyable
{
public:
    enum Action {
        Push,
        Pop,
        Clear
    };

public:
                StateStack(State::Context context);

    template<typename T>
    void        registerState(States::ID stateID);

    void        update(sf::Time dt);
    void        draw();
    void        handleEvent(const sf::Event& event);

    void        pushState(States::ID stateID);
    void        popState();
    void        clearStates();

    bool        isEmpty();

private:
    void        applyPendingChanges();
    State::Ptr  createState(States::ID stateID);

private:
    struct PendingChange {

        explicit PendingChange(Action action,
                               States::ID state = States::None);

        Action      action;
        States::ID  stateID;
    };

private:
    std::vector<State::Ptr>                             mStack;
    std::vector<PendingChange>                          mPendingList;

    State::Context                                      mContext;
    std::map<States::ID, std::function<State::Ptr()>>   mFactories;
};

template<typename T>
void StateStack::registerState(States::ID stateID)
{
    mFactories[stateID] = [this] ()
    {
        return State::Ptr(new T(*this, mContext));
    };

}

#endif // STATESTACK_H
