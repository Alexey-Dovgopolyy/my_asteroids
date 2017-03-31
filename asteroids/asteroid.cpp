#include "asteroid.h"
#include "resouceholder.h"
#include "utility.h"
#include "soundnode.h"

#include <SFML/Graphics/RenderTarget.hpp>

#include <QDebug>

Asteroid::Asteroid(Type type, Size size, int speed, const TextureHolder &textures)
    : Entity(size == Solid ? Table[type].hitpoints : Table[type].hitpoints / 2)
    , mType(type)
    , mSize(size)
    , mSprite(textures.get(Table[type].texture),
              Table[type].textureRect)
    , mSpritePosition(Table[type].spritePosition)
    , mRotateTime(Table[type].rotateTime)
    , mTimeLastUpdate(sf::Time::Zero)
    , mTextures(textures)
    , mExplosion(textures.get(Textures::Explosion))
    , mShowExplosion(true)
    , mPlayedExplosionSound(false)
    , mMaxSpeed(Table[type].speed[speed])
    , mDamage(Table[type].damage)
    , mTail(nullptr)
{
    mExplosion.setFrameSize(sf::Vector2i(256, 256));
    mExplosion.setNumFrames(16);
    mExplosion.setDuration(sf::seconds(1));


    if (mSize == Wreck) {
        mSprite.setScale(0.5f, 0.5f);
        mExplosion.setScale(0.5f, 0.5f);
    }
    centerOrigin(mSprite);
    centerOrigin(mExplosion);
    setRect(mSprite.getGlobalBounds());

    if (type == Fire) {
        std::unique_ptr<EmitterNode> tail(new EmitterNode(Particle::Tail, getVelocity()));
        tail->setPosition(getPosition().x, getPosition().y);
        mTail = tail.get();
        attachChild(std::move(tail));
    }
}

unsigned int Asteroid::getCategory() const
{
    if (mType == Rock)
        return Category::RockAsteroid;
    else if (mType == Ice)
        return Category::IceAsteroid;
    else if (mType == Fire)
        return Category::FireAsteroid;

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

unsigned int Asteroid::getSize() const
{
    return mSize;
}



void Asteroid::playLocalSound(CommandQueue& commands,
                                    SoundEffect::ID effect)
{
    sf::Vector2f worldPosition = getWorldPosition();

    Command command;
    command.category = Category::SoundEffect;
    command.action = derivedAction<SoundNode>(
        [effect, worldPosition] (SoundNode& node, sf::Time)
        {
            node.playSound(effect, worldPosition);
        });

    commands.push(command);
}

void Asteroid::remove()
{
    Entity::remove();

}

bool Asteroid::isMarkedForRemoval() const
{
    return isDestroyed() &&
                    (mExplosion.isFinished() || !mShowExplosion);
}

void Asteroid::updateCurrent(sf::Time dt, CommandQueue &commands)
{
    if (isDestroyed()) {
        mExplosion.update(dt);
        if (mTail) {
            mTail->setIsDestroyed(true);
        }

        if (!mPlayedExplosionSound)
        {
            SoundEffect::ID soundEffect = (randomInt(2) == 0) ? SoundEffect::Explosion1 : SoundEffect::Explosion2;
            playLocalSound(commands, soundEffect);

            mPlayedExplosionSound = true;
        }
        return;
    }

    if (mTimeLastUpdate > mRotateTime) {
        mTimeLastUpdate -= mRotateTime;

        float angle = mSprite.getRotation();
        angle += 360 / mSpritePosition;
        mSprite.setRotation(angle);

    }
    else {
        mTimeLastUpdate += dt;
    }

    Entity::updateCurrent(dt, commands);
}

void Asteroid::drawCurrent(sf::RenderTarget &target,
                                                sf::RenderStates states) const
{
    if (isDestroyed() && mShowExplosion) {
        target.draw(mExplosion, states);
    }
    else {
        target.draw(mSprite, states);
    }
}

//void Asteroid::initializeTexturesMap()
//{
//    mTexturesMap[0] = sf::IntRect(0, 175, 90, 90);
//    mTexturesMap[1] = sf::IntRect(80, 175, 90, 90);
//    mTexturesMap[2] = sf::IntRect(160, 175, 90, 90);
//    mTexturesMap[3] = sf::IntRect(240, 175, 90, 90);
//    mTexturesMap[4] = sf::IntRect(300, 175, 90, 90);
//    mTexturesMap[5] = sf::IntRect(380, 175, 90, 90);
//    mTexturesMap[6] = sf::IntRect(460, 175, 90, 90);
//    mTexturesMap[7] = sf::IntRect(540, 175, 90, 90);
//}
