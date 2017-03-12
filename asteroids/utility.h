#ifndef UTILITY_H
#define UTILITY_H

#include <SFML/Graphics/Sprite.hpp>

namespace sf {

    class Sprite;
    class Text;
}

class Animation;

void centerOrigin(sf::Sprite& sprite);
void centerOrigin(Animation& animation);
void centerOrigin(sf::Text& text);

// Degree/radian conversion
float			toDegree(float radian);
float			toRadian(float degree);


int             randomInt(int exclusiveMax);
#endif // UTILITY_H
