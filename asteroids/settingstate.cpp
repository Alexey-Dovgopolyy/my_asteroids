#include "settingstate.h"
#include "resouceholder.h"
#include "utility.h"

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
    //addButtonLabel(PlayersInput::Fire,      400.f, "Fire", context);

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
    updateLabels();
    mGUIContainer.handleRealtimeInput();
    return true;
}

bool SettingState::handleEvent(const sf::Event &event)
{
    bool isKeyBinding = false;

    for (std::size_t action = 0; action < PlayersInput::ActionCount - 1; ++action) {

        if (mBindingButtons[action]->isActive()) {
            isKeyBinding = true;
            if (event.type == sf::Event::KeyReleased) {
                getContext().player->assignKey(static_cast<PlayersInput::Action>(action), event.key.code);
                mBindingButtons[action]->deactivate();
            }
            break;
        }
    }

    mGUIContainer.handleEvent(event);
    return false;
}


void SettingState::updateLabels()
{
    PlayersInput& player = *getContext().player;

    for (std::size_t i = 0; i < PlayersInput::ActionCount - 1; ++i) {

        sf::Keyboard::Key key =
           player.getAssignedKey(static_cast<PlayersInput::Action>(i));
        mBindingLabels[i]->setText(toString(key));
    }
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
