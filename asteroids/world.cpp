#include "world.h"
#include "datatable.h"
#include "particlenode.h"
#include "soundnode.h"

#include <QDebug>
#include <algorithm>
#include <cmath>
#include <iostream>
#include <vector>

const float worldMargine        = 10.f;
const float maxAircraftSpeed    = 140.f;

namespace {
    const std::vector<LevelData> levelTable = initializeLevelData();
}

World::World(sf::RenderTarget &outputTarget, FontHolder &fonts, SoundPlayer &sounds,
             Levels::ID level, int score, int hitpoints)
    : mTarget(outputTarget)
    , mWorldView(outputTarget.getDefaultView())
    , mTextures()
    , mFonts(fonts)
    , mSounds(sounds)
    , mSceneGraph()
    , mSceneLayer()
    , mWorldBounds(-worldMargine, -worldMargine,
                    mWorldView.getSize().x + worldMargine,
                    mWorldView.getSize().y + worldMargine)
    , mSpawnPosition(mWorldView.getSize().x / 2.f,
                    mWorldView.getSize().y / 2.f)
    , mPlayerAircraft(nullptr)
    , mLevel(level)
    , mLevelInfo(levelTable[mLevel].rockAsteroidsCount,
                 levelTable[mLevel].iceAsteroidsCount,
                 levelTable[mLevel].fireAsteroidsCount)
    , mPlayerHP(level == Levels::Level1 ? 200 : hitpoints)
    , mScoredPoints(score)
{
    loadTextures();
    buildScene();
    //spawnIceAsteroid();
    qDebug() << "info" << mLevelInfo.rockAsteroidsLeft
             << mLevelInfo.iceAsteroidsLeft;
}

