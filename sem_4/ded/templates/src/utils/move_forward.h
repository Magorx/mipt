template<typename T>
struct my_remove_reference {
    using type = T;
};


template<typename T>
struct my_remove_reference<T&> {
    using type = T;
};


template<typename T>
struct my_remove_reference<T&&> {
    using type = T;
};


template<typename T> using my_remove_reference_t = typename my_remove_reference<T>::type;


template<typename T>
my_remove_reference_t<T>&& my_move(T &&obj) {
    return static_cast<my_remove_reference_t<T>&&>(obj);
}


template<typename T>
T&& my_forward(my_remove_reference_t<T> &obj) {
    return static_cast<T&&>(obj);
}

template<typename T>
T&& my_forward(my_remove_reference_t<T> &&obj) {
    return static_cast<T&&>(obj);
}