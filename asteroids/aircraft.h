#ifndef AIRCRAFT_H
#define AIRCRAFT_H

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/System/NonCopyable.hpp>
#include <SFML/System/Time.hpp>
#include <SFML/Graphics/Transformable.hpp>
#include <SFML/Graphics/Drawable.hpp>

#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Window/Event.hpp>

#include <stdexcept>

#include "utility.h"

class Aircraft :    public sf::Transformable, public sf::Drawable,
                    private sf::NonCopyable
{
public:
    Aircraft();

    void                    setCoordinates(float x, float y);
    sf::Vector2f            getCoordinates();
    void                    updateDirection(sf::Vector2i mousePos);
    void                    updateCoordinates(float x, float y);
    void                    setAcceleration(float x, float y);
    void                    updateCoordsByAcceleration(sf::Time dt);

private:
    virtual void			draw(sf::RenderTarget& target, sf::RenderStates states) const;

private:

    sf::Texture             mTexture;

    sf::Sprite				mSprite;
    double                  mDirection;
    sf::Vector2f            mCoords;
    sf::Vector2f            mVelocity;
    sf::Vector2f            mAcceleration;
};

#endif // AIRCRAFT_H
