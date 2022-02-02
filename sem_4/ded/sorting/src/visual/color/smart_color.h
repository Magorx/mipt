#ifndef SMART_COLOR
#define SMART_COLOR

#include "color.h"
#include "engine/tickable.h"

#include <vector>

class SmartColor : public Tickable {
	Color color;

public:
	SmartColor();
	SmartColor(const Color &color_);
	SmartColor(const RGBA  &rgba);

	virtual const Color rgb();

	void set_rgb(const Color color_);
	void set_rgb(const double r, const double g, const double b);

	virtual void tick(const double = 0, const double = 0) override;
};

#endif // SMART_COLOR
