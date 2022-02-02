#ifndef PHYS_TICKABLE_H
#define PHYS_TICKABLE_H

class PhysTickable
{
public:
	bool del_phys_tickable;

	PhysTickable():
	del_phys_tickable(true)
	{}

	virtual ~PhysTickable() {}

	virtual void physics_tick(const double dt) = 0;

	inline void del() { del_phys_tickable = true; }
};

#endif // PHYS_TICKABLE_H
