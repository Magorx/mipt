#include "visual_engine.h"


bool layer_ascending_order(const Renderable *first, const Renderable *second)
{
    // printf("%d %d\n", first->layer, second->layer);
    return first->layer < second->layer;
}


VisualEngine::VisualEngine(RWindow *window):
renderer(new Renderer(window))
{}

VisualEngine::~VisualEngine() {
    delete renderer;
    for (auto rend : render_objects) {
        delete rend;
    }
}

bool VisualEngine::add_renderable(Renderable *object) {
    render_objects.push_back(object);
    return true;
}

bool VisualEngine::render() {
    // sort(render_objects.begin(), render_objects.end(), layer_ascending_order);

    const size_t obj_cnt = render_objects.size();

    int deleted_cnt = 0;
    for (size_t i = 0; i < obj_cnt; ++i) {
        if (render_objects[i]->del_render) {
            ++deleted_cnt;
            continue;
        }

        render_objects[i]->render(renderer);
    }

    if (deleted_cnt && deleted_cnt > render_objects.size() * RENDERABLE_BUFFER_REFRESH_COEF) {
        clear_deleted_renderables();
    }

    return true;
}

void VisualEngine::clear_deleted_renderables() {
    size_t next_alive = 0;
    size_t render_objects_cnt = render_objects.size();
    size_t i = 0;

    for (; i < render_objects_cnt; ++i) {
        if (!render_objects[i] || render_objects[i]->del_render) {
            for (next_alive = std::max(i + 1, next_alive);
                next_alive < render_objects_cnt && render_objects[next_alive] && render_objects[next_alive]->del_render;
                ++next_alive); // find next alive object

            if (next_alive < render_objects_cnt) {
                if (render_objects[i]) {
                    delete render_objects[i];
                }

                render_objects[i] = render_objects[next_alive];
                render_objects[next_alive] = nullptr;
                ++next_alive;
            } else {
                break;
            }
        }
    }

    render_objects.resize(i);
}

void VisualEngine::tick(const double, const double) {
    renderer->clear();
    render();
    // renderer->display();
}
