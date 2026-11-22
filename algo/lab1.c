#include <stdio.h>
#include <stdlib.h>
#include <time.h> // библиотека для работы с временем
#include <locale.h> // библиотека для работы вывода на русском

// Функции формирования массивов

// Функция формирования случайного массива
void generateRandomArray(int* arr, int size, int min_val, int max_val)
{
    for (int i = 0; i < size; ++i)
    {
        arr[i] = min_val + rand() % (max_val - min_val + 1); // генерация случайного числа в диапазоне [min_val, max_val]
    } // for
} // generateRandomArray

// Функция формирования возрастающего массива
void generateAscendingArray(int* arr, int size, int min_val, int max_val)
{
    double step = (double)(max_val - min_val) / size; // вычисление шага
    for (int i = 0; i < size; ++i)
    {
        arr[i] = min_val + (int)(i * step); // заполнение массива
    } // for
} // generateAscendingArray

// Функция формирования убывающего массива
void generateDescendingArray(int* arr, int size, int min_val, int max_val)
{
    double step = (double)(max_val - min_val) / size; // вычисление шага
    for (int i = 0; i < size; ++i)
    {
        arr[i] = max_val - (int)(i * step); // заполнение массива
    } // for
} // generateDescendingArray


// Алгоритмы поиска (BLS, SLS, BS)

// Бинарный поиск
int BLS(int* arr, int n, int key, long long* counter1, long long* counter2)
{
    *counter1 = 0; // счетчик операций сравнения
    *counter2 = 0; // счетчик операций доступа к элементам
    for (int i = 0; i < n; ++i)
    {
        (*counter2)++; // проверка границ массива в цикле
        (*counter1)++; // сравнение с ключом
        if (arr[i] == key) return i; // найден элемент
    } // for
    (*counter2)++; // проверка границ массива после цикла
    return -1;
} // BLS

// Линейный поиск
int SLS(int* arr, int n, int key, long long* counter1, long long* counter2)
{
    *counter1 = 0; // счетчик операций сравнения
    *counter2 = 0; // счетчик операций доступа к элементам

    int last = arr[n - 1]; // сохранение последнего элемента
    arr[n - 1] = key; // установка ключа в конец массива
    int i = 0; // индекс для обхода массива

    while (arr[i] != key)
    {
        (*counter1)++; // сравнение с ключом
        i++; // переход к следующему элементу
    }
    (*counter1)++; // сравнение с ключом для выхода из цикла

    arr[n - 1] = last; // восстановление последнего элемента

    (*counter1)++; // сравнение с ключом для выхода из цикла
    if (i < n - 1 || last == key)
    {
        return i;
    } // if
    return -1;
} // SLS

// Бинарный поиск
int BS(int* arr, int n, int key, long long* counter1, long long* counter2)
{
    *counter1 = 0; // счетчик операций сравнения
    *counter2 = 0; // счетчик операций доступа к элементам
    int left = 0, right = n - 1; // границы поиска

    while (left <= right)
    {
        (*counter2)++; // аналог проверки границ цикла
        int mid = left + (right - left) / 2; // индекс среднего элемента

        (*counter1)++; // сравнение с ключом
        if (arr[mid] == key) return mid; // найден элемент

        (*counter1)++; // сравнение с ключом
        if (arr[mid] < key) left = mid + 1; // сдвиг границы поиска
        else right = mid - 1;
    }
    (*counter2)++;
    return -1;
} // BS


// Вспомогательная функция замера времени
void testSearch(const char* algName, int(*searchFunc)(int*, int, int, long long*, long long*),
                int* arr, int size, int key)
{
    long long c1 = 0, c2 = 0; // счетчики операций(с1 - операции сравнения, с2 - операции доступа к элементам)
    clock_t start_time, end_time; // переменные для замера времени
    double cpu_time_used; // переменная для хранения времени

    start_time = clock(); // начало замера
    int index = searchFunc(arr, size, key, &c1, &c2); // вызов функции поиска
    end_time = clock(); // конец замера

    cpu_time_used = ((double) (end_time - start_time)) / CLOCKS_PER_SEC; // расчет времени

    printf("%-5s | Time: %f sec | C1: %-10lld | C2: %-10lld | %s\n",
           algName, cpu_time_used, c1, c2, (index != -1 ? "Found" : "Not Found")); // вывод результатов
} // testSearch

int main()
{
    setlocale(LC_ALL, ".UTF-8"); // меняем локаль на читаемую

    srand((unsigned int)time(NULL)); // инициализация генератора случайных чисел

    int sizes[] = {50000, 100000, 200000, 300000, 400000, 500000}; // размеры массивов для тестирования
    int numSizes = sizeof(sizes) / sizeof(sizes[0]); // количество размеров
    int min_val = 1, max_val = 1000000; // диапазон значений

    // Цикл по всем размерам массивов
    for (int s = 0; s < numSizes; s++)
    {
        int n = sizes[s]; // текущий размер массива

        // Выделение памяти вне функций
        int* randomArr = (int*)malloc(n * sizeof(int));
        int* ascendingArr = (int*)malloc(n * sizeof(int));
        int* descendingArr = (int*)malloc(n * sizeof(int));

        // Генерация массивов
        generateRandomArray(randomArr, n, min_val, max_val);
        generateAscendingArray(ascendingArr, n, min_val, max_val);
        generateDescendingArray(descendingArr, n, min_val, max_val);

        printf("\nРАЗМЕР МАССИВА: %d\n", n);

        // 1. В начале
        int keyStart = randomArr[0];
        int keyStartAsc = ascendingArr[0];
        printf(">>> ПОЗИЦИЯ: НАЧАЛО\n");
        testSearch("BLS", BLS, randomArr, n, keyStart);
        testSearch("SLS", SLS, randomArr, n, keyStart);
        testSearch("BS",  BS,  ascendingArr, n, keyStartAsc);

        // 2. В середине
        int keyMid = randomArr[(n -1) / 2];
        int keyMidAsc = ascendingArr[(n -1) / 2];
        printf(">>> ПОЗИЦИЯ: СЕРЕДИНА\n");
        testSearch("BLS", BLS, randomArr, n, keyMid);
        testSearch("SLS", SLS, randomArr, n, keyMid);
        testSearch("BS",  BS,  ascendingArr, n, keyMidAsc);

        // 3. Отсутствует
        int keyAbsent = -1; // Точно отсутствует, так как min_val = 1
        printf(">>> ПОЗИЦИЯ: ОТСУТСТВУЕТ\n");
        testSearch("BLS", BLS, randomArr, n, keyAbsent);
        testSearch("SLS", SLS, randomArr, n, keyAbsent);
        testSearch("BS",  BS,  ascendingArr, n, keyAbsent);

        free(randomArr);
        free(ascendingArr);
        free(descendingArr);
    } // for

    return 0;
} // main()
