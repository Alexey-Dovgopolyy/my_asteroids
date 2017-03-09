#include "world.h"
#include <QDebug>
#include <algorithm>
#include <cmath>
#include <iostream>

const float worldMargine        = 10.f;
const float maxAircraftSpeed    = 140.f;

World::World(sf::RenderTarget &outputTarget, FontHolder &fonts)
    : mTarget(outputTarget)
    , mWorldView(outputTarget.getDefaultView())
    , mTextures()
    , mFonts(fonts)
    , mSceneGraph()
    , mSceneLayer()
    , mWorldBounds(-worldMargine, -worldMargine,
                    mWorldView.getSize().x + worldMargine,
                    mWorldView.getSize().y + worldMargine)
    , mSpawnPosition(mWorldView.getSize().x / 2.f,
                    mWorldView.getSize().y / 2.f)
    , mPlayerAircraft(nullptr)
{
    //qDebug() << mSpawnPosition.x << " " << mSpawnPosition.y;
    loadTextures();
    buildScene();
}

void World::update(sf::Time dt)
{
    destroyEntitiesOutsideView();

    while (!mCommandQueue.isEmpty()) {
        mSceneGraph.onCommand(mCommandQueue.pop(), dt);
    }
    adaptPlayerVelocity();

    handleCollisions();

    spawnAsteroids(dt);

    mSceneGraph.removeWrecks();
    mSceneGraph.update(dt, mCommandQueue);
    adaptPlayerPosition();

    //qDebug() << "low" << mSceneLayer[LowerAir]->getChildsCount();
    //qDebug() << "up" << mSceneLayer[UpperAir]->getChildsCount();
}

void World::draw()
{
    mTarget.setView(mWorldView);
    mTarget.draw(mSceneGraph);
}

CommandQueue& World::getCommandQueue()
{
    return mCommandQueue;
}

void World::buildScene()
{
    for (std::size_t i = 0; i < LayerCount; ++i) {

        Category::Type category = (i == LowerAir) ?
                    Category::SceneAirLayer : Category::None;
        SceneNode::Ptr layer(new SceneNode(category));
        mSceneLayer[i] = layer.get();
        mSceneGraph.attachChild(std::move(layer));
    }

    sf::Texture& spaceTexture = mTextures.get(Textures::Space);
    std::unique_ptr<SpriteNode> spaceSprite
                                    (new SpriteNode(spaceTexture));
    spaceSprite->setPosition(0.f, 0.f);
    mSceneLayer[Background]->attachChild(std::move(spaceSprite));

    //qDebug() << 1;
    std::unique_ptr<Aircraft> playerAircraft
                    (new Aircraft(Aircraft::Eagle, mTextures));
    mPlayerAircraft = playerAircraft.get();
    mSceneLayer[UpperAir]->attachChild(std::move(playerAircraft));
    mPlayerAircraft->setPosition(mSpawnPosition);
    //qDebug() << 2;


}

void World::loadTextures()
{
    mTextures.load(Textures::Space,         "Media/Textures/Space.jpg");
    mTextures.load(Textures::Eagle,         "Media/Textures/Eagle.png");
    mTextures.load(Textures::Entities,      "Media/Textures/Entities.png");
    mTextures.load(Textures::RockAsteroid,  "Media/Textures/Asteroid1.png");
}

void World::adaptPlayerPosition()
{
    sf::Vector2f position = mPlayerAircraft->getPosition();

    if (position.x < mWorldBounds.left)
        position.x = mWorldBounds.width;
    if (position.x > mWorldBounds.width)
        position.x = mWorldBounds.left;
    if (position.y < mWorldBounds.top)
        position.y = mWorldBounds.height;
    if (position.y > mWorldBounds.height)
        position.y = mWorldBounds.top;

    mPlayerAircraft->setPosition(position);
}

