#include "gate_type_aliases.h"


namespace RBlend {
    const RBlendMode none = sf::BlendNone;
    const RBlendMode add = sf::BlendAdd;
    const RBlendMode alpha = sf::BlendAlpha;
    const RBlendMode multiply = sf::BlendMultiply;

    const RBlendMode me_add = sf::BlendMode(sf::BlendMode::Factor::One, sf::BlendMode::Factor::One, sf::BlendMode::Equation::Add, sf::BlendMode::Factor::One, sf::BlendMode::Factor::One, sf::BlendMode::Equation::Add);
    const RBlendMode me_sub = sf::BlendMode(sf::BlendMode::Factor::One, sf::BlendMode::Factor::One, sf::BlendMode::Subtract, sf::BlendMode::Factor::One, sf::BlendMode::Factor::One, sf::BlendMode::Subtract);
}