#pragma once


#include "operators.h"

#include "event_system/signal.h"


template<typename T>
class Observed;


template<typename T>
struct OperatorSignal {
    Operator op;
    size_t op_cnt;
    const Observed<T> *first;
    const Observed<T> *second;
};


template<typename T>
class ObservedPool : public SignalDispatcher<OperatorSignal<T>> {
    size_t op_count[Operator::MAX];
    
    static int max_id;

public:
    ObservedPool() {}

    void on_operator(Operator op, const Observed<T> *first, const Observed<T> *second) {    
        op_count[op]++;

        SignalDispatcher<OperatorSignal<T>>::emit({
            op,
            op_count[op],
            first,
            second
        });
    }

    int get_unique_id() {
        int ret = max_id;
        ++max_id;
        return ret;
    }
};


template<typename T>
int ObservedPool<T>::max_id = 0;


#define OPERATOR_DEF_UNARY_PREF_SAME(op_sgn, op_code) \
Observed<T> operator op_sgn() const {                 \
    pool.on_operator(op_code, this, nullptr);         \
    return Observed<T>(op_sgn get_data());            \
}

#define OPERATOR_DEF_UNARY_PREF_SAME_REF(op_sgn, op_code)   \
Observed<T> &operator op_sgn() const {                      \
    pool.on_operator(op_code, this, nullptr);               \
    data = op_sgn get_data();                               \
    return *this;                                           \
}

#define OPERATOR_DEF_UNARY_POST_SAME(op_sgn, op_code) \
Observed<T> operator op_sgn(int) const {              \
    pool.on_operator(op_code, this, nullptr);         \
    return Observed<T>(get_data() op_sgn);            \
}


#define OPERATOR_DEF_INPLACE(op_sgn, op_code)            \
Observed<T> &operator op_sgn(const Observed<T> &other) { \
    pool.on_operator(Operator::op_code, this, &other);   \
    get_data() op_sgn other.get_data();                  \
}

#define OPERATOR_DEF_BINARY_SAME(op_sgn, op_code)                                   \
friend Observed<T> operator op_sgn(Observed<T> first, const Observed<T> &second) {  \
    first.pool.on_operator(Operator::op_code, &first, &second);                     \
    first.get_data() op_sgn##= second.get_data();                                   \
    return first;                                                                   \
}

#define OPERATOR_DEF_BINARY_TYPE(ret_type, op_sgn, op_code)                             \
friend ret_type operator op_sgn(const Observed<T> &first, const Observed<T> &second) {  \
   first.pool.on_operator(Operator::op_code, &first, &second);                          \
   return first.get_data() op_sgn second.get_data();              \
}


template<typename T>
class Observed {
    T data;
    ObservedPool<T> &pool;

    int id;

    static ObservedPool<T> default_pool;

public:
    Observed() :
    data(), pool(default_pool),
    id(pool.get_unique_id())
    {
        pool.on_operator(Operator::ctor, this, nullptr);
    }

    Observed(const T &data) :
    data(data), pool(default_pool),
    id(pool.get_unique_id())
    {
        pool.on_operator(Operator::ctor, this, nullptr);
    }

    Observed(const T &data, ObservedPool<T> &pool) :
    data(data), pool(pool),
    id(pool.get_unique_id())
    {
        pool.on_operator(Operator::ctor, this, nullptr);
    }

    Observed(const Observed<T> &other) :
    data(other.get_data()), pool(other.get_pool()),
    id(pool.get_unique_id())
    {
        pool.on_operator(Operator::ctor_copy, this, &other);
    }

    Observed<T> &operator=(const Observed<T> &other) {
        pool.on_operator(Operator::asgn_copy, this, &other);
        get_data() = other.get_data();
        return *this;
    }

    Observed(Observed<T> &&other) :
    data(std::move(other.get_data())), pool(other.get_pool()),
    id(pool.get_unique_id())
    {
        pool.on_operator(Operator::ctor_move, this, &other);
    }

    Observed<T> &operator=(Observed<T> &&other) {
        pool.on_operator(Operator::asgn_move, this, &other);
        get_data() = std::move(other.get_data());
        return *this;
    }

    #include "operators.dsl"
    

    inline ObservedPool<T> &get_pool() const {
        return pool;
    }

    inline T &get_data() {
        return data;
    }

    inline const T &get_data() const {
        return data;
    }

    inline int get_id() const {
        return id;
    }

    static ObservedPool<T> &get_default_pool() { return default_pool; }
};

template<typename T>
ObservedPool<T> Observed<T>::default_pool = {};

#undef OPERATOR_DEF_UNARY_PREF_SAME
#undef OPERATOR_DEF_UNARY_PREF_SAME_REF
#undef OPERATOR_DEF_UNARY_POST_SAME
#undef OPERATOR_DEF_INPLACE
#undef OPERATOR_DEF_BINARY_SAME
#undef OPERATOR_DEF_BINARY_TYPE


//============================================================================= Observed<T>

