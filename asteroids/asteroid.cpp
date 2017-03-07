#include "asteroid.h"
#include "resouceholder.h"
#include "utility.h"

#include <SFML/Graphics/RenderTarget.hpp>

#include <QDebug>

Asteroid::Asteroid(Type type, const TextureHolder &textures)
    : Entity(20)
    , mType(type)
    , mSprite(textures.get(Textures::RockAsteroid),
              sf::IntRect(0, 202, 100, 82))
    , mSpritePosition(75)
    , mRotateTime(sf::seconds(0.05f))
    , mTextures(textures)
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
    //return getWorldTransform().transformRect(mSprite.getGlobalBounds());
}

float Asteroid::getMaxSpeed() const
{
    return 100;
}

int Asteroid::getDamage() const
{
    return 20;
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
