#ifndef LEVELIDENTIFIERS_H
#define LEVELIDENTIFIERS_H

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

    int rockAsteroidsLeft;
    int iceAsteroidsLeft;
    int fireAsteroidsLeft;
};

#endif // LEVELIDENTIFIERS_H
