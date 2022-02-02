#ifndef CHEM_ENGINE_H
#define CHEM_ENGINE_H

#include "MogaEngine.h"

class ChemEngine : public MogaEngine {
    void reactions_process();
	void physics_tick() override final;

	bool to_compute_chemistry;

public:
    ChemEngine( const char  *window_name,
				const size_t screen_width,
				const size_t screen_height,
				const size_t pixel_size);

	inline void toggle_chemistry() { to_compute_chemistry ^= 1; }
	inline void chemistry_on() { to_compute_chemistry = 1; }
	inline void chemistry_off() { to_compute_chemistry = 0; }
};

#endif // CHEM_ENGINE_H
