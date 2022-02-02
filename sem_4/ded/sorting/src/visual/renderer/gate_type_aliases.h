#pragma once

#include <SFML/Graphics.hpp>

#include "utils/vec2d.h"
#include "visual/color/color.h"

using RMode = sf::RenderStates;
using RVertex = sf::Vertex;
using RColor = RGBA;
using RTexture = sf::Texture;
using RRendTexture = sf::RenderTexture;
using RFont = sf::Font;
using RState = sf::RenderStates;
using RShader = sf::Shader;
using RSprite = sf::Sprite;
using RImage = sf::Image;
using RWindow = sf::RenderWindow;

using RBlendMode = sf::BlendMode;

namespace RBlend {
    const extern RBlendMode none;
    const extern RBlendMode add;
    const extern RBlendMode alpha;
    const extern RBlendMode multiply;
    
    const extern RBlendMode me_add;
    const extern RBlendMode me_sub;
}
