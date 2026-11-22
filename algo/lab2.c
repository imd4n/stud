#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <locale.h>

// Общие счетчики: counter1 — сравнения, counter2 — пересылки.
// Они обнуляются перед запуском каждой сортировки.
long long counter1 = 0;
long long counter2 = 0;
int arraySize = 0; // полный размер массива для пошагового вывода
int step = 0;
FILE* steps = NULL;

// Формирование массивов, как в первой лабораторной.
void generateArray(int* a, int n, int type)
{
    int maximum = 1000000;
    if (n == 15) maximum = 99;
    if (n == 1)
    {
        a[0] = 1;
        return;
    }
    for (int i = 0; i < n; ++i)
    {
        if (type == 0) a[i] = 1 + rand() % maximum;
        if (type == 1)
            a[i] = 1 + (int)((long long)(maximum - 1) * i / (n - 1));
        if (type == 2)
            a[i] = maximum - (int)((long long)(maximum - 1) * i / (n - 1));
    }
}

// На больших массивах пошаговый вывод отключен.
void printArray(int* a)
{
    if (arraySize != 15 || steps == NULL) return;
    fprintf(steps, "%3d |", step);
    ++step;
    for (int i = 0; i < arraySize; ++i) fprintf(steps, " %d", a[i]);
    fprintf(steps, " | C=%lld M=%lld\n", counter1, counter2);
}

// Обмен с самим собой не выполняется; обычный обмен = 3 пересылки.
void swapElements(int* a, int* b)
{
    if (a == b) return;
    int t = *a;
    *a = *b;
    *b = t;
    counter2 += 3;
}

void selectionSort(int* a, int n)
{
    for (int i = 0; i < n - 1; ++i)
    {
        int smallest = i;
        for (int j = i + 1; j < n; ++j)
        {
            ++counter1;
            if (a[j] < a[smallest]) smallest = j;
        }
        swapElements(&a[i], &a[smallest]);
        printArray(a);
    }
}

void insertionSort(int* a, int n)
{
    for (int i = 1; i < n; ++i)
    {
        int key = a[i];
        ++counter2;
        int j = i - 1;
        while (j >= 0)
        {
            ++counter1;
            if (a[j] <= key) break;
            a[j + 1] = a[j];
            ++counter2;
            --j;
        }
        a[j + 1] = key;
        ++counter2;
        printArray(a);
    }
}

// Сначала сортируем левую и правую половины, затем сливаем их.
// left и right — индексы первого и последнего элементов участка.
void mergeSort(int* a, int* buffer, int left, int right)
{
    if (left >= right) return;
    int middle = (left + right) / 2;
    mergeSort(a, buffer, left, middle);
    mergeSort(a, buffer, middle + 1, right);

    int i = left;       // текущий элемент левой половины
    int j = middle + 1; // текущий элемент правой половины
    int k = left;       // место для записи в буфере
    while (i <= middle && j <= right)
    {
        ++counter1;
        if (a[i] <= a[j])
        {
            buffer[k] = a[i];
            ++i;
        }
        else
        {
            buffer[k] = a[j];
            ++j;
        }
        ++k;
        ++counter2;
    }
    // Копируем остаток той половины, в которой остались элементы.
    while (i <= middle)
    {
        buffer[k] = a[i];
        ++i;
        ++k;
        ++counter2;
    }
    while (j <= right)
    {
        buffer[k] = a[j];
        ++j;
        ++k;
        ++counter2;
    }
    // Возвращаем отсортированный участок из буфера в массив.
    for (k = left; k <= right; ++k)
    {
        a[k] = buffer[k];
        ++counter2;
    }
    printArray(a);
}

// Ломуто: опорный — последний элемент участка.
int partitionLast(int* a, int left, int right)
{
    int pivot = a[right];
    ++counter2;
    int i = left;
    for (int j = left; j < right; ++j)
    {
        ++counter1;
        if (a[j] <= pivot)
        {
            swapElements(&a[i], &a[j]);
            ++i;
        }
    }
    swapElements(&a[i], &a[right]);
    return i;
}

// Чтобы не получить 100000 вложенных вызовов, рекурсивно сортируем
// меньшую часть. Большую продолжаем сортировать в этом же цикле.
void quickLastSort(int* a, int left, int right)
{
    while (left < right)
    {
        int p = partitionLast(a, left, right);
        printArray(a);
        if (p - left < right - p)
        {
            quickLastSort(a, left, p - 1);
            left = p + 1;
        }
        else
        {
            quickLastSort(a, p + 1, right);
            right = p - 1;
        }
    }
}


