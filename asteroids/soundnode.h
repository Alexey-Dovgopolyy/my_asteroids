#ifndef SOUNDNODE_H
#define SOUNDNODE_H

#include "scenenode.h"
#include "resourceidentifiers.h"

class SoundPlayer;

class SoundNode : public SceneNode
{
public:
    SoundNode(SoundPlayer& player);

    void playSound(SoundEffect::ID sound, sf::Vector2f position);

    virtual unsigned int getCategory() const;

private:
    SoundPlayer& mSounds;
};

#endif // SOUNDNODE_H