void World::update(sf::Time dt)
{
    destroyEntitiesOutsideView();

    while (!mCommandQueue.isEmpty()) {
        mSceneGraph.onCommand(mCommandQueue.pop(), dt);
    }
    adaptPlayerVelocity();

    handleCollisions();

    updateTextNodes();

    mSceneGraph.removeWrecks();
    spawnAsteroids(dt);

    mSceneGraph.update(dt, mCommandQueue);
    adaptPlayerPosition();
    adaptAstoriodPosition();

    updateSounds();
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

bool World::isPlayerDead()
{
    return (mPlayerAircraft->getHitpoints() <= 0 &&
            mPlayerAircraft->isAnimationFinnished());
}

bool World::isAnyAsteroidAlive()
{
    //qDebug() << "Level" << mLevel;
    //qDebug() << "fire in table" << levelTable[mLevel].fireAsteroidsCount;
    if (mLevelInfo.iceAsteroidsLeft < 1 &&
            mLevelInfo.rockAsteroidsLeft < 1 &&
                mLevelInfo.fireAsteroidsLeft < 1) {

        //qDebug() << mSceneLayer[MiddleAir]->getChildsCount();
        return static_cast<bool>
                        (mSceneLayer[MiddleAir]->getChildsCount());
    }
    else {
        return true;
    }
}

void World::setLevel(Levels::ID level)
{
    mLevel = level;

    mLevelInfo.rockAsteroidsLeft = levelTable[mLevel].rockAsteroidsCount;
    mLevelInfo.iceAsteroidsLeft = levelTable[mLevel].iceAsteroidsCount;
    mLevelInfo.fireAsteroidsLeft = levelTable[mLevel].fireAsteroidsCount;
}

Levels::ID World::getLevel() const
{
    return static_cast<Levels::ID>(mLevel);
}

void World::setScore(int score)
{
    mScoredPoints = score;
}

int World::getScore() const
{
    return mScoredPoints;
}

int World::getHitpoints() const
{
    return mPlayerHP;
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

    //=====Background Space=====
    sf::Texture& spaceTexture = mTextures.get(Textures::Space);
    std::unique_ptr<SpriteNode> spaceSprite
                                    (new SpriteNode(spaceTexture));
    spaceSprite->setPosition(0.f, 0.f);
    mSceneLayer[Background]->attachChild(std::move(spaceSprite));

    //=====Particle System=====
    std::unique_ptr<ParticleNode> tailNode(new ParticleNode(Particle::Tail, mTextures));
    mSceneLayer[LowerAir]->attachChild(std::move(tailNode));

    //=====Players Aircraft======
    std::unique_ptr<Aircraft> playerAircraft
                    (new Aircraft(Aircraft::Eagle, mTextures));
    mPlayerAircraft = playerAircraft.get();
    mSceneLayer[UpperAir]->attachChild(std::move(playerAircraft));
    mPlayerAircraft->setPosition(mSpawnPosition);

    //=====Sounds=====
    std::unique_ptr<SoundNode> soundNode(new SoundNode(mSounds));
    mSceneGraph.attachChild(std::move(soundNode));

    //=====HP Text=====
    std::string hp = "HP " + std::to_string(mPlayerAircraft->getHitpoints());
    std::cout << hp << std::endl;
    std::unique_ptr<TextNode> hpMonitor(new TextNode(mFonts, hp));
    mHPMonitor = hpMonitor.get();
    mHPMonitor->setPosition(mWorldView.getSize().x * 0.93f,
                            mWorldView.getSize().y * 0.04f);
    mSceneLayer[UpperAir]->attachChild(std::move(hpMonitor));

    //=====Level Text=====
    std::string level = "Level " + std::to_string(mLevel + 1);
    std::unique_ptr<TextNode> levelMonitor(new TextNode(mFonts, level));
    mLevelMonitor = levelMonitor.get();
    mLevelMonitor->setPosition(mWorldView.getSize().x * 0.05f,
                               mWorldView.getSize().y * 0.04f);
    mSceneLayer[UpperAir]->attachChild(std::move(levelMonitor));

    //=====Score Text=====
    std::unique_ptr<TextNode> scoreMonitor(new TextNode(mFonts, ""));
    mScoredPointsMonitor = scoreMonitor.get();
    mScoredPointsMonitor->setPosition(mWorldView.getSize().x / 2.f,
                                      mWorldView.getSize().y * 0.04f);
    mSceneLayer[UpperAir]->attachChild(std::move(scoreMonitor));
}

void World::loadTextures()
{
    mTextures.load(Textures::Space,         "Media/Textures/Space.jpg");
    mTextures.load(Textures::Eagle,         "Media/Textures/Eagle.png");
    mTextures.load(Textures::Entities,      "Media/Textures/Entities.png");
    mTextures.load(Textures::RockAsteroid,  "Media/Textures/Asteroid1.png");
    mTextures.load(Textures::IceAsteroid,   "Media/Textures/Asteroid3.png");
    mTextures.load(Textures::FireAsteroid,  "Media/Textures/Asteroid4.png");
    mTextures.load(Textures::Particle,      "Media/Textures/Particle.png");
    mTextures.load(Textures::Explosion,     "Media/Textures/Explosion.png");
}

void World::updateTextNodes()
{
    std::string msg;
    msg = "HP: " + std::to_string(mPlayerHP);
    mHPMonitor->setString(msg);

    msg = "Score: " + std::to_string(mScoredPoints);
    mScoredPointsMonitor->setString(msg);
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

void World::adaptAstoriodPosition()
{
    Command command;
    command.category = Category::Asteroid;
    command.action = derivedAction<Entity>([this] (Entity& e, sf::Time){

            if (!mWorldBounds.intersects(e.getBoundingRect())) {
                e.adapt(mWorldBounds);
            }
    });

    mCommandQueue.push(command);
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

void World::dealWithMaxPlayerSpeed(sf::Vector2f &current,
                                                    float maxX, float maxY)
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
    command.category = Category::AlliedProjectile;
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
        spawnRand = randomInt(5001);

        Asteroid::Type type = determineAsteroidType();
        if (!decreaseAsteroidsCount(type)) {
            // TO DO
            // need to notify stateStack that the level is over
            return;
        }

        std::unique_ptr<Asteroid> asteroid(new Asteroid(type,
                                                        Asteroid::Size::Solid,
                                                        2, mTextures));

        int xSpawn = randomInt(mWorldBounds.width);
        int ySpawn = randomInt(mWorldBounds.height);
        asteroidSpawnCoordinates(xSpawn, ySpawn);

        //qDebug() << xSpawn << " " << ySpawn;
        asteroid->setPosition(static_cast<float>(xSpawn), static_cast<float>(ySpawn));

        int vx = randomInt(100);
        int vy = randomInt(100);

        if (asteroid->getCategory() & Category::FireAsteroid) {
            vx *= 2;
            vy *= 2;
            if (vx < 100) {
                vx = 100;
            }
            else if (vy < 100) {
                vy = 100;
            }
            //asteroid->createParticleEmitter();
        }

        asteroidSpawnVelocity(vx, vy, xSpawn, ySpawn);
        asteroid->setVelocity(static_cast<float>(vx), static_cast<float>(vy));

        mSceneLayer[MiddleAir]->attachChild(std::move(asteroid));
    }
    else {
        spawnTime += dt;
    }
}

Asteroid::Type World::determineAsteroidType()
{

    std::vector<Asteroid::Type> typesToChoose;

    for (std::size_t i = 0; i < Asteroid::TypeCount; ++i) {

        Asteroid::Type type = static_cast<Asteroid::Type>(i);
        if (mLevelInfo.isAsteroidAvailable(type)) {
            typesToChoose.push_back(type);
        }
    }

    if (!typesToChoose.empty()) {
        int index = randomInt(typesToChoose.size());
        return typesToChoose[index];
    }
    else {
        return Asteroid::Rock;
    }
}

bool World::decreaseAsteroidsCount(Asteroid::Type type)
{
    if (type == Asteroid::Rock && mLevelInfo.rockAsteroidsLeft > 0) {
        --mLevelInfo.rockAsteroidsLeft;
        return true;
    }
    else if (type == Asteroid::Ice && mLevelInfo.iceAsteroidsLeft > 0) {
        --mLevelInfo.iceAsteroidsLeft;
        return true;
    }
    else if (type == Asteroid::Fire && mLevelInfo.fireAsteroidsLeft > 0) {
        --mLevelInfo.fireAsteroidsLeft;
        return true;
    }
    return false;
}

void World::spawnSmallAsteroid(sf::Vector2f position, unsigned int category)
{
    if (category & Category::RockAsteroid) {

        for (int i = 0; i < 2; ++i) {
            createSmallAsteroid(position, Asteroid::Rock);
        }
    }
    else if (category & Category::IceAsteroid) {

        for (int i = 0; i < 3; ++i) {
            createSmallAsteroid(position, Asteroid::Ice);
        }
    }
}

void World::createSmallAsteroid(sf::Vector2f &position, Asteroid::Type type)
{
    std::unique_ptr<Asteroid> asteroid(new Asteroid(type, Asteroid::Size::Wreck,
                                                    2, mTextures));
    asteroid->setPosition(position);

    int vx = randomInt(100);
    int vy = randomInt(100);
    asteroid->setVelocity(vx, vy);

    mSceneLayer[MiddleAir]->attachChild(std::move(asteroid));
}

void World::asteroidSpawnCoordinates(int &x, int &y)
{
    int chooseAsix = randomInt(2); // 0 - y asix, 1 - x asix
    if (chooseAsix) {
        if (x >= mWorldBounds.width / 2.f) {
            x = mWorldBounds.width;
        }
        else {
            x = 0;
        }
    }
    else {
        if (y >= mWorldBounds.height / 2.f) {
            y = mWorldBounds.height;
        }
        else {
            y = 0;
        }
    }
}

void World::asteroidSpawnVelocity(int &vx, int &vy, const int xSpawn, const int ySpawn)
{
    if (xSpawn > mWorldBounds.width / 2.f) {
        vx *= -1;
    }
    if (ySpawn > mWorldBounds.height / 2.f) {
        vy *= -1;
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

            if (asteroid.getHitpoints() <= 0) {

                mScoredPoints += asteroid.getScore();
                if (asteroid.getSize() == Asteroid::Size::Solid) {
                    spawnSmallAsteroid(asteroid.getPosition(), asteroid.getCategory());
                }
            }

            projectile.destroy();
        }
        else if (matchesCategories(pair, Category::Asteroid,
                                   Category::PlayerAircraft)) {

            auto& asteroid = static_cast<Asteroid&>(*pair.first);
            auto& aircraft = static_cast<Aircraft&>(*pair.second);

            aircraft.damage(asteroid.getHitpoints());
            asteroid.destroy();

            mPlayerHP = aircraft.getHitpoints();
        }
    }
}

void World::updateSounds()
{
    mSounds.removeStoppedSound();
}


void World::spawnIceAsteroid()
{
    std::unique_ptr<Asteroid> asteroid(new Asteroid(Asteroid::Fire,
                                                    Asteroid::Size::Solid,
                                                    2, mTextures));

    int xSpawn = randomInt(mWorldBounds.width);
    int ySpawn = randomInt(mWorldBounds.height);
    asteroidSpawnCoordinates(xSpawn, ySpawn);

    qDebug() << xSpawn << " " << ySpawn;
    asteroid->setPosition(static_cast<float>(xSpawn), static_cast<float>(ySpawn));

    int vx = randomInt(100);
    int vy = randomInt(100);
    asteroidSpawnVelocity(vx, vy, xSpawn, ySpawn);
    asteroid->setVelocity(static_cast<float>(vx), static_cast<float>(vy));
    //asteroid->createParticleEmitter();

    mSceneLayer[MiddleAir]->attachChild(std::move(asteroid));
}
