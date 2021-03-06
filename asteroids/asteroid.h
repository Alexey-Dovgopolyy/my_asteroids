#ifndef ASTEROID_H
#define ASTEROID_H

#include "entity.h"
#include "resourceidentifiers.h"
#include "datatable.h"
#include "animation.h"
#include "emitternode.h"
#include "commandqueue.h"

#include <SFML/Graphics/Sprite.hpp>

#include <map>

namespace {
    const std::vector<AsteroidData> Table = initializeAsteroidData();
}

class Asteroid : public Entity
{
public:
    enum Type {
        Rock,
        Ice,
        Fire,
        TypeCount
    };

    enum Size {
        Solid ,
        Wreck ,
        SizeCount
    };

public:
    Asteroid(Type type, Size size, int speed, const TextureHolder& textures);
    virtual unsigned int    getCategory() const;
    virtual sf::FloatRect   getBoundingRect() const;
    float                   getMaxSpeed() const;
    int                     getDamage() const;
    unsigned int            getSize() const;
    void                    playLocalSound(CommandQueue& commands,
                                           SoundEffect::ID effect);

    virtual void            remove();
    virtual bool            isMarkedForRemoval() const;

private:
    virtual void updateCurrent(sf::Time dt, CommandQueue &commands);
    virtual void drawCurrent(sf::RenderTarget &target,
                                                sf::RenderStates states) const;

    //void initializeTexturesMap();

private:
    Type            mType;
    Size            mSize;
    sf::Sprite      mSprite;
    short           mSpritePosition;
    sf::Time        mRotateTime;
    sf::Time        mTimeLastUpdate;
    const TextureHolder&  mTextures;
    sf::FloatRect   mBoundingRect;

    Animation       mExplosion;
    bool            mShowExplosion;
    bool            mPlayedExplosionSound;

    int             mMaxSpeed;
    int             mDamage;

    EmitterNode*    mTail;
    std::map<short, sf::IntRect>    mTexturesMap;
};

#endif // ASTEROID_H
