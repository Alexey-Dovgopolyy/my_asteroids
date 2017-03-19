#include "settingstate.h"
#include "resouceholder.h"

#include <SFML/Graphics/RenderWindow.hpp>

SettingState::SettingState(StateStack &stack, Context context)
    : State(stack, context)
    , mGUIContainer(context.window)
{
    mBackgroundSprite.setTexture(context.textures->get(Textures::TitleScreen));

    addButtonLabel(PlayersInput::MoveUp,    200.f, "Move Up", context);
    addButtonLabel(PlayersInput::MoveDown,  250.f, "MoveDown", context);
    addButtonLabel(PlayersInput::MoveLeft,  300.f, "MoveLeft", context);
    addButtonLabel(PlayersInput::MoveRight, 350.f, "MoveRight", context);
    addButtonLabel(PlayersInput::Fire,      400.f, "Fire", context);

    auto backButton = std::make_shared<GUI::Button>(context);
    backButton->setText("Back");
    backButton->setPosition(100.f, 500.f);
    backButton->setCallback([this] () {

        requestStackPop();
        requestStackPush(States::Menu);
    });

    mGUIContainer.pack(backButton);
}

void SettingState::draw()
{
    sf::RenderWindow* window = getContext().window;

    window->draw(mBackgroundSprite);
    window->draw(mGUIContainer);
}

bool SettingState::update(sf::Time dt)
{
    mGUIContainer.handleRealtimeInput();
    return true;
}

bool SettingState::handleEvent(const sf::Event &event)
{
    mGUIContainer.handleEvent(event);
    return true;
}


void SettingState::updateLabels()
{

}

void SettingState::addButtonLabel(PlayersInput::Action action,
                    float y, const std::string& text,
                    Context context)
{
    mBindingButtons[action] = std::make_shared<GUI::Button>(context);
    mBindingButtons[action]->setText(text);
    mBindingButtons[action]->setPosition(100.f, y);
    mBindingButtons[action]->setToggle(true);

    mBindingLabels[action] = std::make_shared<GUI::Label>(text, *context.fonts);
    mBindingLabels[action]->setPosition(320.f, y + 15.f);

    mGUIContainer.pack(mBindingButtons[action]);
    mGUIContainer.pack(mBindingLabels[action]);
}
