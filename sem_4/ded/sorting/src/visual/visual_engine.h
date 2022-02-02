#ifndef VISUAL_ENGINE
#define VISUAL_ENGINE

#include "engine/tickable.h"

#include "visual_engine_settings.h"
#include "renderer/renderer.h"
#include "renderer/renderable.h"

#include <vector>
#include <algorithm>

class VisualEngine : Tickable
{
	Renderer *renderer;

	std::vector<Renderable*> render_objects;

public:

	VisualEngine(RWindow *window);

	~VisualEngine();

	inline Renderer *get_renderer() {
		return renderer;
	}

	bool add_renderable(Renderable *object);

	bool render();

	void clear_deleted_renderables();

	void tick(const double = 0, const double = 0) override;
};

#endif // VISUAL_ENGINE