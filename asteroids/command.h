#ifndef COMMAND_H
#define COMMAND_H

#include "scenenode.h"
#include "category.h"

#include <SFML/System/Time.hpp>

#include <functional>
#include <cassert>

struct Command
{
    typedef std::function<void(SceneNode&, sf::Time)> Action;

    Command();

    Action          action;
    unsigned int    category;
};

template<typename GameObject, typename function>
Command::Action derivedAction(function fn)
{
    return [=] (SceneNode& node, sf::Time dt) {
        assert(static_cast<GameObject*>(&node) != nullptr);
        fn(static_cast<GameObject&>(node), dt);
    };
}

#endif // COMMAND_H
