#ifndef SCENENODE_H
#define SCENENODE_H

#include "category.h"

#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/Transformable.hpp>
#include <SFML/System/NonCopyable.hpp>
#include <SFML/System/Time.hpp>

#include <memory>
#include <vector>
#include <utility>
#include <array>
#include <set>

struct Command;
class CommandQueue;
class SceneNode;

const short nodesCount = 10;
using column = std::array<std::vector<SceneNode*>, nodesCount>;

class SceneNode : public sf::Drawable, public sf::Transformable,
                    private sf::NonCopyable
{
public :
    typedef std::unique_ptr<SceneNode> Ptr;
    typedef std::pair<SceneNode*, SceneNode*> Pair;

public:
    explicit SceneNode(Category::Type category = Category::Type::None);

    void            attachChild(Ptr child);
    Ptr             detachChild(const SceneNode& node);
    std::vector<Ptr>::size_type getChildsCount() const;

    sf::Vector2f    getWorldPosition() const;
    sf::Transform   getWorldTransform() const;

    void            update(sf::Time dt, CommandQueue& commands);
    void            onCommand(const Command& command, sf::Time dt);
    virtual unsigned int getCategory() const;
    void            removeWrecks();

    virtual sf::FloatRect getBoundingRect() const;
    virtual bool    isMarkedForRemoval() const;
    virtual bool    isDestroyed() const;

    void            checkSceneCollision(SceneNode& graph,
                                        std::set<Pair>& collisionPairs);
    void            checkNodeCollision(SceneNode& node,
                                       std::set<Pair>& collisionPairs);

    void            updatePositionNodes(std::array<column, nodesCount>& nodes,
                                        float nodeWidth, float nodeHeight,
                                        float worldMargine);

private:
    virtual void    updateCurrent(sf::Time dt, CommandQueue& commands);
    void            updateChildren(sf::Time dt, CommandQueue &commands);

    virtual void    draw(sf::RenderTarget& target,
                         sf::RenderStates states) const;
    virtual void    drawCurrent(sf::RenderTarget& target,
                                sf::RenderStates states) const;
    void            drawChildren(sf::RenderTarget& target,
                                 sf::RenderStates states) const;
    void            drawBoundingRect(sf::RenderTarget& target,
                                     sf::RenderStates states) const;
private:
    std::vector<Ptr>        mChildren;
    SceneNode*              mParent;
    Category::Type          mType;

};

bool collision(SceneNode &first, SceneNode &second);

#endif // SCENENODE_H
