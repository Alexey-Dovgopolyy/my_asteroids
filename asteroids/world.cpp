#include "world.h"
#include <QDebug>
#include <algorithm>
#include <cmath>

const float worldMargine        = 20.f;
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
    spawnAsteroids();
}

void World::update(sf::Time dt)
{
    destroyEntitiesOutsideView();

    while (!mCommandQueue.isEmpty()) {
        mSceneGraph.onCommand(mCommandQueue.pop(), dt);
    }
    adaptPlayerVelocity();

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

    std::unique_ptr<Aircraft> playerAircraft
                    (new Aircraft(Aircraft::Eagle, mTextures));
    mPlayerAircraft = playerAircraft.get();
    mSceneLayer[UpperAir]->attachChild(std::move(playerAircraft));
    mPlayerAircraft->setPosition(mSpawnPosition);
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
    command.action = derivedAction<Entity>([this] (Entity& e, sf::Time dt){

            if (!mWorldBounds.intersects(e.getBoundingRect())) {
                e.remove();
            }
    });

    mCommandQueue.push(command);
}

void World::spawnAsteroids()
{
    std::unique_ptr<Asteroid> asteroid(new Asteroid(Asteroid::Rock,mTextures));
    asteroid->setPosition(30.f, 30.f);
    asteroid->setVelocity(20.f, 20.f);

    mSceneLayer[UpperAir]->attachChild(std::move(asteroid));
}
