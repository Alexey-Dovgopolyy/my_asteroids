#include "aircraft.h"
#include <cmath>
#include <QDebug>

Aircraft::Aircraft(Type type, const TextureHolder &textures)
    : Entity(100)
    , mType(type)
    , mSprite(textures.get(Textures::Eagle))
    , mFireCommand()
    , mFireCountdown(sf::Time::Zero)
    , mIsFiring(false)
    , mFireRateLevel(1)
    , mDirection(0.f)
{
    centerOrigin(mSprite);

    mFireCommand.category = Category::SceneAirLayer;
    mFireCommand.action = [this, &textures] (SceneNode& node,
                                            sf::Time)
    {

    };
}

unsigned int Aircraft::getCategory() const
{
    //qDebug() << Category::PlayerAircraft;
    return Category::PlayerAircraft;
}

void Aircraft::drawCurrent(sf::RenderTarget& target,
                         sf::RenderStates states) const
{
    target.draw(mSprite, states);
}

void Aircraft::updateCurrent(sf::Time dt, CommandQueue& commands)
{
    Entity::updateCurrent(dt, commands);
}
