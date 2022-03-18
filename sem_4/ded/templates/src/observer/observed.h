#pragma once


#include "operators.h"
#include "event_system/signal.h"

#include "utils/logger.h"

#include <map>
#include <string>


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

    static int max_addr_id;
    static std::map<const Observed<T>*, int> addr_map;

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
        int ret = ++max_id;
        return ret;
    }

    int get_tmp_id() {
        int ret = ++max_tmp_id;
        return ret;
    }

    std::string get_tmp_name() {
        return "TMP" + std::to_string(get_tmp_id());
    }

    int register_addr(const Observed<T>* addr) {
        if (!addr_map.contains(addr)) {
            addr_map[addr] = ++max_addr_id;
        }
        return addr_map[addr];
    }

    int get_addr_id(const Observed<T>* addr) const {
        if (!addr_map.contains(addr)) {
            return -1;
        } else {
            return addr_map[addr];
        }
    }
};


template<typename T>
int ObservedPool<T>::max_id = 0;

template<typename T>
int ObservedPool<T>::max_tmp_id = 0;


#define OPERATOR_DEF_UNARY_PREF_SAME(op_sgn, op_code) \
Observed<T> operator op_sgn() const {                 \
    pool.on_operator(op_code, this, nullptr);         \
    auto ret = Observed<T>(op_sgn get_data(), pool.get_tmp_name(), true);            \
    ret.set_history(#op_sgn + history);\
    pool.register_addr(&ret);\
    return ret;\
}

#define OPERATOR_DEF_UNARY_PREF_SAME_REF(op_sgn, op_code)   \
Observed<T> &operator op_sgn() {                      \
    pool.on_operator(op_code, this, nullptr);               \
    data = op_sgn get_data();                               \
    history = #op_sgn + std::string("(") + history + ")";                              \
    return *this;                                           \
}

#define OPERATOR_DEF_UNARY_POST_SAME(op_sgn, op_code) \
Observed<T> operator op_sgn(int) {              \
    pool.on_operator(op_code, this, nullptr);         \
    auto ret= Observed<T>(*this, pool.get_tmp_name(), true);            \
    get_data() op_sgn; \
    pool.register_addr(&ret);\
    set_history("(" + get_history() + ")" + #op_sgn);\
    return ret; \
}


#define OPERATOR_DEF_INPLACE(op_sgn, op_code)            \
Observed<T> &operator op_sgn(const Observed<T> &other) { \
    pool.on_operator(Operator::op_code, this, &other);   \
    data op_sgn other.get_data(); \
    set_history(get_history() + " " +  #op_sgn + " " + other.get_history()); \
    return *this; \
}

/*
#define OPERATOR_DEF_BINARY_SAME(op_sgn, op_code)                                   \
friend Observed<T> operator op_sgn(Observed<T> first, const Observed<T> &second) {  \
    first.pool.on_operator(Operator::op_code, &first, &second);                     \
    first.get_data() op_sgn##= second.get_data();                                   \
    first.set_history(first.get_history() + " " + #op_sgn + " " + second.get_history()); \
    return first;                                                                   \
}
*/

#define OPERATOR_DEF_BINARY_SAME(op_sgn, op_code)                                   \
friend Observed<T> operator op_sgn(const Observed<T> &first, const Observed<T> &second) {  \
    first.pool.on_operator(Operator::op_code, &first, &second);                     \
    Observed<T> ret(first.get_data() op_sgn second.get_data(), first.pool.get_tmp_name(), true); \
    ret.set_history(first.get_history() + " " + #op_sgn + " " + second.get_history()); \
    return ret;                                                                   \
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

    bool is_temp = false;

    using one_line_log_type = std::function<std::string (const Observed<T>&)>;
    using value_length_type = std::function<int (const Observed<T>&)>;
    static one_line_log_type one_line_log;
    static value_length_type value_length;

    static ObservedPool<T> default_pool;

    static int max_name_len;
    static int max_value_len;
    static int max_id_len;
    static int max_addr_len;
    static int max_addr_id_len;


    void set_default_ctor_history() {
        history = name + std::to_string(id);
    }

public:
    Observed() :
    data(), pool(default_pool),
    id(pool.get_unique_id())
    {
        pool.register_addr(this);
        set_default_ctor_history();

        pool.on_operator(Operator::ctor, this, nullptr);
    }

    Observed(const T &data, const std::string &name="@noname", bool is_temp=false) :
    data(data), pool(default_pool),
    id(pool.get_unique_id()),
    name(name),
    history(""),
    is_temp(is_temp)
    {
        pool.register_addr(this);
        set_default_ctor_history();

        pool.on_operator(Operator::ctor, this, nullptr);
    }

    Observed(const T &data, ObservedPool<T> &pool, const std::string &name="@noname", bool is_temp=false) :
    data(data), pool(pool),
    id(pool.get_unique_id()),
    name(name),
    history(""),
    is_temp(is_temp)
    {
        pool.register_addr(this);
        set_default_ctor_history();

        pool.on_operator(Operator::ctor, this, nullptr);
    }

    Observed(const Observed<T> &other, const std::string &name="@noname", bool is_temp=false) :
    data(other.get_data()), pool(other.get_pool()),
    id(pool.get_unique_id()),
    name(name[0] == '@' ? pool.get_tmp_name() : name),
    history("COPY{" + other.get_history() + "}"),
    is_temp(name[0] == '@' ?  true : is_temp)
    {
        pool.register_addr(this);
        pool.on_operator(Operator::ctor_copy, this, &other);
    }

    Observed<T> &operator=(const Observed<T> &other) {
        pool.register_addr(this);
        history = "COPY={" + other.get_history() + "}";

        get_data() = other.get_data();
        pool.on_operator(Operator::asgn_copy, this, &other);
        return *this;
    }

#ifndef NO_MOVE
    Observed(Observed<T> &&other) :
    data(std::move(other.get_data())), pool(other.get_pool()),
    id(pool.get_unique_id()),
    name(pool.get_tmp_name()),
    history("move(" + other.get_history() + ")"),
    is_temp(true)
    {
        pool.register_addr(this);
        pool.on_operator(Operator::ctor_move, this, &other);
    }

    Observed<T> &operator=(Observed<T> &&other) {
        pool.register_addr(this);
        history = "move=(" + other.get_history() + ")";

        get_data() = std::move(other.get_data());
        pool.on_operator(Operator::asgn_move, this, &other);
        return *this;
    }
#endif

    ~Observed() {
        pool.on_operator(Operator::dtor, this, nullptr);
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

    static void set_one_line_log(one_line_log_type new_one_line_log) {
        one_line_log = new_one_line_log;
    }

    static void set_value_length(value_length_type new_value_length) {
        value_length = new_value_length;
    }

    int get_value_length() {
        if (value_length) {
            return value_length(*this);
        } else {
            return -1;
        }
    }

    #define _UPD_MAX_VALUE_LEN(value_name, spec, value) \
    memset(str, 0, MAX_UNIT_LEN);\
    snprintf(str, MAX_UNIT_LEN, spec, value); \
    max_##value_name##_len = std::max(max_##value_name##_len, (int) strlen(str)); \
    memset(str, 0, MAX_UNIT_LEN); \

    void update_max_lens() const {
        const int MAX_UNIT_LEN = 100;
        char str[MAX_UNIT_LEN];

        _UPD_MAX_VALUE_LEN(id, "%d", id);
        _UPD_MAX_VALUE_LEN(name, "%s", name.c_str());
        _UPD_MAX_VALUE_LEN(addr, "%p", this);
        _UPD_MAX_VALUE_LEN(addr_id, "%d", pool.get_addr_id(this));

        if (value_length) {
            max_value_len = std::max(max_value_len, value_length(*this));
        }
    }

    #undef _UPD_MAX_VALUE_LEN

    #define _GET_MAX_VALUE_LEN(value_name) \
    int get_max_##value_name##_len() const { return max_##value_name##_len; }

    _GET_MAX_VALUE_LEN(id)
    _GET_MAX_VALUE_LEN(name)
    _GET_MAX_VALUE_LEN(addr)
    _GET_MAX_VALUE_LEN(value)

    std::string to_str() const {
        if (!one_line_log) {
            return "{no one_line_log set up}";
        }
        return one_line_log(*this);
    }

    void log(Logger &logger=kctf::logger) const {
        if (!one_line_log) {
            return;
        }
        auto str = one_line_log(*this);
        logger.print_aligned(Logger::Align::middle, get_max_value_len(), "%s", str.c_str());
    }

    void logger_log(Logger &logger=kctf::logger, bool to_show_history=false) const {
        update_max_lens();

        logger.print_aligned(Logger::Align::left, max_name_len, "%s", name.c_str());

        logger.print(" | [");
        logger.print_aligned(Logger::Align::middle, max_id_len, "var%d", id);
        logger.print("][");
        logger.print_aligned(Logger::Align::middle, max_addr_len, "%p", this);
        logger.print("][");
        logger.print_aligned(Logger::Align::middle, max_addr_id_len, "loc%d", pool.get_addr_id(this));
        logger.print("] | val = ");
        log(logger);

        if (to_show_history) {
            logger.print(" | %s", history.c_str());
        }

        logger.print("\n");
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

    inline bool is_tmp() const {
        return is_temp;
    }
};

template<typename T>
ObservedPool<T> Observed<T>::default_pool = {};

template<typename T>
typename Observed<T>::one_line_log_type Observed<T>::one_line_log = nullptr;

template<typename T>
typename Observed<T>::value_length_type Observed<T>::value_length = nullptr;

template<typename T>
int Observed<T>::max_name_len = 0;
template<typename T>
int Observed<T>::max_value_len = 0;
template<typename T>
int Observed<T>::max_id_len = 0;
template<typename T>
int Observed<T>::max_addr_len = 0;
template<typename T>
int Observed<T>::max_addr_id_len = 0;

template<typename T>
int ObservedPool<T>::max_addr_id = 0;

template<typename T>
std::map<const Observed<T>*, int> ObservedPool<T>::addr_map;

#undef OPERATOR_DEF_UNARY_PREF_SAME
#undef OPERATOR_DEF_UNARY_PREF_SAME_REF
#undef OPERATOR_DEF_UNARY_POST_SAME
#undef OPERATOR_DEF_INPLACE
#undef OPERATOR_DEF_BINARY_SAME
#undef OPERATOR_DEF_BINARY_TYPE


//============================================================================= Observed<T>

