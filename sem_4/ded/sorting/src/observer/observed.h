#pragma once


#include "operators.h"

#include "event_system/signal.h"


template<typename T>
class ObservedPool;


template<typename T>
class Observed {
    T data;

    ObservedPool<T> &pool;

public:
    Observed(const T &data, ObservedPool<T> &pool);

    Observed operator+(const Observed &other) const;

    const T &get_data() const;
};


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

public:
    ObservedPool() {}

    void on_operator(Operator op, const Observed<T> *first, const Observed<T> *second);
};


//============================================================================= Observed<T>


template<typename T>
Observed<T>::Observed(const T &data, ObservedPool<T> &pool) : data(data), pool(pool) {}

template<typename T>
Observed<T> Observed<T>::operator+(const Observed<T> &other) const {
    pool.on_operator(Operator::add, this, &other);
    return Observed(data + other.get_data(), pool);
}

template<typename T>
const T &Observed<T>::get_data() const {
    return data;
}


//============================================================================= ObservedPool<T>


template<typename T>
void ObservedPool<T>::on_operator(Operator op, const Observed<T> *first, const Observed<T> *second) {    
    op_count[op]++;

    SignalDispatcher<OperatorSignal<T>>::emit({
        op,
        op_count[op],
        first,
        second
    });
}
