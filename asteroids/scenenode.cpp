#include "scenenode.h"
#include "command.h"

#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RectangleShape.hpp>

#include <cassert>
#include <algorithm>
#include <set>

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

std::vector<SceneNode::Ptr>::size_type SceneNode::getChildsCount() const
{
    return mChildren.size();
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
    //qDebug() << "cat" << command.category << " " << getCategory();
    if (command.category & getCategory()){
//        if (command.category & 16) {
//            qDebug() << "cat" << command.category << " " << getCategory();
//        }
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

void SceneNode::removeWrecks()
{
    auto remove = remove_if(mChildren.begin(), mChildren.end(),
                                std::mem_fn(&SceneNode::isMarkedForRemoval));
    mChildren.erase(remove, mChildren.end());

    std::for_each(mChildren.begin(), mChildren.end(),
                                    std::mem_fn(&SceneNode::removeWrecks));
}

sf::FloatRect SceneNode::getBoundingRect() const
{
    return sf::FloatRect();
}

bool SceneNode::isMarkedForRemoval() const
{
    return isDestroyed();
}

bool SceneNode::isDestroyed() const
{
    return false; // by default
}

void SceneNode::checkSceneCollision(SceneNode& graph,
                                        std::set<Pair>& collisionPairs)
{
    checkNodeCollision(graph, collisionPairs);

    for (Ptr& child : graph.mChildren) {
        checkSceneCollision(*child, collisionPairs);
    }
}

void SceneNode::checkNodeCollision(SceneNode& node,
                                        std::set<Pair>& collisionPairs)
{
    if (this != &node && collision(*this, node)
            && !isDestroyed() && !node.isDestroyed()) {
        collisionPairs.insert(std::minmax(this, &node));
    }

    for (Ptr& child : mChildren) {
        child->checkNodeCollision(node, collisionPairs);
    }
}

void SceneNode::updatePositionNodes(std::array<column, nodesCount>& nodes,
                                    float nodeWidth, float nodeHeight,
                                    float worldMargine)
{
    short row = static_cast<short>((getWorldPosition().y + worldMargine)
                                                             / (nodeHeight + 1));
    short col = static_cast<short>((getWorldPosition().x + worldMargine)
                                                             / (nodeWidth + 1));

    nodes[row][col].push_back(this);

    for (Ptr& child : mChildren) {
        child->updatePositionNodes(nodes, nodeWidth, nodeHeight, worldMargine);
    }
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
    //drawBoundingRect(target, states);
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

bool collision(SceneNode& first, SceneNode& second)
{
    return first.getBoundingRect().intersects(second.getBoundingRect());
}
