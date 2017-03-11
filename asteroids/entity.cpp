#include "entity.h"
#include <QDebug>
#include <cassert>

Entity::Entity(int hitpoints)
    : mVelocity()
    , mHitpoints(hitpoints)
{

}

void Entity::setVelocity(sf::Vector2f velocity)
{
    mVelocity = velocity;
}

void Entity::setVelocity(float vx, float vy)
{
    mVelocity.x = vx;
    mVelocity.y = vy;
}

void Entity::accelerate(sf::Vector2f velocity)
{
    mVelocity += velocity;
    //qDebug() << "accel" << mVelocity.x << " " << mVelocity.y;
}

void Entity::accelerate(float vx, float vy)
{
    mVelocity.x += vx;
    mVelocity.y += vy;
}

sf::Vector2f Entity::getVelocity()
{
    return mVelocity;
}

int Entity::getHitpoints() const
{
    return mHitpoints;
}

void Entity::repair(int points)
{
    assert(points > 0);
    mHitpoints += points;
}

void Entity::damage(int points)
{
    assert(points > 0);
    mHitpoints -= points;
}

void Entity::destroy()
{
    mHitpoints = 0;
}

void Entity::adapt(sf::FloatRect worldBounds)
{
    sf::Vector2f pos = getPosition();

    if (pos.x > worldBounds.width) pos.x = 0;
    if (pos.x < 0) pos.x = worldBounds.width;
    if (pos.y > worldBounds.height) pos.y = 0;
    if (pos.y < 0) pos.y = worldBounds.height;

    setPosition(pos.x, pos.y);
}

void Entity::remove()
{
    destroy();
}

bool Entity::isDestroyed() const
{
    return mHitpoints <= 0;
}

void Entity::setRect(sf::FloatRect rect)
{
    mBoundingRect = rect;
    correctBoundingRect();
}

sf::FloatRect Entity::getRect() const
{
    return mBoundingRect;
}

void Entity::updateCurrent(sf::Time dt, CommandQueue&)
{
    //qDebug() << mVelocity.x << " " << mVelocity.y;
    move(mVelocity * dt.asSeconds());
}

void Entity::correctBoundingRect()
{
    float difference = std::abs(mBoundingRect.width - mBoundingRect.height);
    if (mBoundingRect.width > mBoundingRect.height) {
        mBoundingRect.left += difference / 2;
        mBoundingRect.width -= difference;
    }
    else {
        mBoundingRect.top += difference / 2;
        mBoundingRect.height -= difference;
    }
}
