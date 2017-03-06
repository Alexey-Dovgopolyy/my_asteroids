#ifndef PROJECTILE_H
#define PROJECTILE_H

#include "entity.h"
#include "resourceidentifiers.h"

#include <SFML/Graphics/Sprite.hpp>

class Projectile : public Entity
{
public:
    enum Type {
        AlliedBullet,
        TypeCount
    };

public:
    Projectile(Type type, const TextureHolder& textures);

    virtual unsigned int    getCategory() const;
    virtual sf::FloatRect   getBoundingRect() const;
    float                   getMaxSpeed() const;
    int                     getDamage() const;

private:
    virtual void updateCurrent(sf::Time dt, CommandQueue& commands);
    virtual void drawCurrent(sf::RenderTarget& target,
                             sf::RenderStates states) const;

private:
    Type            mType;
    sf::Sprite      mSprite;
    sf::Vector2f    mTargetDirection;
};

#endif // PROJECTILE_H
