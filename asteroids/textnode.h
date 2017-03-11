#ifndef TEXTNODE_H
#define TEXTNODE_H

#include "scenenode.h"
#include "resouceholder.h"
#include "resourceidentifiers.h"

#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/Font.hpp>

class TextNode : public SceneNode
{
public:
    explicit TextNode(const FontHolder& fonts,
                      const std::string& text);

    void        setString(const std::string& text);

private:
    virtual void drawCurrent(sf::RenderTarget &target,
                             sf::RenderStates states) const;

private:
    sf::Text mText;
};

#endif // TEXTNODE_H
