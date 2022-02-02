#ifndef CHEM_REACTIONS_H
#define CHEM_REACTIONS_H

#include "MogaEngine.h"

void reaction_circle_circle(Object *first, Object *second, MogaEngine *engine);
void reaction_circle_square(Object *first, Object *second, MogaEngine *engine);
void reaction_square_circle(Object *first, Object *second, MogaEngine *engine);
void reaction_square_square(Object *first, Object *second, MogaEngine *engine);

void reaction(Object *fisrt, Object *second, MogaEngine *engine);

#endif // CHEM_REACTIONS_H
