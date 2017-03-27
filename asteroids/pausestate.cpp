#include "pausestate.h"
#include "button.h"
#include "utility.h"
#include "resouceholder.h"

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/RectangleShape.hpp>

#include <memory>

PauseState::PauseState(StateStack &stack, Context context)
    : State(stack, context)
    , mGUIContainer(context.window)
{

    auto continueButton = std::make_shared<GUI::Button>(context);
    //continueButton->setPosition(100, 100);
    continueButton->setText("Continue");
    continueButton->setCallback([this]() {
        requestStackPop();
    });

    auto menuButton = std::make_shared<GUI::Button>(context);
    //menuButton->setPosition(100, 150);
    menuButton->setText("Menu");
    menuButton->setCallback([this] () {
        requestStackClear();
        requestStackPush(States::Menu);
    });

    float buttonXpos = context.window->getSize().x / 2.f -
            menuButton->getRect().width / 2.f;

    continueButton->setPosition(buttonXpos, 350);
    menuButton->setPosition(buttonXpos, 400);

    mGUIContainer.pack(continueButton);
    mGUIContainer.pack(menuButton);

    mPauseText.setString("Pause");
    mPauseText.setFont(context.fonts->get(Fonts::Main));
    mPauseText.setCharacterSize(70);
    centerOrigin(mPauseText);
    mPauseText.setPosition(context.window->getSize().x / 2.f, 200);
}

void PauseState::draw()
{
    sf::RenderWindow& window = *(getContext().window);

    sf::RectangleShape backgroundShape;
    backgroundShape.setFillColor(sf::Color(0, 0, 50, 150));
    backgroundShape.setSize(window.getView().getSize());

    window.draw(backgroundShape);
    window.draw(mPauseText);
    window.draw(mGUIContainer);
}

bool PauseState::update(sf::Time dt)
{
    mGUIContainer.handleRealtimeInput();
    return false;
}

bool PauseState::handleEvent(const sf::Event &event)
{
    if (event.type == sf::Event::KeyReleased) {

        if (event.key.code == sf::Keyboard::Escape) {
            requestStackPop();
        }
    }

    mGUIContainer.handleEvent(event);
    return true;
}
