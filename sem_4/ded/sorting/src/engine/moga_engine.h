#ifndef MOGA_ENGINE_H
#define MOGA_ENGINE_H

#include <SFML/Graphics.hpp>

#include <cstdlib>
#include <cstdio>
#include <vector>
#include <string>
#include <cmath>
#include <ctime>
#include <thread>
#include <chrono>

#include "moga_engine_settings.h"
#include "physics/physics_engine.h"
#include "../visual/visual_engine.h"
#include "view/view.h"
#include "utils/vec2d.h"

#include "tickable.h"
#include "object.h"

#include "resource_manager.h"
#include "Resources.h"

#include "collections/Collections.h"

#include <vector>

class MogaEngine : Tickable
{
protected:
	std::vector<Tickable*> tickables;
	std::vector<Object*> objects;

	double current_time;
	double prev_tick_time;
	double physics_current_time;
	double dt;
	double physics_time_multiplier;

	time_t init_time;
    double fps_second_start;
    
    int frames_cnt;
    int fps;
    int pause_mode;

	bool fps_seconds_tick;

    Vec2d mouse_pos;
//  =====================

	const char  *name;
	const size_t screen_width;
	const size_t screen_height;

public:
	VisualEngine  *visual;
	PhysicsEngine *physics;
	AbstractView *main_view;
	MogaResourceManager resman;

	MogaEngine(	RWindow *window,
				const char  *name);
	
	virtual ~MogaEngine();

	bool add_tickable(Tickable *object);
	bool add_physics_tickable(PhysTickable *object);
	bool add_solid_body(SolidBody *object);
	bool add_renderable(Renderable *object);
	bool add_object(Object *object, bool is_collidable = true);
	bool add_view(AbstractView *view);

	virtual void frame_init_tick();
	virtual void visual_render_tick();
	virtual void physics_tick();
	virtual void tickable_tick();
	virtual void logic_tick();

	virtual void on_mouse_click(Vec2d click, Event::MouseButton button = Event::MouseButton::none);
	virtual void on_mouse_hover(Vec2d click);
	virtual void on_mouse_release(Vec2d click, Event::MouseButton button = Event::MouseButton::none);
	virtual void handle_events(sf::RenderWindow &window);

	virtual void everlasting_loop();

	void tick(const double dt = 0, const double current_time = 0) override;

	void clear_deleted_tickables();
	void clear_deleted_objects();

	inline void set_phys_time_mult(double mult) { physics_time_multiplier = mult; }

//  ===== get / set =====

	Vec3d  get_mouse_pos() const;

	inline double get_cur_time() const {
		return current_time;
	}
	
	inline bool fps_seconds_ticked() const  {
		return fps_seconds_tick;
	};

	inline size_t get_screen_width() const { return screen_width; }
	inline size_t get_screen_height() const { return screen_height; }

//  =====================

	inline Vec2d random_screen_pos() { return {vec3d_randdouble(0, screen_width), vec3d_randdouble(0, screen_height)}; }

	inline void centrize_view_body(ViewBody &body) {
		main_view->centrize(body);
	}

// ======================

	static RWindow *create_window(const char *name, Vec2d size);
	static RWindow *create_window(const char *name, bool fullscreen);

};

#endif // MOGA_ENGINE_H