#ifndef BUTTON_H
#define BUTTON_H

#include "component.h"
#include "resourceidentifiers.h"
#include "state.h"

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Text.hpp>

#include <vector>
#include <string>
#include <memory>
#include <functional>

namespace GUI {

class Button : public Component
{
public:
    typedef std::shared_ptr<Button>     Ptr;
    typedef std::function<void()>       Callback;

    enum Type {

        Normal,
        Selected,
        Pressed,
        ButtonCount
    };

public:
                        Button(State::Context context);

    void                setCallback(Callback callback);
    void                setText(const std::string& text);
    void                setToggle(bool flag);

    virtual bool        isSelectable() const;
    virtual void        select();
    virtual void        deselect();

    virtual void        activate();
    virtual void        deactivate();

    virtual void        handleEvent(const sf::Event& event);

    virtual sf::FloatRect getRect() const;

private:
    virtual void        draw(sf::RenderTarget& target,
                            sf::RenderStates states) const;
    void                changeTexture(Type buttonType);

private:
    Callback        mCallback;
    sf::Sprite      mSprite;
    sf::Text        mText;
    bool            mIsToggle;
};

}

#endif // BUTTON_H
