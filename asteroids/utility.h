#ifndef UTILITY_H
#define UTILITY_H

#include <SFML/Graphics/Sprite.hpp>

namespace sf {

    class Sprite;
}

void centerOrigin(sf::Sprite& sprite);

// Degree/radian conversion
float			toDegree(float radian);
float			toRadian(float degree);


int             randomInt(int exclusiveMax);
#endif // UTILITY_H
