#ifndef CATEGORY
#define CATEGORY

namespace Category {

    enum Type {
        None                = 0,
        SceneAirLayer       = 1 << 0,
        PlayerAircraft      = 1 << 1,
        RockAsteroid        = 1 << 2,
        IceAsteroid         = 1 << 3,
        AlliedProjectile    = 1 << 4,

        Asteroid = RockAsteroid | IceAsteroid
    };

}

#endif // CATEGORY