void World::adaptPlayerVelocity()
{
    static sf::Vector2f previousVelocity = sf::Vector2f(0, 0);
    sf::Vector2f currentVelocity = mPlayerAircraft->getVelocity();

    // velocity correction
    if (abs(currentVelocity.x) < 1.f) currentVelocity.x = 0.f;
    if (abs(currentVelocity.y) < 1.f) currentVelocity.y = 0.f;

    // check max speed
    float maxX = maxAircraftSpeed;
    float maxY = maxAircraftSpeed;
    float moveDirection = 0.f;

    moveDirection = std::atan2(currentVelocity.x, currentVelocity.y);
    //qDebug() << "dir" << toDegree(moveDirection);
    maxX = abs(maxAircraftSpeed * std::sin(moveDirection));
    maxY = abs(maxAircraftSpeed * std::cos(moveDirection));

    dealWithMaxPlayerSpeed(currentVelocity, maxX, maxY);

    // reduce velocity if it stays the same
    if (previousVelocity.x == currentVelocity.x) {
        currentVelocity.x *= 0.98f;
    }
    if (previousVelocity.y == currentVelocity.y) {
        currentVelocity.y *= 0.98f;
    }

    //set calculated velocity
    mPlayerAircraft->setVelocity(currentVelocity);
    previousVelocity = currentVelocity;
}

void World::dealWithMaxPlayerSpeed(sf::Vector2f &current, float maxX, float maxY)
{
    if (current.x > maxX) {
        current.x = maxX;
    }
    else if (current.x < -maxX) {
        current.x = -maxX;
    }
    if (current.y > maxY) {
        current.y = maxY;
    }
    else if (current.y < -maxY) {
        current.y = -maxY;
    }
}

void World::destroyEntitiesOutsideView()
{
    Command command;
    command.category =  Category::Asteroid | Category::AlliedProjectile;
    command.action = derivedAction<Entity>([this] (Entity& e, sf::Time){

            if (!mWorldBounds.intersects(e.getBoundingRect())) {
                e.remove();
            }
    });

    mCommandQueue.push(command);
}

void World::spawnAsteroids(sf::Time dt)
{

    static sf::Time spawnTime = sf::Time::Zero;
    static int spawnRand = randomInt(5001);


    if (spawnTime.asMilliseconds() > spawnRand) {

        spawnTime = sf::Time::Zero;
        spawnRand += randomInt(5001);

        std::unique_ptr<Asteroid> asteroid(new Asteroid(Asteroid::Rock,
                                                        Asteroid::Size::Solid,
                                                        2, mTextures));

        int rx = randomInt(2);
        int ry = 0;

        if (rx > 0) {
            rx = randomInt(mWorldBounds.width);
        }
        else {
            ry = randomInt(mWorldBounds.height);
        }

        asteroid->setPosition(static_cast<float>(rx), static_cast<float>(ry));
        asteroid->setVelocity(20.f, 20.f);

        mSceneLayer[UpperAir]->attachChild(std::move(asteroid));
    }
    else {
        spawnTime += dt;
    }


}

bool matchesCategories(SceneNode::Pair& colliders, Category::Type type1,
                                                    Category::Type type2)
{
    unsigned int category1 = colliders.first->getCategory();
    unsigned int category2 = colliders.second->getCategory();

    if (category1 & type1 && category2 & type2) {
        return true;
    }
    else if (category1 & type2 && category2 & type1) {
        std::swap(colliders.first, colliders.second);
        return true;
    }
    else {
        return false;
    }
}

void World::handleCollisions()
{
    std::set<SceneNode::Pair> collisionPairs;
    mSceneGraph.checkSceneCollision(mSceneGraph, collisionPairs);

    for (SceneNode::Pair pair : collisionPairs) {

        if (matchesCategories(pair, Category::AlliedProjectile,
                                                    Category::Asteroid)) {
            auto& projectile = static_cast<Projectile&>(*pair.first);
            auto& asteroid = static_cast<Asteroid&>(*pair.second);

            asteroid.damage(projectile.getDamage());
            projectile.destroy();
        }
        else if (matchesCategories(pair, Category::Asteroid,
                                   Category::PlayerAircraft)) {

            auto& asteroid = static_cast<Asteroid&>(*pair.first);
            auto& aircraft = static_cast<Aircraft&>(*pair.second);

            aircraft.damage(asteroid.getHitpoints());
            asteroid.destroy();
        }
    }
}

void World::debugShowNodes(std::array<column, nodesCount>& nodes)
{
    std::cout << "==================================" << std::endl;
    for (int i = 0; i < nodesCount; ++i) {
        for (int j = 0; j < nodesCount; ++j) {
            std::cout << nodes[i][j].size() << " ";
        }
        std::cout << std::endl;
    }
}
