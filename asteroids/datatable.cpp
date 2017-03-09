#include "datatable.h"
#include "asteroid.h"
#include "aircraft.h"

#include <QDebug>

std::vector<AircraftData> initializeAircraftData()
{
    std::vector<AircraftData> data(Aircraft::TypeCount);

    data[Aircraft::Eagle].hitpoints      = 200;
    data[Aircraft::Eagle].speed          = 140;
    data[Aircraft::Eagle].texture        = Textures::Entities;
    data[Aircraft::Eagle].textureRect    = sf::IntRect(0, 0, 48, 64);
    data[Aircraft::Eagle].fireInterval   = sf::seconds(1);

    return data;
}

std::vector<AsteroidData> initializeAsteroidData()
{
    std::vector<AsteroidData> data(Asteroid::TypeCount);

    data[Asteroid::Rock].speed          = { 50, 100, 140 };
    data[Asteroid::Rock].wrecksCount    = 2;
    data[Asteroid::Rock].hitpoints      = 100;
    data[Asteroid::Rock].damage         = 20;
    data[Asteroid::Rock].texture        = Textures::RockAsteroid;
    data[Asteroid::Rock].textureRect    = sf::IntRect(0, 202, 100, 82);
    data[Asteroid::Rock].spritePosition = 75;
    data[Asteroid::Rock].rotateTime     = sf::seconds(0.05f);

    return data;
}

std::vector<LevelData> initializeLevelData()
{
    std::vector<LevelData> data;

    return data;
}
