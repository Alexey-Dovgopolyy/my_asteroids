#include "container.h"

#include <SFML/Window/Event.hpp>
#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/RenderTarget.hpp>

#include <QDebug>

namespace GUI {

Container::Container(sf::RenderWindow* window)
    : mChildren()
    , mSelectedChild(-1)
    , mActiveChild(-1)
    , mWindow(window)
{
}

void Container::pack(Component::Ptr component)
{
    mChildren.push_back(component);

    if (!hasSelection() && component->isSelectable()) {
        select(mChildren.size() - 1);
    }
}

bool Container::isSelectable() const
{
    return false;
}

void Container::handleEvent(const sf::Event& event)
{    
    if (event.type == sf::Event::MouseButtonReleased) {

        sf::Vector2f mousePos =
                            static_cast<sf::Vector2f>
                            (sf::Mouse::getPosition(static_cast<sf::RenderWindow&>(*mWindow)));

        if (event.key.code == sf::Mouse::Left) {

            for (std::vector<Component::Ptr>::size_type i = 0; i < mChildren.size(); ++i) {
                if (mChildren[i]->getRect().contains(mousePos)) {
                    if (mChildren[i]->isActive()) {
                        mChildren[i]->deactivate();
                    }
                    else {
                        deactivateAll();
                        mActiveChild = static_cast<int>(i);
                        mSelectedChild = static_cast<int>(i);
                        mChildren[i]->activate();
                    }
                }
            }
        }
    }

    if (hasSelection() && mChildren[mSelectedChild]->isActive()) {
       mChildren[mSelectedChild]->handleEvent(event);
    }
    else if (event.type == sf::Event::KeyReleased) {

        if (event.key.code == sf::Keyboard::Escape) {

            for (auto button : mChildren) {
                if (button->isActive()) {
                    mSelectedChild = -1;
                    mActiveChild = -1;
                    button->deactivate();
                    break;
                }
            }
        }
        else if (event.key.code == sf::Keyboard::Down) {
            selectNext();
        }
        else if (event.key.code == sf::Keyboard::Up) {
            selectPrevious();
        }
        else if (event.key.code == sf::Keyboard::Return
                  || event.key.code == sf::Keyboard::Space) {
            if (hasSelection()) {
                mChildren[mSelectedChild]->activate();
            }
        }        
    }
}

void Container::handleRealtimeInput()
{
    sf::Vector2f mousePos =
                        static_cast<sf::Vector2f>
                        (sf::Mouse::getPosition(static_cast<sf::RenderWindow&>(*mWindow)));


    for (std::vector<Component::Ptr>::size_type i = 0; i < mChildren.size(); ++i) {

        sf::FloatRect bounds = mChildren[i]->getRect();
        if (bounds.contains(mousePos)) {

            if (mChildren[i]->isActive()) {
                if (mSelectedChild != mActiveChild) {
                    mChildren[mSelectedChild]->deselect();
                }
            }
            else {
                if (!mChildren[mSelectedChild]->isActive()) {
                    mChildren[mSelectedChild]->deselect();
                }
                mChildren[i]->select();
                mSelectedChild = static_cast<int>(i);
            }
//            if (hasSelection()) {
//                mChildren[mSelectedChild]->deselect();
//            }
//            if (mChildren[mSelectedChild]->isActive()) {
//                mChildren[mSelectedChild]->activate();
//            }
//            if (!mChildren[i]->isActive()) {
//                mChildren[i]->select();
//            }
            //mSelectedChild = static_cast<int>(i);
        }
    }
}


void Container::draw(sf::RenderTarget& target,
                                    sf::RenderStates states) const
{
    states.transform *= getTransform();

    for (const Component::Ptr& child : mChildren) {
        target.draw(*child, states);
    }
}

bool Container::hasSelection() const
{
    return mSelectedChild >= 0;
}

void Container::select(std::size_t index)
{
    if (mChildren[index]->isSelectable()) {

        if (hasSelection()) {
            mChildren[mSelectedChild]->deselect();
        }

        mChildren[index]->select();
        mSelectedChild = index;
    }
}

void Container::selectNext()
{
    if (!hasSelection()) {
        return;
    }

    int next = mSelectedChild;
    do {
        next = (next + 1) % mChildren.size();
    } while (!mChildren[next]->isSelectable());

    select(next);
}

void Container::selectPrevious()
{
    if (!hasSelection()) {
        return;
    }

    int prev = mSelectedChild;
    do {
        prev = (prev + mChildren.size() - 1) % mChildren.size();
    } while (!mChildren[prev]->isSelectable());

    select(prev);
}

void Container::deactivateAll()
{
    for (Component::Ptr child : mChildren) {
        mActiveChild = -1;
        mSelectedChild = -1;
        child->deselect();
        child->deactivate();
    }
}

}
