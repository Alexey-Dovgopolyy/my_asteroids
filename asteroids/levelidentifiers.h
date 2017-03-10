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

    LevelInfo(int rock, int ice)
        : rockAsteroidsLeft(rock)
        , iceAsteroidsLeft(ice)
    {
    }

    int rockAsteroidsLeft;
    int iceAsteroidsLeft;
};

#endif // LEVELIDENTIFIERS_H
