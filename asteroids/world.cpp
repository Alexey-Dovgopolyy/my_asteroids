#include "world.h"
#include <QDebug>
#include <algorithm>

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
    loadTextures();
    buildScene();
}

void World::update(sf::Time dt)
{
    mSceneGraph.update(dt, mCommandQueue);
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
    std::unique_ptr<SpriteNode> spaceSprite(new SpriteNode(spaceTexture));
    spaceSprite->setPosition(0.f, 0.f);
    mSceneLayer[Background]->attachChild(std::move(spaceSprite));

}

void World::loadTextures()
{
    mTextures.load(Textures::Space, "Media/Textures/Space.jpg");
    mTextures.load(Textures::Eagle, "Media/Textures/Eagle.png");
}
