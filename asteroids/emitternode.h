#ifndef EMITTERNODE_H
#define EMITTERNODE_H

#include "scenenode.h"
#include "particle.h"

class ParticleNode;

class EmitterNode : public SceneNode
{
public:
    EmitterNode(Particle::Type type);

private:
    virtual void    updateCurrent(sf::Time dt, CommandQueue &commands);
    void            emitParticles(sf::Time dt);

private:
    sf::Time            mAccumulatedTime;
    Particle::Type      mType;
    ParticleNode*       mParticleSystem;

};

#endif // EMITTERNODE_H
