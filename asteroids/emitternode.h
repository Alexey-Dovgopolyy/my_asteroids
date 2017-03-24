#ifndef EMITTERNODE_H
#define EMITTERNODE_H

#include "scenenode.h"
#include "particle.h"

class ParticleNode;

class EmitterNode : public SceneNode
{
public:
    EmitterNode(Particle::Type type, sf::Vector2f velocity);

    void            setIsDestroyed(bool flag);

private:
    virtual void    updateCurrent(sf::Time dt, CommandQueue &commands);
    void            emitParticles(sf::Time dt);


private:
    sf::Time            mAccumulatedTime;
    Particle::Type      mType;
    ParticleNode*       mParticleSystem;
    sf::Vector2f        mAsteroidVelocity;
    bool                mIsDestroyed;

};

#endif // EMITTERNODE_H
