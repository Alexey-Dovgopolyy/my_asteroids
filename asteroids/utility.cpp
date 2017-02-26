#include "utility.h"
#include <cmath>

void centerOrigin(sf::Sprite& sprite)
{
    sf::FloatRect bounds = sprite.getLocalBounds();
    sprite.setOrigin(std::floor(bounds.left + bounds.width / 2.f),
                     std::floor(bounds.top + bounds.height / 2.f));
}

float toDegree(float radian)
{
    return 180.f / 3.141592653589793238462643383f * radian;
}

float toRadian(float degree)
{
    return 3.141592653589793238462643383f / 180.f * degree;
}
