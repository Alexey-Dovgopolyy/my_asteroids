#include "datatable.h"
#include "asteroid.h"
#include "aircraft.h"
#include "levelidentifiers.h"

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
    data[Asteroid::Rock].hitpoints      = 60;
    data[Asteroid::Rock].damage         = 20;
    data[Asteroid::Rock].texture        = Textures::RockAsteroid;
    data[Asteroid::Rock].textureRect    = sf::IntRect(0, 202, 100, 82);
    data[Asteroid::Rock].spritePosition = 75;
    data[Asteroid::Rock].rotateTime     = sf::seconds(0.05f);

    data[Asteroid::Ice].speed           = { 50, 100, 140 };
    data[Asteroid::Ice].wrecksCount     = 4;
    data[Asteroid::Ice].hitpoints       = 100;
    data[Asteroid::Ice].damage          = 40;
    data[Asteroid::Ice].texture         = Textures::IceAsteroid;
    data[Asteroid::Ice].textureRect     = sf::IntRect(5, 7, 60, 57);
    data[Asteroid::Ice].spritePosition  = 75;
    data[Asteroid::Ice].rotateTime      = sf::seconds(0.05f);

    return data;
}

std::vector<LevelData> initializeLevelData()
{
    std::vector<LevelData> data(Levels::LevelCount);

    data[Levels::Level1].rockAsteroidsCount = 10;
    data[Levels::Level1].iceAsteroidsCount = 0;

    data[Levels::Level2].rockAsteroidsCount = 20;
    data[Levels::Level2].iceAsteroidsCount = 0;

    data[Levels::Level3].rockAsteroidsCount = 10;
    data[Levels::Level3].iceAsteroidsCount = 10;

    data[Levels::Level4].rockAsteroidsCount = 10;
    data[Levels::Level4].iceAsteroidsCount = 20;

    data[Levels::Level5].rockAsteroidsCount = 20;
    data[Levels::Level5].iceAsteroidsCount = 20;

    return data;
}
