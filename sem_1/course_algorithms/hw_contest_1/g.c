/*
Герой по имени Магина сражается с группой из monsters_count монстров с помощью легендарного топора, известного как Ярость Битвы. Каждый из монстров имеет ai очков здоровья. Каждым ударом топора Магина уменьшает здоровье того, кого он ударил, на hero_main_power очков, при этом уменьшая здоровье всех остальных монстров на hero_secondary_power очков. Монстр умирает, когда у него остается 0 или менее очков здоровья. Магина хочет при каждом ударе выбирать цель таким образом, чтобы убить всех монстров за минимальное количество ударов. Требуется определить это количество.

Входные данные
В первой строке содержатся три целых числа через пробел: monsters_count, hero_main_power и hero_secondary_power (1⩽monsters_count⩽200000, 1⩽hero_secondary_power⩽hero_main_power⩽109) — количество монстров, урон по цели и урон по всем остальным соответственно.

Во второй строке содержатся monsters_count целых чисел через пробел: ai (1⩽ai⩽109) — количество очков здоровья у каждого из монстров.

Выходные данные
Выведите единственное целое число — минимальное количество ударов, за которое Магина сможет убить всех монстров.
*/

#include <stdio.h>
#include <stdlib.h>

char can_kill(const long long *arr, const long long monsters_count, const long long hero_secondary_power,
              const long long hero_main_power, long long hits) {
    long long init_hits = hits;
    long long power_delta = hero_main_power - hero_secondary_power;
    for (int i = 0; i < monsters_count; ++i) {
        long long hp_left = arr[i] - hero_secondary_power * init_hits;
        if (power_delta == 0 && hp_left > 0) {
            return 0;
        } else  {
            if (hp_left > 0) {
                hits -= (hp_left + power_delta - 1) / power_delta;
            }
        }
    }

    return power_delta * hits >= 0;
}

long long TheBattleOfTwoArmies(const long long *arr, const long long monsters_count, const long long hero_secondary_power, const long long hero_main_power) {
    long long l = 0;
    long long r = 10000000001;
    while (l + 1 < r) {
        long long m = (l + r) / 2;
        if (can_kill(arr, monsters_count, hero_secondary_power, hero_main_power, m)) {
            r = m;
        } else {
            l = m;
        }
    }

    return r;
}

int main() {
    long long monsters_count, hero_main_power, hero_secondary_power;
    scanf("%lld %lld %lld", &monsters_count, &hero_main_power, &hero_secondary_power);

    long long *arr = calloc(monsters_count, sizeof(long long));
    for (int i = 0; i < monsters_count; ++i) {
        scanf("%lld", &arr[i]);
    }

    printf("%lld\n", TheBattleOfTwoArmies(arr, monsters_count, hero_secondary_power, hero_main_power));

    free(arr);

    return 0;
}

// O(nlogn) - binsearch
