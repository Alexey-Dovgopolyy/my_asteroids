#ifndef SETTINGSTATE_H
#define SETTINGSTATE_H

#include "state.h"
#include "playersinput.h"
#include "container.h"
#include "button.h"
#include "label.h"

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Text.hpp>

class SettingState : public State
{
public:
    SettingState(StateStack& stack, Context context);

    virtual void draw();
    virtual bool update(sf::Time dt);
    virtual bool handleEvent(const sf::Event &event);

private:
    void updateLabels();
    void addButtonLabel(PlayersInput::Action action,
                        float y, const std::string& text,
                        Context context);

private:
    sf::Sprite              mBackgroundSprite;
    GUI::Container          mGUIContainer;
    std::array<GUI::Button::Ptr, PlayersInput::ActionCount> mBindingButtons;
    std::array<GUI::Label::Ptr, PlayersInput::ActionCount> mBindingLabels;

};

#endif // SETTINGSTATE_H
