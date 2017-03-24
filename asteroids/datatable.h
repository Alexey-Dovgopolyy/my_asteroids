#ifndef DATATABLE_H
#define DATATABLE_H

#include <resourceidentifiers.h>

#include <SFML/System/Time.hpp>
#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/Color.hpp>

#include <vector>

struct AircraftData {

    int                 hitpoints;
    int                 speed;
    Textures::ID        texture;
    sf::IntRect         textureRect;
    sf::Time            fireInterval;
};

struct AsteroidData {

    std::vector<int>    speed;
    int                 size;
    int                 wrecksCount;
    int                 hitpoints;
    int                 damage;
    Textures::ID        texture;
    sf::IntRect         textureRect;
    int                 spritePosition;
    sf::Time            rotateTime;
};


struct ParticleData
{
    sf::Color						color;
    sf::Time						lifetime;
};


struct LevelData {

    int                 rockAsteroidsCount;
    int                 iceAsteroidsCount;
    int                 fireAsteroidsCount;
};

std::vector<AircraftData>   initializeAircraftData();
std::vector<AsteroidData>   initializeAsteroidData();
std::vector<ParticleData>	initializeParticleData();
std::vector<LevelData>      initializeLevelData();

#endif // DATATABLE_H
