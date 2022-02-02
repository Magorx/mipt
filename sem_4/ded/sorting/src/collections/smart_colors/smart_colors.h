#ifndef SMART_COLORS_COLLECTION_H
#define SMART_COLORS_COLLECTION_H

#include "visual/color/smart_color.h"


class SmartColorSin : public SmartColor
{
	Color color;
	double ampl;
	double freq;
	double time_shift;
	double cur_time;

public:
	SmartColorSin(const Color color_,
				  const double freq_ = 1,
				  const double time_shift_ = 0,
				  const double ampl_ = 1);
	
	const Color rgb() override;

	virtual void tick(double dt, double time = 0) override;
};

class SmartColorBlip : public SmartColor
{
	Color  color;
	
	int    state;
	double time_on;
	double time_off;
	double time_cnt;

public:
	SmartColorBlip(const Color color_,
				   const double time_on_  = 1,
				   const double time_off_ = 1,
				   const int  init_state  = 1);

	const Color rgb() override;

	virtual void tick(double dt, double time = 0) override;
};


// class SmartColorTimedGradient : public SmartColor
// {
// 	std::vector<SmartColor*> colors;
// 	double time_span;
// 	// double cur_time;

// public:
// 	SmartColorTimedGradient(const std::vector<SmartColor*> &colors_, 
// 							const double time_span_);

// 	const Color rgb() override;
// };

// class SmartColorCheckered : public SmartColor
// {
// 	SmartColor* color_1;
// 	SmartColor* color_2;
// 	double w;
// 	double h;

// public:
// 	SmartColorCheckered(SmartColor* color_1_,
// 						SmartColor* color_2_, 
// 						const double w_,
// 						const double h_);

// 	const Color rgb() override;
// };

#endif // SMART_COLORS_COLLECTION_H