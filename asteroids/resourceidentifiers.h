#ifndef RESOURCEIDENTIFIERS
#define RESOURCEIDENTIFIERS

namespace sf
{
    class Texture;
    class Font;
    class SoundBuffer;
    class Shader;
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

namespace Shaders {

    enum ID {
        BrightnessPass,
        DownSamplePass,
        GaussianBlurPass,
        AddPass,
    };
}

namespace SoundEffect {

    enum ID {

        AlliedGunfire,
        Explosion1,
        Explosion2,
        Button,
    };
}

namespace Music
{
    enum ID
    {
        MenuTheme,
        MissionTheme,
    };
}

template <typename Resource, typename Identifier>
class ResourceHolder;

typedef ResourceHolder<sf::Texture, Textures::ID>           TextureHolder;
typedef ResourceHolder<sf::Font, Fonts::ID>                 FontHolder;
typedef ResourceHolder<sf::SoundBuffer, SoundEffect::ID>    SoundBufferHolder;
typedef ResourceHolder<sf::Shader, Shaders::ID>             ShaderHolder;

#endif // RESOURCEIDENTIFIERS

