#if 0

  ќчень плодовитый автор ѕравдолюбов принЄс в издательство свою новую эпопею, от которой издательству отказатьс€
  не удалось (как ни т€жела ноша издательства, а адвокат у ѕравдолюбова был очень известным, что тут поделаешь).
  ¬ эпопее много глав (1<=N<=1000000), в каждой из которых было до 10000 страниц. »здать такой монументальный труд
  в виде одного тома оказалось никак невозможно, поэтому эпопею дл€ печати потребовалось разбить на тома.  ак обычно,
  каждый том содержит последовательные главы книги. Ѕыло решено издать сей продукт творчества в K томах, причЄм
  размер каждого тома в страницах должен быть ограничен каким-то числом M.

  ¬аша задача - найти такое минимальное число M, чтобы при разбиении книги на тома размер тома не превосходил M и
  при этом потребовалось бы не более K томов.

  Input format
    K N

    V1 V2 ... VN

  Output format
    M

#endif

//=== INCLUDES ===============================================================

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <assert.h>
#include <time.h>

//=== GENERAL_LIB =============================================================

const int KCTF_DEBUG_MODE = 1; /// Just a mode for debugging

int           DEBUG_NUMBER = 1;   ///< Just an int for debugging
unsigned char DEBUG_LETTER = 'a'; //< Just a char for debugging

#define DEBUG_NUMBER_PRINT() printf("[deb] %d [deb]\n", DEBUG_NUMBER++);
#define DEBUG_LETTER_PRINT() printf("[deb] %c [deb]\n", DEBUG_LETTER++);

#define DEBUG_NUMBER_INCR() DEBUG_NUMBER++;
#define DEBUG_LETTER_INCR() DEBUG_LETTER++;

#define DEBUG if(KCTF_DEBUG_MODE)

//=============================================================================

long long find_minimal_pages(long long *pages_arr, long long chapter_count, long long max_books, long long max_pages) {
    long long l = 0;
    long long r = (long long) 1000000000;
    while (l + 1 < r) {
        long long m = (l + r) / 2;
        long long s = 0;
        long long cnt = 1;
        if (max_pages > m) {
            l = m;
            continue;
        }
        for (int i = 0; i < chapter_count; ++i) {
            s += pages_arr[i];
            if (s == m) {
                s = 0;
                cnt += 1 * (i != chapter_count - 1);
            } else if (s > m) {
                s = 0;
                cnt += 1;
                i -= 1;
            }
        }
        if (cnt > max_books) {
            l = m;
        } else {
            r = m;
        }
    }
    return r;
}

int main() {
    long long n = 0;
    long long k = 0;
    scanf("%lld %lld", &k, &n);
    long long *pages_arr = (long long *) calloc(n, sizeof(long long));
    int mx = 0;
    for (int i = 0; i < n; ++i) {
        scanf("%lld", &pages_arr[i]);
        mx = (pages_arr[i] > mx) ? pages_arr[i] : mx;
    }

    long long min_pages = find_minimal_pages(pages_arr, n, k, mx);

    printf("%lld\n", min_pages);

    free(pages_arr);

    return 0;
}

// O(n * logn)
