#include "world.h"
#include <QDebug>
#include <algorithm>
#include <cmath>

const float worldMargine = 20.f;

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
    while (!mCommandQueue.isEmpty()) {
        mSceneGraph.onCommand(mCommandQueue.pop(), dt);
    }
    adaptPlayerVelocity();

    mSceneGraph.update(dt, mCommandQueue);
    adaptPlayerPosition();
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
    mTextures.load(Textures::Space, "Media/Textures/Space.jpg");
    mTextures.load(Textures::Eagle, "Media/Textures/Eagle.png");
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

    // TO DO
    // Need to recalculate velocity for different angles

    // velocity correction
    if (abs(currentVelocity.x) < 2.f) currentVelocity.x = 0.f;
    if (abs(currentVelocity.y) < 2.f) currentVelocity.y = 0.f;

    // check max speed
    float maxSpeed = 0;
    if (currentVelocity.x != 0 && currentVelocity.y != 0) {
        maxSpeed = maxAircraftSpeed / std::sqrt(2.f);
    }
    else {
        maxSpeed = maxAircraftSpeed;
    }

    qDebug() << currentVelocity.x << " " << currentVelocity.y;
    qDebug() << maxSpeed;

    if (currentVelocity.x > maxSpeed) {
        currentVelocity.x = maxSpeed;
    }
    else if (currentVelocity.x < -maxSpeed) {
        currentVelocity.x  = -maxSpeed;
    }
    if (currentVelocity.y > maxSpeed) {
        currentVelocity.y = maxSpeed;
    }
    else if (currentVelocity.y < -maxSpeed) {
        currentVelocity.y = -maxSpeed;
    }

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
