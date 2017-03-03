#ifndef WORLD_H
#define WORLD_H

#include "aircraft.h"
#include "resourceidentifiers.h"
#include "resouceholder.h"
#include "commandqueue.h"
#include "spritenode.h"

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Time.hpp>
#include <SFML/System/NonCopyable.hpp>

#include <array>

const float maxAircraftSpeed = 140.f;

class World
{
public:
                    World(sf::RenderTarget& outputTarget,
                          FontHolder& fonts);

    void            update(sf::Time dt);
    void            draw();

    CommandQueue&   getCommandQueue();

private:
    void            buildScene();
    void            loadTextures();

    void            adaptPlayerPosition();
    void            adaptPlayerVelocity();

private:
    enum Layer {

        Background,
        LowerAir,
        UpperAir,
        LayerCount
    };

private:
    sf::RenderTarget&                   mTarget;
    sf::View                            mWorldView;
    TextureHolder                       mTextures;
    FontHolder&                         mFonts;

    SceneNode                           mSceneGraph;
    std::array<SceneNode*, LayerCount>  mSceneLayer;
    CommandQueue                        mCommandQueue;

    sf::FloatRect                       mWorldBounds;
    sf::Vector2f                        mSpawnPosition;
    Aircraft*                           mPlayerAircraft;

};

#endif // WORLD_H
