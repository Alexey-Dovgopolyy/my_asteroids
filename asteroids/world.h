#ifndef WORLD_H
#define WORLD_H

#include "aircraft.h"
#include "resourceidentifiers.h"
#include "resouceholder.h"
#include "commandqueue.h"
#include "spritenode.h"
#include "asteroid.h"

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Time.hpp>
#include <SFML/System/NonCopyable.hpp>

#include <array>
#include <vector>
#include <set>

const short columns = 10;
const short rows    = 10;

//const short nodesCount = 10;
//using column = std::array<std::vector<SceneNode*>, nodesCount>;

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
    void            dealWithMaxPlayerSpeed(sf::Vector2f& current,
                                           float maxX, float maxY);
    void            destroyEntitiesOutsideView();
    void            spawnAsteroids(sf::Time dt);
    void            handleCollisions();
    void            debugShowNodes(std::array<column, nodesCount> &nodes);

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
