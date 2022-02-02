#ifndef TICKABLE
#define TICKABLE

class Tickable
{
public:
	bool del_tickable;
	bool tickable_nonfree;

	Tickable():
	del_tickable(false),
	tickable_nonfree(false)
	{}

	virtual ~Tickable() {}

	virtual void tick(const double = 0, const double = 0) {}; // double dt, double absolute_time

	inline void del() { del_tickable = true; }
};

#endif // TICKABLE
