#ifndef CATEGORY
#define CATEGORY

namespace Category {

    enum Type {
        None                = 0,
        SceneAirLayer       = 1 << 0,
        PlayerAircraft      = 1 << 1,
        RockAsteroid        = 1 << 2,
        IceAsteroid         = 1 << 3,
        FireAsteroid        = 1 << 4,
        AlliedProjectile    = 1 << 5,
        ParticleSystem      = 1 << 6,

        Asteroid = RockAsteroid | IceAsteroid | FireAsteroid
    };

}

#endif // CATEGORY

