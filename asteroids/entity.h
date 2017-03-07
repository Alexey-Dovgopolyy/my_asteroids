#ifndef ENTITY_H
#define ENTITY_H

#include "scenenode.h"

class Entity : public SceneNode
{
public:
    explicit        Entity(int hitpoints);

    void            setVelocity(sf::Vector2f velocity);
    void            setVelocity(float vx, float vy);
    void            accelerate(sf::Vector2f velocity);
    void            accelerate(float vx, float vy);
    sf::Vector2f    getVelocity();

    int             getHitpoints() const;
    void            repair(int points);
    void            damage(int points);
    void            destroy();
    virtual void    remove();
    virtual bool    isDestroyed() const;
    void            setRect(sf::FloatRect rect);
    sf::FloatRect   getRect() const;

protected:
    virtual void    updateCurrent(sf::Time dt,
                                 CommandQueue& commands);
    void            correctBoundingRect();

private:
    sf::Vector2f        mVelocity;
    int                 mHitpoints;
    sf::FloatRect       mBoundingRect;
};

#endif // ENTITY_H
