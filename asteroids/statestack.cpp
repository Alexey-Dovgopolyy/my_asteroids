#include "statestack.h"

#include <cassert>
#include <QDebug>

StateStack::StateStack(State::Context context)
    : mStack            ()
    , mPendingList      ()
    , mContext          (context)
    , mFactories        ()
{
}

StateStack::PendingChange::PendingChange(Action action,
                                         States::ID state)
    : action(action)
    , stateID(state)
{
}

void StateStack::update(sf::Time dt)
{
    for (auto it = mStack.rbegin(); it != mStack.rend(); ++it) {
        if (!(*it)->update(dt)) {
            break;
        }
    }
    applyPendingChanges();
}

void StateStack::draw()
{
    for (auto it = mStack.begin(); it != mStack.end(); ++it) {
        (*it)->draw();
    }
}

void StateStack::handleEvent(const sf::Event& event)
{
    for (auto it = mStack.begin(); it != mStack.end(); ++it) {
        (*it)->handleEvent(event);
    }
    applyPendingChanges();
}

void StateStack::pushState(States::ID stateID)
{
    mPendingList.push_back(PendingChange(Push, stateID));
}

void StateStack::popState()
{
    mPendingList.push_back(PendingChange(Pop));
}

void StateStack::clearStates()
{
    mPendingList.push_back(PendingChange(Clear));
}

bool StateStack::isEmpty()
{
    return mStack.empty();
}

void StateStack::applyPendingChanges()
{
    for (PendingChange act : mPendingList) {

        switch (act.action)
        {
        case Push:
            mStack.push_back(createState(act.stateID));
            break;
        case Pop:
            mStack.pop_back();
            break;
        case Clear:
            mStack.clear();
            break;
        }
    }
    mPendingList.clear();
}

State::Ptr StateStack::createState(States::ID stateID)
{
    auto found = mFactories.find(stateID);
    assert(found != mFactories.end());
    return found->second();
}
