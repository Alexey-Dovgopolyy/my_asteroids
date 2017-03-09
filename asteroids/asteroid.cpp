#include "asteroid.h"
#include "resouceholder.h"
#include "utility.h"

#include <SFML/Graphics/RenderTarget.hpp>

#include <QDebug>

Asteroid::Asteroid(Type type, Size size, int speed, const TextureHolder &textures)
    : Entity(Table[type].hitpoints)
    , mType(type)
    , mSize(size)
    , mSprite(textures.get(Table[type].texture),
              Table[type].textureRect)
    , mSpritePosition(Table[type].spritePosition)
    , mRotateTime(Table[type].rotateTime)
    , mTextures(textures)
    , mMaxSpeed(Table[type].speed[speed])
    , mDamage(Table[type].damage)
{
    //mSprite.setScale(0.5f, 0.5f);
    centerOrigin(mSprite);
    setRect(mSprite.getGlobalBounds());
}

unsigned int Asteroid::getCategory() const
{
    return Category::RockAsteroid;
}

sf::FloatRect Asteroid::getBoundingRect() const
{
    return getWorldTransform().transformRect(getRect());
}

float Asteroid::getMaxSpeed() const
{
    return mMaxSpeed;
}

int Asteroid::getDamage() const
{
    return mDamage;
}

void Asteroid::updateCurrent(sf::Time dt, CommandQueue &commands)
{
    static sf::Time time = sf::Time::Zero;
    time += dt;
    if (time > mRotateTime) {
        time -= mRotateTime;

        float angle = mSprite.getRotation();
        angle += 360 / mSpritePosition;
        mSprite.setRotation(angle);
    }

    Entity::updateCurrent(dt, commands);
}

void Asteroid::drawCurrent(sf::RenderTarget &target,
                                                sf::RenderStates states) const
{
    target.draw(mSprite, states);
}

void Asteroid::initializeTexturesMap()
{
    mTexturesMap[0] = sf::IntRect(0, 175, 90, 90);
    mTexturesMap[1] = sf::IntRect(80, 175, 90, 90);
    mTexturesMap[2] = sf::IntRect(160, 175, 90, 90);
    mTexturesMap[3] = sf::IntRect(240, 175, 90, 90);
    mTexturesMap[4] = sf::IntRect(300, 175, 90, 90);
    mTexturesMap[5] = sf::IntRect(380, 175, 90, 90);
    mTexturesMap[6] = sf::IntRect(460, 175, 90, 90);
    mTexturesMap[7] = sf::IntRect(540, 175, 90, 90);
}
