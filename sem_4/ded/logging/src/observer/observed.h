#pragma once


#include "operators.h"
#include "event_system/signal.h"

#include "utils/logger.h"


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
    static int max_tmp_id;

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

    int get_tmp_id() {
        int ret = max_tmp_id;
        ++max_tmp_id;
        return ret;
    }

    std::string get_tmp_name() {
        return "tmp" + std::to_string(get_tmp_id());
    }
};


template<typename T>
int ObservedPool<T>::max_id = 0;

template<typename T>
int ObservedPool<T>::max_tmp_id = 0;


#define OPERATOR_DEF_UNARY_PREF_SAME(op_sgn, op_code) \
Observed<T> operator op_sgn() const {                 \
    pool.on_operator(op_code, this, nullptr);         \
    auto ret = Observed<T>(op_sgn get_data(), pool.get_tmp_name());            \
    ret.set_history(#op_sgn + history);\
    return ret;\
}

#define OPERATOR_DEF_UNARY_PREF_SAME_REF(op_sgn, op_code)   \
Observed<T> &operator op_sgn() const {                      \
    pool.on_operator(op_code, this, nullptr);               \
    data = op_sgn get_data();                               \
    history = #op_sgn + std::string("(") + history + ")";                              \
    return *this;                                           \
}

#define OPERATOR_DEF_UNARY_POST_SAME(op_sgn, op_code) \
Observed<T> operator op_sgn(int) const {              \
    pool.on_operator(op_code, this, nullptr);         \
    auto ret= Observed<T>(get_data() op_sgn, pool.get_tmp_name());            \
    ret.set_history("(" + get_history() + ")" + #op_sgn); \
    return ret; \
}


#define OPERATOR_DEF_INPLACE(op_sgn, op_code)            \
Observed<T> &operator op_sgn(const Observed<T> &other) { \
    pool.on_operator(Operator::op_code, this, &other);   \
    data = get_data() op_sgn other.get_data(); \
    set_history(get_history() + " " +  #op_sgn + " " + other.get_history()); \
    return *this; \
}

#define OPERATOR_DEF_BINARY_SAME(op_sgn, op_code)                                   \
friend Observed<T> operator op_sgn(Observed<T> first, const Observed<T> &second) {  \
    first.pool.on_operator(Operator::op_code, &first, &second);                     \
    first.get_data() op_sgn##= second.get_data();                                   \
    first.set_history(first.get_history() + " " + #op_sgn + " " + second.get_history()); \
    return first;                                                                   \
}

#define OPERATOR_DEF_BINARY_TYPE(ret_type, op_sgn, op_code)                             \
friend ret_type operator op_sgn(const Observed<T> &first, const Observed<T> &second) {  \
   first.pool.on_operator(Operator::op_code, &first, &second);                          \
   return first.get_data() op_sgn second.get_data();                                    \
}


template<typename T>
class Observed {
    T data;
    ObservedPool<T> &pool;

    int id;

    std::string name;
    std::string history;

    using one_line_log_type = std::function<void (const Observed<T>&)>;
    static one_line_log_type one_line_log;

    static ObservedPool<T> default_pool;

    void set_default_ctor_history() {
        history = "$" + std::to_string(id);
    }

public:
    Observed() :
    data(), pool(default_pool),
    id(pool.get_unique_id())
    {
        set_default_ctor_history();
        pool.on_operator(Operator::ctor, this, nullptr);
    }

    Observed(const T &data, const std::string &name="noname") :
    data(data), pool(default_pool),
    id(pool.get_unique_id()),
    name(name),
    history("")
    {
        set_default_ctor_history();
        pool.on_operator(Operator::ctor, this, nullptr);
    }

    Observed(const T &data, ObservedPool<T> &pool, const std::string &name="noname") :
    data(data), pool(pool),
    id(pool.get_unique_id()),
    name(name),
    history("")
    {
        set_default_ctor_history();
        pool.on_operator(Operator::ctor, this, nullptr);
    }

    Observed(const Observed<T> &other) :
    data(other.get_data()), pool(other.get_pool()),
    id(pool.get_unique_id()),
    name(pool.get_tmp_name()),
    history("{" + other.get_history() + "}")
    {
        pool.on_operator(Operator::ctor_copy, this, &other);
    }

    Observed<T> &operator=(const Observed<T> &other) {
        history = "=(" + other.get_history() + ")";
        pool.on_operator(Operator::asgn_copy, this, &other);
        get_data() = other.get_data();
        return *this;
    }

    Observed(Observed<T> &&other) :
    data(std::move(other.get_data())), pool(other.get_pool()),
    id(pool.get_unique_id()),
    name(pool.get_tmp_name()),
    history("&&(" + other.get_history() + ")")
    {
        pool.on_operator(Operator::ctor_move, this, &other);
    }

    Observed<T> &operator=(Observed<T> &&other) {
        history = "=&&(" + other.get_history() + ")";

        pool.on_operator(Operator::asgn_move, this, &other);
        get_data() = std::move(other.get_data());
        return *this;
    }

    #include "operators.dsl"

    inline T &operator()() const {
        return get_data();
    }

    inline T &operator()() {
        return get_data();
    }

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

    static void set_one_line_log(one_line_log_type oll) {
        one_line_log = oll;
    }

    void log() const {
        if (one_line_log) {
            one_line_log(*this);
        }
    }

    void logger_log(bool to_show_history=false) const {
        printf("%s<%d>[%p] | ", name.c_str(), id, this);
        log();

        if (to_show_history) {
            printf(" | hist: %s", history.c_str());
        }

        printf("\n");
    }

    void set_name(const std::string &new_name) {
        name = new_name;
    }

    const std::string &get_name() const {
        return name;
    }

    void set_history(const std::string &new_history) {
        history = new_history;
    }

    const std::string &get_history() const {
        return history;
    }
};

template<typename T>
ObservedPool<T> Observed<T>::default_pool = {};

template<typename T>
Observed<T>::one_line_log_type Observed<T>::one_line_log = nullptr;

#undef OPERATOR_DEF_UNARY_PREF_SAME
#undef OPERATOR_DEF_UNARY_PREF_SAME_REF
#undef OPERATOR_DEF_UNARY_POST_SAME
#undef OPERATOR_DEF_INPLACE
#undef OPERATOR_DEF_BINARY_SAME
#undef OPERATOR_DEF_BINARY_TYPE


//============================================================================= Observed<T>

