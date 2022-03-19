#pragma once


template <
          char S=' ',
          char E='\n'
         >
void print() {
    std::cout << E;
}

template <
          char S=' ',
          char E='\n',
          typename T,
          typename ... Ts
         >
void print(T&& a, Ts&& ... vs) {
    std::cout << std::forward<T>(a);
    if constexpr (sizeof...(Ts) != 0) { std::cout << S; }
    print<S, E>(std::forward<Ts>(vs)...);
}
