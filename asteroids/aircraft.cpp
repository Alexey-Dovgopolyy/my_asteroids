#include "aircraft.h"
#include "commandqueue.h"
#include "datatable.h"

#include <cmath>
#include <QDebug>
#include <vector>

namespace {
    const std::vector<AircraftData> Table = initializeAircraftData();
}

Aircraft::Aircraft(Type type, const TextureHolder &textures)
    : Entity(Table[type].hitpoints)
    , mType(type)
    , mSprite(textures.get(Table[type].texture),
              Table[type].textureRect)
    , mFireCommand()
    , mFireCountdown(sf::Time::Zero)
    , mExplosion(textures.get(Textures::Explosion))
    , mIsFiring(false)
    , mShowExplosion(true)
    , mFireRateLevel(1)
    , mDirection(0.f)
{
    mExplosion.setFrameSize(sf::Vector2i(256, 256));
    mExplosion.setNumFrames(16);
    mExplosion.setDuration(sf::seconds(1));

    centerOrigin(mSprite);
    centerOrigin(mExplosion);

    setRect(mSprite.getGlobalBounds());

    mFireCommand.category = Category::SceneAirLayer;
    mFireCommand.action = [this, &textures] (SceneNode& node,
                                            sf::Time)
    {
        createBullet(node, textures);
    };

}

unsigned int Aircraft::getCategory() const
{
    //qDebug() << Category::PlayerAircraft;
    return Category::PlayerAircraft;
}

sf::FloatRect Aircraft::getBoundingRect() const
{
    //qDebug() << mBoundingRect.width << " " << mBoundingRect.height;
    return getWorldTransform().transformRect(getRect());
    //return getWorldTransform().transformRect(mSprite.getGlobalBounds());
}

void Aircraft::remove()
{
    Entity::remove();
    //mShowExplosion = false;
}

bool Aircraft::isMarkedForRemoval() const
{
    return isDestroyed() &&
                        (mExplosion.isFinished() || !mShowExplosion);
}

void Aircraft::setDirection(float angle)
{
    mDirection = angle;
    mSprite.setRotation(mDirection);
}

float Aircraft::getDirection() const
{
    return mDirection;
}

void Aircraft::fire()
{
    mIsFiring = true;
}

void Aircraft::drawCurrent(sf::RenderTarget& target,
                         sf::RenderStates states) const
{
    if (isDestroyed() && mShowExplosion) {
        target.draw(mExplosion, states);
    }
    else {
        target.draw(mSprite, states);
    }
}

void Aircraft::updateCurrent(sf::Time dt, CommandQueue& commands)
{    
    if (isDestroyed()) {
        mExplosion.update(dt);
        return;
    }

    checkProjectileLaunch(dt, commands);
    Entity::updateCurrent(dt, commands);    
}

void Aircraft::checkProjectileLaunch(sf::Time dt,
                                        CommandQueue& commands)
{
    if (mIsFiring && mFireCountdown <= sf::Time::Zero) {

        commands.push(mFireCommand);
        mFireCountdown += sf::seconds(0.5f);
        mIsFiring = false;
    }
    else if (mFireCountdown > sf::Time::Zero) {
        mFireCountdown -= dt;
        mIsFiring = false;
    }
}

void Aircraft::createBullet(SceneNode& node,
                                    const TextureHolder& textures)
{
    Projectile::Type type = Projectile::AlliedBullet;

    createProjectile(node, type, 0.4f, 0.0f, textures);
    createProjectile(node, type, -0.4f, 0.0f, textures);
}

void Aircraft::createProjectile(SceneNode& node,
                                Projectile::Type type,
                                float xOffset, float yOffset,
                                const TextureHolder& textures)
{
    std::unique_ptr<Projectile> projectile(new Projectile(type,textures));
    sf::Vector2f offset(xOffset * mSprite.getGlobalBounds().width,
                        yOffset * mSprite.getGlobalBounds().height);

    float maxSpeed = projectile->getMaxSpeed();
    float vx = maxSpeed * std::cos(toRadian(mDirection - 90));
    float vy = maxSpeed * std::sin(toRadian(mDirection - 90));
    sf::Vector2f velocity(vx, vy);

    offset.x = offset.x * std::cos(toRadian(mDirection));
    offset.y = offset.x * std::tan(toRadian(mDirection));

    //qDebug() << offset.x << " " << offset.y;

    sf::Vector2f worldPos = (getWorldPosition() + offset);

    projectile->setRotation(mDirection);
    projectile->setPosition(worldPos);
    projectile->setVelocity(velocity);
    node.attachChild(std::move(projectile));
}

//void Aircraft::correctBoundingRect()
//{
//    float difference = std::abs(mBoundingRect.width - mBoundingRect.height);
//    if (mBoundingRect.width > mBoundingRect.height) {
//        mBoundingRect.left -= difference / 2;
//        mBoundingRect.width -= difference;
//    }
//    else {
//        mBoundingRect.top += difference / 2;
//        mBoundingRect.height -= difference;
//    }
//}
