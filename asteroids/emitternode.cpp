#include "emitternode.h"
#include "particlenode.h"
#include "command.h"
#include "commandqueue.h"
#include <QDebug>

EmitterNode::EmitterNode(Particle::Type type, sf::Vector2f velocity)
    : SceneNode()
    , mAccumulatedTime(sf::Time::Zero)
    , mType(type)
    , mParticleSystem(nullptr)
    , mAsteroidVelocity(velocity)
    , mIsDestroyed(false)
{
}

void EmitterNode::updateCurrent(sf::Time dt, CommandQueue &commands)
{
    if(mIsDestroyed) {
        return;
    }

    if (mParticleSystem) {
        emitParticles(dt);
    }
    else {
        auto finder = [this] (ParticleNode& container, sf::Time) {
            if (container.getParticleType() == mType) {
                mParticleSystem = &container;
            }
        };

        Command command;
        command.category = Category::ParticleSystem;
        command.action = derivedAction<ParticleNode>(finder);

        commands.push(command);
    }
}

void EmitterNode::emitParticles(sf::Time dt)
{
    const float emissionRate = 30.f;
    const sf::Time interval = sf::seconds(1.f) / emissionRate;

    mAccumulatedTime += dt;

    sf::Vector2f particleVelocity;
    particleVelocity.x = 40.f;
    particleVelocity.y = (mAsteroidVelocity.x * particleVelocity.x) / mAsteroidVelocity.y;

    while (mAccumulatedTime > interval) {
        mAccumulatedTime -= interval;
        qDebug() << "ParticlePos" << getWorldPosition().x << " " << getWorldPosition().y;
        mParticleSystem->addParticle(getWorldPosition(), particleVelocity);
    }
}

void EmitterNode::setIsDestroyed(bool flag)
{
    mIsDestroyed = flag;
}
