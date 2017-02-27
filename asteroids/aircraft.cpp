#include "aircraft.h"
#include <cmath>
#include <QDebug>

Aircraft::Aircraft()
    : mDirection        (0.0)
    , mCoords           ()
    , mVelocity         ()
    , mAcceleration     ()
{
    if (!mTexture.loadFromFile("Media/Textures/Eagle.png")) {
        throw std::runtime_error("ResourceHolder::load - Failed to load Eagle");
    }
    mSprite.setTexture(mTexture);
    centerOrigin(mSprite);
}

void Aircraft::setCoordinates(float x, float y)
{
    mCoords.x = x;
    mCoords.y = y;
    mSprite.setPosition(x, y);
}

sf::Vector2f Aircraft::getCoordinates()
{
    return mCoords;
}

void Aircraft::updateDirection(sf::Vector2i mousePos)
{
    float dx = mousePos.x - mCoords.x;
    float dy = mCoords.y - mousePos.y;

    float angle = std::atan2(dx, dy);
    float degree = toDegree(angle);
    //qDebug() << degree;

    mSprite.setRotation(degree);
}

void Aircraft::updateCoordinates(float x, float y)
{
    //qDebug() << x << " " << y << fabs(x);
    if (fabs(x) < 0.05f) {
        mVelocity.x > 0 ? mVelocity.x -= 0.05f : mVelocity.x += 0.05f;
        if (fabs(mVelocity.x) <= 0.05f) {
            mVelocity.x = 0;
        }
        //qDebug() << mVelocity.x;
    }
    else {
        mVelocity.x += x;

    }

    if (fabs(y) < 0.05f) {
        mVelocity.y > 0 ? mVelocity.y -= 0.05f : mVelocity.y += 0.05f;
        if (fabs(mVelocity.y) <= 0.05f) {
            mVelocity.y = 0;
        }
    }
    else {
        mVelocity.y += y;
    }

    mVelocity.x = std::min(10.f, mVelocity.x);
    mVelocity.x = std::max(-10.f, mVelocity.x);
    mVelocity.y = std::min(10.f, mVelocity.y);
    mVelocity.y = std::max(-10.f, mVelocity.y);

    qDebug() << mVelocity.x << " " << mVelocity.y;
    float radians = std::atan2(mVelocity.x, mVelocity.y);
    //qDebug() << toDegree(radians);

    x = abs(mVelocity.x) * std::sin(radians);
    y = abs(mVelocity.y) * std::cos(radians);

    //qDebug() << x << " " << y;
    mCoords.x += x;
    mCoords.y += y;
    mSprite.setPosition(mCoords.x, mCoords.y);
}

void Aircraft::setAcceleration(float x, float y)
{
    if (x)
        mAcceleration.x = x;
    if (y)
        mAcceleration.y = y;
}

void Aircraft::updateCoordsByAcceleration(sf::Time dt)
{
    static sf::Time updateInterval;
    updateInterval += dt;

    if (updateInterval > sf::seconds(0.1f)) {
        mAcceleration.x /= 2;
        mAcceleration.y /= 2;
        updateCoordinates(mAcceleration.x, mAcceleration.y);
        updateInterval -= sf::seconds(0.1f);
    }
}

void Aircraft::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
    //qDebug() << mSprite.getPosition().x << " " << mSprite.getPosition().y;
    //qDebug() << mAcceleration.x << " " << mAcceleration.y;
    target.draw(mSprite, states);
}

