#ifndef UTILITY_H
#define UTILITY_H

//#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Window/Event.hpp>

#include <sstream>

namespace sf {

    class Sprite;
    class Text;
}

class Animation;

std::string toString(sf::Keyboard::Key key);
//std::string toString(sf::Mouse::Button button);

void centerOrigin(sf::Sprite& sprite);
void centerOrigin(Animation& animation);
void centerOrigin(sf::Text& text);

// Degree/radian conversion
float			toDegree(float radian);
float			toRadian(float degree);


int             randomInt(int exclusiveMax);
#endif // UTILITY_H
