#ifndef AIRCRAFT_H
#define AIRCRAFT_H

#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include "SFML/Graphics/RenderTarget.hpp"

#include "entity.h"
#include "command.h"
#include "resouceholder.h"
#include "resourceidentifiers.h"
#include "utility.h"

class Aircraft : public Entity
{
public:
    enum Type {
        Eagle,
        TypeCount
    };

public:
    Aircraft(Type type, const TextureHolder& textures);

    virtual unsigned int getCategory() const;

private:
    virtual void drawCurrent(sf::RenderTarget& target,
                             sf::RenderStates states) const;
    virtual void updateCurrent(sf::Time dt, CommandQueue& commands);

private:
    Type                    mType;
    sf::Sprite				mSprite;
    Command                 mFireCommand;
    sf::Time                mFireCountdown;

    bool                    mIsFiring;

    int                     mFireRateLevel;
    double                  mDirection;
};

#endif // AIRCRAFT_H
