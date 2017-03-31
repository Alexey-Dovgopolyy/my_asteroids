#ifndef AIRCRAFT_H
#define AIRCRAFT_H

#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include "SFML/Graphics/RenderTarget.hpp"

#include "entity.h"
#include "command.h"
#include "resouceholder.h"
#include "resourceidentifiers.h"
#include "utility.h"
#include "projectile.h"
#include "animation.h"

class Aircraft : public Entity
{
public:
    enum Type {
        Eagle,
        TypeCount
    };

public:
    Aircraft(Type type, const TextureHolder& textures);

    virtual unsigned int    getCategory() const;
    virtual sf::FloatRect   getBoundingRect() const;
    virtual void            remove();
    virtual bool            isMarkedForRemoval() const;

    void        setDirection(float angle);
    float       getDirection() const;

    void        fire();
    bool        isAnimationFinnished();
    void        playLocalSound(CommandQueue& commands,
                               SoundEffect::ID effect);

private:
    virtual void drawCurrent(sf::RenderTarget& target,
                             sf::RenderStates states) const;
    virtual void updateCurrent(sf::Time dt, CommandQueue& commands);

    void        checkProjectileLaunch(sf::Time dt,
                                        CommandQueue& commands);
    void        createBullet(SceneNode& node,
                             const TextureHolder& textures);
    void        createProjectile(SceneNode& node,
                                 Projectile::Type type,
                                 float xOffset, float yOffset,
                                 const TextureHolder& textures);
    //void        correctBoundingRect();

private:
    Type                    mType;
    sf::Sprite				mSprite;
    Command                 mFireCommand;
    sf::Time                mFireCountdown;
    Animation               mExplosion;

    bool                    mIsFiring;
    bool                    mShowExplosion;
    bool                    mPlayedExplosionSound;

    int                     mFireRateLevel;
    float                   mDirection;
};

#endif // AIRCRAFT_H
