#include "projectile.h"
#include "resouceholder.h"
#include "utility.h"

#include <SFML/Graphics/RenderTarget.hpp>

Projectile::Projectile(Type type, const TextureHolder &textures)
    : Entity(1)
    , mType(type)
    , mSprite(textures.get(Textures::Entities),
              sf::IntRect(175, 64, 3, 14))
    , mTargetDirection()
{
    centerOrigin(mSprite);
    setRect(mSprite.getGlobalBounds());
    correctBoundingRect();
}

unsigned int Projectile::getCategory() const
{
    return Category::AlliedProjectile;
}

sf::FloatRect Projectile::getBoundingRect() const
{
    return getWorldTransform().transformRect(getRect());
    //return getWorldTransform().transformRect(mSprite.getGlobalBounds());
}

float Projectile::getMaxSpeed() const
{
    return 400;
}

int Projectile::getDamage() const
{
    return 10;
}

void Projectile::updateCurrent(sf::Time dt, CommandQueue& commands)
{
    Entity::updateCurrent(dt, commands);
}

void Projectile::drawCurrent(sf::RenderTarget& target,
                                    sf::RenderStates states) const
{
    target.draw(mSprite, states);
}
