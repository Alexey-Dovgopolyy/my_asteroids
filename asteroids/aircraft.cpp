#include "aircraft.h"
#include "commandqueue.h"

#include <cmath>
#include <QDebug>

Aircraft::Aircraft(Type type, const TextureHolder &textures)
    : Entity(100)
    , mType(type)
    , mSprite(textures.get(Textures::Entities),
              sf::IntRect(0, 0, 48, 64))
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
    return getWorldTransform().transformRect
                                    (mSprite.getGlobalBounds());
}

void Aircraft::setDirection(float angle)
{
    mDirection = angle;
    //mSprite.setRotation(mDirection);
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
    target.draw(mSprite, states);
}

void Aircraft::updateCurrent(sf::Time dt, CommandQueue& commands)
{
    //qDebug() << "global" << mSprite.getGlobalBounds().left << " " << mSprite.getGlobalBounds().top;
    checkProjectileLaunch(dt, commands);
    setRotation(mDirection);
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
    std::unique_ptr<Projectile> projectile
                                    (new Projectile(type,textures));
    sf::Vector2f offset(xOffset * mSprite.getGlobalBounds().width,
                        yOffset * mSprite.getGlobalBounds().height);

    float maxSpeed = projectile->getMaxSpeed();
    float vx = maxSpeed * std::cos(toRadian(mDirection - 90));
    float vy = maxSpeed * std::sin(toRadian(mDirection - 90));
    sf::Vector2f velocity(vx, vy);

    offset.x = offset.x * std::cos(toRadian(mDirection));
    offset.y = offset.x * std::tan(toRadian(mDirection));

    qDebug() << offset.x << " " << offset.y;

    sf::Vector2f worldPos = (getWorldPosition() + offset);

    projectile->setRotation(mDirection);
    projectile->setPosition(worldPos);
    projectile->setVelocity(velocity);
    node.attachChild(std::move(projectile));
}
