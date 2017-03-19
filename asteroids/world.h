#ifndef WORLD_H
#define WORLD_H

#include "aircraft.h"
#include "resourceidentifiers.h"
#include "levelidentifiers.h"
#include "resouceholder.h"
#include "commandqueue.h"
#include "spritenode.h"
#include "asteroid.h"
#include "category.h"
#include "textnode.h"

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
                    World(sf::RenderTarget& outputTarget, FontHolder& fonts,
                          Levels::ID level, int score);

    void            update(sf::Time dt);
    void            draw();

    CommandQueue&   getCommandQueue();

    bool            isAnyAsteroidAlive();

    void            setLevel(Levels::ID level);
    Levels::ID      getLevel() const;

    void            setScore(int score);
    int             getScore() const;

private:
    void            buildScene();
    void            loadTextures();

    void            updateTextNodes();
    void            adaptPlayerPosition();
    void            adaptAstoriodPosition();
    void            adaptPlayerVelocity();
    void            dealWithMaxPlayerSpeed(sf::Vector2f& current,
                                                     float maxX, float maxY);
    void            destroyEntitiesOutsideView();
    void            spawnAsteroids(sf::Time dt);
    Asteroid::Type  determineAsteroidType();
    bool            decreaseAsteroidsCount(Asteroid::Type type);
    void            spawnSmallAsteroid(sf::Vector2f position,
                                                       unsigned int category);
    void            createSmallAsteroid(sf::Vector2f& position,
                                                         Asteroid::Type type);
    void            asteroidSpawnCoordinates(int& x, int& y);
    void            asteroidSpawnVelocity(int& vx, int& vy,
                                          const int xSpawn, const int ySpawn);
    void            handleCollisions();

    // some debug functions
    void            debugShowNodes(std::array<column, nodesCount> &nodes);
    void            spawnIceAsteroid();

private:
    enum Layer {

        Background,
        LowerAir,
        MiddleAir,
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

    unsigned int                        mLevel;
    LevelInfo                           mLevelInfo;

    TextNode*                           mHPMonitor;
    TextNode*                           mLevelMonitor;
    TextNode*                           mScoredPointsMonitor;

    int                                 mPlayerHP;
    int                                 mScoredPoints;
};

#endif // WORLD_H
