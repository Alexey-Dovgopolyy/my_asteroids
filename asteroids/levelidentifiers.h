#ifndef LEVELIDENTIFIERS_H
#define LEVELIDENTIFIERS_H

#include "asteroid.h"

namespace Levels {

    enum ID {
        Level1,
        Level2,
        Level3,
        Level4,
        Level5,
        LevelCount
    };
}

struct LevelInfo {

    LevelInfo(int rock, int ice, int fire)
        : rockAsteroidsLeft(rock)
        , iceAsteroidsLeft(ice)
        , fireAsteroidsLeft(fire)
    {
    }

    bool isAsteroidAvailable(Asteroid::Type type) {

        if (type == Asteroid::Rock && rockAsteroidsLeft > 0) {
            return true;
        }
        else if (type == Asteroid::Ice && iceAsteroidsLeft > 0) {
            return true;
        }
        else if (type == Asteroid::Fire && fireAsteroidsLeft > 0) {
            return true;
        }
        else {
            return false;
        }
    }

    int rockAsteroidsLeft;
    int iceAsteroidsLeft;
    int fireAsteroidsLeft;
};

#endif // LEVELIDENTIFIERS_H
