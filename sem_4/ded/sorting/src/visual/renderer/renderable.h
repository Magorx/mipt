#ifndef RENDERABLE
#define RENDERABLE

#include "renderer.h"

class Renderable {
public:
	int layer;
	bool del_render;

	inline Renderable(int layer_ = 0):
	layer(layer_),
	del_render(false)
	{}

	virtual ~Renderable() {}

	inline void set_layer(const int layer_) {
		layer = layer_;
	}

	virtual void render(Renderer *) {};

	inline void del() { del_render = true; }
};

#endif // RENDERABLE
