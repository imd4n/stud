/*
 * Проверка шести вариантов сортировки из lab2.c сравнением с qsort.
 *
 * Схема: 6 алгоритмов x 3 типа входа x 335 массивов = 6030 проверок.
 * Типы входа: 0 — случайный, 1 — возрастающий, 2 — убывающий.
 * Размер каждого массива случаен от 1 до 64; для каждого алгоритма
 * берётся копия одной и той же последовательности.
 *
 * lab2.c включается текстом, чтобы тестировать те же самые функции,
 * которые используются в лабораторной (а не их копию).
 */
#define main lab2_main_disabled
#include "lab2.c"
#undef main

#include <string.h>
#include <locale.h>

#define METHODS 6
#define TYPES 3
#define REPEATS 335
#define MAX_N 64

#if METHODS * TYPES * REPEATS != 6030
#error "Схема теста должна давать ровно 6030 проверок"
#endif

static int cmp_int(const void* lhs, const void* rhs)
{
    int a = *(const int*)lhs;
    int b = *(const int*)rhs;
    return (a > b) - (a < b);
}

int main(void)
{
    setlocale(LC_ALL, ".UTF-8");

    int original[MAX_N];
    int actual[MAX_N];
    int expected[MAX_N];
    int buffer[MAX_N];
    long long checks = 0;
    long long failures = 0;

    srand(2026);

    for (int type = 0; type < TYPES; ++type)
    {
        for (int rep = 0; rep < REPEATS; ++rep)
        {
            int n = 1 + rand() % MAX_N;
            generateArray(original, n, type);

            memcpy(expected, original, (size_t)n * sizeof(int));
            qsort(expected, (size_t)n, sizeof(int), cmp_int);

            for (int method = 0; method < METHODS; ++method)
            {
                memcpy(actual, original, (size_t)n * sizeof(int));
                counter1 = 0;
                counter2 = 0;
                step = 0;
                arraySize = n;
                sortArray(actual, buffer, n, method);

                ++checks;
                if (memcmp(actual, expected, (size_t)n * sizeof(int)) != 0)
                {
                    ++failures;
                    printf("FAIL: type=%d rep=%d method=%d n=%d\n",
                           type, rep, method, n);
                }
            }
        }
    }

    printf("Проверок: %lld, ошибок: %lld\n", checks, failures);
    if (failures == 0)
    {
        printf("Все %lld проверок пройдены: результат каждого алгоритма совпадает с qsort.\n",
               checks);
        return 0;
    }
    return 1;
}
