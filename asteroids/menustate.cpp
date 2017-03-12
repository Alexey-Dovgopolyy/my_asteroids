#include "menustate.h"
#include "button.h"
#include "state.h"
#include "utility.h"
#include "resouceholder.h"

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/View.hpp>

MenuState::MenuState(StateStack &stack, Context context)
    : State(stack, context)
    , mGUIContainer()
{
    sf::Texture& texture = context.textures->get(Textures::TitleScreen);
    mBackgroundSprite.setTexture(texture);

    auto playButton = std::make_shared<GUI::Button>(context);
    playButton->setPosition(100, 300);
    playButton->setText("Play");
    playButton->setCallback([this]  ()
    {
        requestStackPop();
        requestStackPush(States::Game);
    });

    auto exitButton = std::make_shared<GUI::Button>(context);
    exitButton->setPosition(100, 350);
    exitButton->setText("Exit");
    exitButton->setCallback([this]  ()
    {
        requestStackPop();
    });

    mGUIContainer.pack(playButton);
    mGUIContainer.pack(exitButton);
}

void MenuState::draw()
{
    sf::RenderTarget& window = *getContext().window;

    window.setView(window.getDefaultView());

    window.draw(mBackgroundSprite);
    window.draw(mGUIContainer);
}

bool MenuState::update(sf::Time dt)
{
    return true;
}

bool MenuState::handleEvent(const sf::Event& event)
{
    mGUIContainer.handleEvent(event);
    return false;
}
