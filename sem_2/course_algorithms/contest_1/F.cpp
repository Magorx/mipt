/*
Только что закончилась вечеринка в одном известном ночном клубе. На вечеринке была группа из n юношей и m девушек. Им было хорошо, но пришло время поехать домой. Все очень устали и уже довольно поздно, поэтому они решили ехать на такси. Так случилось, что все молодые люди живут на одной и той же улице, и клуб расположен на той же самой улице. Более того клуб расположен ровно в начале улице, т.е. всем ехать в одну и ту же сторону.

Таким образом можно сэкономить много денег.

В одно такси помещаются 4 человека. Таксист доедет до дома того человека, что живет дальше всех, высаживая остальных пассажиров по дороге. Оплата такси — 1 у.е. за километр. Местные таксисты не так просты, поэтому молодые люди решили, что в каждом такси должен поехать хотя бы один юноша (но не обязательно, чтобы он ехал в такси до самого конца, т.е. может выйти не последним).

Найдите способ для молодых людей разделиться на группы и сесть в такси таким образом, что суммарная стоимость всех такси будет минимальна.
*/

#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

using namespace std;

int inf = 99999999;

struct Chelik {
    string name;
    int x;
};

struct Pair {
    int x;
    int y;
};

bool operator<(const Chelik &a, const Chelik &b) {
    return a.x < b.x;
}

Pair **Pairpupa(int n, int m) {
    Pair **ret = (Pair**) calloc(m, sizeof(Pair*));
    for (int i = 0; i < m; ++i) {
        ret[i] = (Pair*) calloc(n, sizeof(Pair));
    }
    return ret;
}
 
int main() {
    int n = 0;
    int m = 0;

    scanf("%d", &n);
    vector<Chelik> boys;
    boys.push_back({"", 0});
    for (int i = 0; i < n; ++i) {
        char s[16];
        int x;
        scanf("%s %d", s, &x);
        boys.push_back({string(s), x});
    }

    scanf("%d", &m);
    vector<Chelik> girls;
    girls.push_back({"", 0});
    for (int i = 0; i < m; ++i) {
        char s[16];
        int x;
        scanf("%s %d", s, &x);
        girls.push_back({string(s), x});
    }

    sort(boys.begin(),   boys.end());
    sort(girls.begin(), girls.end());

    vector<vector<Pair>> dp(m + 1, vector<Pair>(n + 1, {+inf, 0}));
    vector<vector<Pair>> p (m + 1, vector<Pair>(n + 1, {0,    0}));
    dp[0][0].x = 0;

    for (int i = 0; i <= m; ++i) {
        for (int j = 1; j <= n; ++j) {
            for (int k = 1; k <= 4; ++k) {
                for (int l = 0; l <= 4 - k; ++l) {
                    if (k > j || l > i) {
                        break;
                    }

                    int cur = dp[i - l][j - k].x + max(boys[j].x, girls[i].x);
                    if (dp[i][j].x > cur) {
                        dp[i][j].x = cur;
                        dp[i][j].y = dp[i - l][j - k].y + 1;
                        p [i][j]   = {k, l};
                    }
                }
            }
        }
    }

    int i  = m;
    int j  = n;
    int tx = 0;

    printf("%d\n%d\n", dp[m][n].x, dp[m][n].y);

    while (tx < dp[m][n].y) {
        ++tx;
        printf("Taxi %d: ", tx);
        int bb = p[i][j].x;
        int gg = p[i][j].y;
        int cnt = bb + gg;
        
        for (int k = i; k > i - gg; k--) {
            printf("%s", girls[k].name.c_str());
            cnt--;
            if (cnt != 1) {
                printf(", ");
            } else {
                printf(" ");
            }
        }

        for (int k = j; k > j - bb + 1; k--) {
            printf("%s", boys[k].name.c_str());
            cnt--;
            if (cnt != 1) {
                printf(", ");
            } else {
                printf(" ");
            }
        }

        if (gg + bb != 1) {
            printf("and ");
        }

        printf("%s.\n", boys[j - bb + 1].name.c_str());

        i -= gg;
        j -= bb;
    }
 
    return 0;
}

// dp[i][j] - оптимальная стоимость и кол-во такси, если рассматривать первых i девочек и j мальчиков
// обновляем через рассматривание создание нового такси для очередного мальчика либо перепихивания
// его в более старые такси с учетом того, что более ранние ответы мы уже знаем
// O(n * m)