#ifndef RESOURCEIDENTIFIERS
#define RESOURCEIDENTIFIERS

namespace sf
{
    class Texture;
    class Font;
    class SoundBuffer;
}

namespace Textures {

    enum ID {

        Entities,
        Eagle,
        Space,
        TitleScreen,
        Buttons,
        Explosion,
        Particle,
        RockAsteroid,
        IceAsteroid,
        FireAsteroid
    };
}

namespace Fonts {

    enum ID {

        Main
    };
}

namespace SoundEffect {

    enum ID {


    };
}

template <typename Resource, typename Identifier>
class ResourceHolder;

typedef ResourceHolder<sf::Texture, Textures::ID>           TextureHolder;
typedef ResourceHolder<sf::Font, Fonts::ID>                 FontHolder;
typedef ResourceHolder<sf::SoundBuffer, SoundEffect::ID>    SoundBufferHolder;

#endif // RESOURCEIDENTIFIERS

