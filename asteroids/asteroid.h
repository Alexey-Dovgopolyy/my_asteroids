#ifndef ASTEROID_H
#define ASTEROID_H

#include "entity.h"
#include "resourceidentifiers.h"
#include "datatable.h"

#include <SFML/Graphics/Sprite.hpp>

#include <map>

namespace {
    const std::vector<AsteroidData> Table = initializeAsteroidData();
}

class Asteroid : public Entity
{
public:
    enum Type {
        Rock,
        Ice,
        TypeCount
    };

    enum Size {
        Solid ,
        Wreck ,
        SizeCount
    };

public:
    Asteroid(Type type, Size size, int speed, const TextureHolder& textures);
    virtual unsigned int    getCategory() const;
    virtual sf::FloatRect   getBoundingRect() const;
    float                   getMaxSpeed() const;
    int                     getDamage() const;

private:
    virtual void updateCurrent(sf::Time dt, CommandQueue &commands);
    virtual void drawCurrent(sf::RenderTarget &target,
                                                sf::RenderStates states) const;

    void initializeTexturesMap();

private:
    Type            mType;
    Size            mSize;
    sf::Sprite      mSprite;
    short           mSpritePosition;
    sf::Time        mRotateTime;
    const TextureHolder&  mTextures;
    sf::FloatRect   mBoundingRect;

    int             mMaxSpeed;
    int             mDamage;

    std::map<short, sf::IntRect> mTexturesMap;
};

#endif // ASTEROID_H
