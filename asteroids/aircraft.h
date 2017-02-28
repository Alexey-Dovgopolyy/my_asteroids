#ifndef AIRCRAFT_H
#define AIRCRAFT_H

#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Sprite.hpp>

#include "entity.h"
#include "utility.h"

class Aircraft : public Entity
{
public:
    Aircraft();


private:
    sf::Sprite				mSprite;
    double                  mDirection;

};

#endif // AIRCRAFT_H
