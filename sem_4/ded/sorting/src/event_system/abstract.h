#include "dispatcher_begin.h"


template <typename SUCC_T>
class AbstractEventSystem {
    SUCC_T *self;
protected:
    SUCC_T *es_parent;

    std::vector<SUCC_T*> sub_es;

public:
    int index_in_parent;
    bool to_delete;

    AbstractEventSystem(SUCC_T *self) :
    self(self),
    es_parent(nullptr),
    index_in_parent(0),
    to_delete(false)
    {}

    virtual ~AbstractEventSystem() {
        for (size_t i = 0; i < sub_es.size(); ++i) {
            if (sub_es[i]) sub_es[i]->set_es_parent(nullptr);
            delete sub_es[i];
        }
        delete_from_parent();
    }

    virtual void set_to_delete(bool to_delete_, bool = false) { to_delete = to_delete_; }

    void add_es(SUCC_T *sub_system) {
        if (!sub_system) return;

        sub_system->delete_from_parent();
        sub_system->set_es_parent(self);
        sub_system->index_in_parent = sub_es.size();
        sub_es.push_back(sub_system);
    }

    void delete_from_parent() {
        if (!es_parent) return;

        es_parent->delete_es(index_in_parent);
    }

    void delete_es(size_t sub_idx) {
        if (sub_idx >= sub_es.size()) {
            return;
        }

        sub_es.erase(sub_es.begin() + sub_idx);
        for (size_t i = sub_idx; i < sub_es.size(); ++i) {
            sub_es[i]->index_in_parent = i;
        }
    }

    const std::vector<SUCC_T*> &get_sub_es() {
        return sub_es;
    }

    void focus(size_t sub_idx) {
        if (sub_idx >= sub_es.size()) {
            return;
        }

        auto es = sub_es[sub_idx];
        for (int i = sub_idx; i > 0; --i) {
            sub_es[i] = sub_es[i - 1];
            sub_es[i]->index_in_parent = i;
        }
        sub_es[0] = es;
        es->index_in_parent = 0;
    }

    void focus() {
        if (es_parent) es_parent->focus(index_in_parent);
    }

    bool is_focused() {
        return (!es_parent || es_parent->is_focused(index_in_parent));
    }

    bool is_focused(int idx) {
        if (idx >= (int) sub_es.size()) return false;
        else return idx == 0;
    }

    inline void set_es_parent(SUCC_T *parent_) {
        es_parent = parent_;
    }
};
