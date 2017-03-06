#include "scenenode.h"
#include "command.h"

#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RectangleShape.hpp>

#include <cassert>
#include <algorithm>

#include <QDebug>

SceneNode::SceneNode(Category::Type category)
    : mChildren()
    , mParent(nullptr)
    , mType(category)
{

}

void SceneNode::attachChild(Ptr child)
{
    child->mParent = this;
    mChildren.push_back(std::move(child));
}

SceneNode::Ptr SceneNode::detachChild(const SceneNode& node)
{
    auto found = std::find_if(mChildren.begin(), mChildren.end(),
                              [&] (Ptr& p) -> bool {
                                return p.get() == &node;
                                });
    assert(found != mChildren.end());

    Ptr child = std::move(*found);
    child->mParent = nullptr;
    mChildren.erase(found);
    return child;
}

sf::Vector2f SceneNode::getWorldPosition() const
{
    return getWorldTransform() * sf::Vector2f();
}

sf::Transform SceneNode::getWorldTransform() const
{
    sf::Transform transform = sf::Transform::Identity;

    for (const SceneNode* node = this; node != nullptr;
                                            node = node->mParent) {
       transform = node->getTransform() * transform;
    }

    return transform;
}

void SceneNode::update(sf::Time dt, CommandQueue& commands)
{
    updateCurrent(dt, commands);
    updateChildren(dt, commands);
}

void SceneNode::onCommand(const Command& command, sf::Time dt)
{
    if (command.category & getCategory()){
        command.action(*this, dt);
    }

    for (Ptr& child : mChildren) {
        child->onCommand(command, dt);
    }
}

unsigned int SceneNode::getCategory() const
{
    return mType;
}

sf::FloatRect SceneNode::getBoundingRect() const
{
    return sf::FloatRect();
}

void SceneNode::updateCurrent(sf::Time, CommandQueue &)
{
    // reimplement in childrens
}

void SceneNode::updateChildren(sf::Time dt, CommandQueue& commands)
{
    for (Ptr& child : mChildren) {
        child->update(dt, commands);
    }
}

void SceneNode::draw(sf::RenderTarget& target,
                     sf::RenderStates states) const
{
    states.transform *= getTransform();
    drawCurrent(target, states);
    drawChildren(target, states);
    drawBoundingRect(target, states);
}

void SceneNode::drawCurrent(sf::RenderTarget&,
                            sf::RenderStates) const
{
    //reimplemented by childrens
}

void SceneNode::drawChildren(sf::RenderTarget& target,
                             sf::RenderStates states) const
{
    for (const Ptr& child : mChildren) {
        child->draw(target, states);
    }
}

void SceneNode::drawBoundingRect(sf::RenderTarget& target,
                                 sf::RenderStates) const
{
    sf::FloatRect rect = getBoundingRect();

    sf::RectangleShape shape;
    shape.setPosition(sf::Vector2f(rect.left, rect.top));
    shape.setSize(sf::Vector2f(rect.width, rect.height));
    shape.setFillColor(sf::Color::Transparent);
    shape.setOutlineColor(sf::Color::Green);
    shape.setOutlineThickness(1.f);

    target.draw(shape);
}
