#ifndef PHYSICS_ENGINE
#define PHYSICS_ENGINE

#include "physics_engine_settings.h"

#include "engine/tickable.h"
#include "phys_tickable.h"
#include "solid_body/solid_body.h"

#include <vector>

class PhysicsEngine : Tickable
{
	std::vector<PhysTickable*> tickables;
	std::vector<SolidBody*> solids;
	std::vector<Collision*> collisions;

public:
	PhysicsEngine();
	~PhysicsEngine();

	bool add_phys_tickable(PhysTickable *object);
	bool add_solid(SolidBody *object);

	// void global_affects(const double dt);

	void physics_tick(const double dt);
	void tick(const double dt, const double time);

	void update_collisions();
	void solve_collisions();

	std::vector<Collision*> &get_collisions() { return collisions; }
};

#endif // PHYSICS_ENGINE
