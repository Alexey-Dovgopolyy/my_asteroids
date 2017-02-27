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

struct Command;
class CommandQueue;

class SceneNode : public sf::Drawable, public sf::Transformable,
                    private sf::NonCopyable
{
public :
    typedef std::unique_ptr<SceneNode> Ptr;


public:
    explicit SceneNode(Category::Type category = Category::Type::None);

    void            attachChild(Ptr child);
    Ptr             detachChild(const SceneNode& node);

    void            update(sf::Time dt, CommandQueue& commands);
    void            onCommand(const Command& command, sf::Time dt);
    virtual unsigned int getCategory() const;

    virtual sf::FloatRect getBoundingRect() const;

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

#endif // SCENENODE_H