// Схема Хоара: два встречных указателя; опорный из середины участка.
int partitionHoare(int* a, int left, int right)
{
    int pivot = a[left + (right - left) / 2];
    ++counter2;
    int i = left - 1, j = right + 1;
    for (;;)
    {
        do
        {
            ++i;
            ++counter1;
        } while (a[i] < pivot);
        do
        {
            --j;
            ++counter1;
        } while (a[j] > pivot);
        if (i >= j) return j;
        swapElements(&a[i], &a[j]);
    }
}

void quickHoareSort(int* a, int left, int right)
{
    if (left >= right) return;
    int p = partitionHoare(a, left, right);
    printArray(a);
    quickHoareSort(a, left, p);
    quickHoareSort(a, p + 1, right);
}

// Последовательность шагов: n/2, n/4, ..., 1.
void shellSort(int* a, int n)
{
    for (int gap = n / 2; gap > 0; gap /= 2)
    {
        if (arraySize == 15 && steps != NULL)
            fprintf(steps, "Шаг Шелла: %d\n", gap);
        for (int i = gap; i < n; ++i)
        {
            int key = a[i];
            ++counter2;
            int j = i;
            while (j >= gap)
            {
                ++counter1;
                if (a[j - gap] <= key) break;
                a[j] = a[j - gap];
                ++counter2;
                j -= gap;
            }
            a[j] = key;
            ++counter2;
            printArray(a);
        }
    }
}

// Номер нужен только для выбора функции, как пункта обычного меню.
void sortArray(int* a, int* buffer, int n, int method)
{
    switch (method)
    {
        case 0: selectionSort(a, n); break;
        case 1: insertionSort(a, n); break;
        case 2: mergeSort(a, buffer, 0, n - 1); break;
        case 3: quickLastSort(a, 0, n - 1); break;
        case 4: quickHoareSort(a, 0, n - 1); break;
        case 5: shellSort(a, n); break;
    }
}

// Проверка результата не включается во время сортировки.
int isSorted(int* a, int n)
{
    for (int i = 1; i < n; ++i)
        if (a[i - 1] > a[i]) return 0;
    return 1;
}

int main(void)
{
    setlocale(LC_ALL, ".UTF-8"); // меняем локаль на читаемую

    int sizes[] = {15, 500, 1000, 10000, 50000, 100000};
    const char* types[] = {"Случайный", "Возрастающий", "Убывающий"};
    const char* names[] = {"Выбором", "Вставками", "Слиянием",
        "Быстрая (последний)", "Быстрая (Хоар)", "Шелла"};

    // Три массива выделяются один раз, до начала эксперимента.
    int* original = malloc(100000 * sizeof(int));
    int* a = malloc(100000 * sizeof(int));
    int* buffer = malloc(100000 * sizeof(int));
    FILE* results = fopen("results.csv", "w");
    steps = fopen("steps.txt", "w");
    if (original == NULL || a == NULL || buffer == NULL ||
        results == NULL || steps == NULL)
    {
        printf("Не удалось выделить память или открыть файл.\n");
        free(original);
        free(a);
        free(buffer);
        if (results != NULL) fclose(results);
        if (steps != NULL) fclose(steps);
        return 1;
    }

    srand(42); // одинаковые исходные данные при повторном запуске
    fprintf(results, "n,type,algorithm,seconds,comparisons,moves,correct\n");
    for (int s = 0; s < 6; ++s)
    {
        int n = sizes[s];
        arraySize = n;
        for (int type = 0; type < 3; ++type)
        {
            generateArray(original, n, type);
            for (int method = 0; method < 6; ++method)
            {
                // Каждому методу передается одна и та же последовательность.
                for (int i = 0; i < n; ++i) a[i] = original[i];
                counter1 = 0;
                counter2 = 0;
                step = 0;
                if (n == 15)
                {
                    fprintf(steps, "\n%s / %s / n=15\n",
                        types[type], names[method]);
                    printArray(a);
                }

                printf("Выполняется: n=%d, %s, %s...\n",
                    n, types[type], names[method]);
                fflush(stdout); // показываем сообщение сразу, до сортировки
                clock_t start = clock();
                sortArray(a, buffer, n, method);
                double seconds = (double)(clock() - start) / CLOCKS_PER_SEC;
                int correct = isSorted(a, n);
                const char* status = correct ? "OK" : "FAIL";

                printf("n=%d | %s | %s | %.6f s | C=%lld M=%lld | %s\n",
                    n, types[type], names[method], seconds,
                    counter1, counter2, status);
                fprintf(results, "%d,%s,%s,%.9f,%lld,%lld,%s\n",
                    n, types[type], names[method], seconds,
                    counter1, counter2, status);
                if (n == 15)
                    fprintf(steps, "ИТОГО: C=%lld M=%lld; %s\n",
                        counter1, counter2, status);
                fflush(stdout);
                fflush(results);
                fflush(steps);
            }
        }
    }
    free(original);
    free(a);
    free(buffer);
    fclose(results);
    fclose(steps);
    return 0;
}
