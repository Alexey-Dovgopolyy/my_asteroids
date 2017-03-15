#include "statechanginglevel.h"
#include "utility.h"
#include "playersinput.h"
#include "resouceholder.h"

#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/View.hpp>

ChangingLevelState::ChangingLevelState(StateStack &stack,
                                       Context context)
    : State         (stack, context)
    , mText         ()
    , mElapsedTime  (sf::Time::Zero)
    , mPlayer       (*(context.player))
{
    sf::Font& font = context.fonts->get(Fonts::Main);
    sf::Vector2f winSize(context.window->getSize());

    mText.setFont(font);
    std::string message;
    if (context.player->getMissionStatus() ==
                                        PlayersInput::MissionFailue) {

        message = "Mission failed!";
    }
    else {
        message = "CONGRATULATIONS\n";
        if (context.player->getMissionStatus() ==
                                        PlayersInput::MissionRunning) {
            message += "next level "
                    + std::to_string(mPlayer.getLevel());
        }
        else {
            message += "Mission successful!";
        }
    }
    mText.setString(message);

    mText.setCharacterSize(70);
    centerOrigin(mText);
    mText.setPosition(0.5f * winSize.x, 0.4f * winSize.y);
}

void ChangingLevelState::draw()
{
    sf::RenderWindow& window = *getContext().window;
    window.setView(window.getDefaultView());

    sf::RectangleShape backgroundShape;
    backgroundShape.setFillColor(sf::Color(0, 0, 0, 150));
    backgroundShape.setSize(window.getView().getSize());

    window.draw(backgroundShape);
    window.draw(mText);
}

bool ChangingLevelState::update(sf::Time dt)
{
    mElapsedTime += dt;
    if (mElapsedTime > sf::seconds(3)) {

        requestStackClear();

        if (mPlayer.getMissionStatus() !=
                                        PlayersInput::MissionRunning) {
            requestStackClear();
            requestStackPush(States::Menu);
        }
        else {
            requestStackClear();
            requestStackPush(States::Game);
        }
    }
    return false;
}

bool ChangingLevelState::handleEvent(const sf::Event &event)
{
    return false;
}
