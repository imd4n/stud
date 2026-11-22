#include "graph.h"

// Конструктор пустого графа
Graph::Graph(bool t) {
    type = t;               // тип графа: true = неориентированный, false = ориентированный
    vertex_count = 0;       // количество вершин пока равно нулю
    adj = nullptr;          // массив списков смежности (двумерный динамический массив)
    sizes = nullptr;        // массив текущих размеров списков смежности
    capacities = nullptr;   // массив ёмкостей списков смежности (для динамического расширения)
}

// Деструктор графа
Graph::~Graph() {
    if (adj) {
        // Освобождаем память, выделенную под списки смежности каждой вершины
        for (int i = 0; i < vertex_count; ++i) {
            delete[] adj[i];
        }
        delete[] adj; // удаляем массив указателей
    }
    delete[] sizes;        // удаляем массив размеров
    delete[] capacities;   // удаляем массив ёмкостей
}

// Конструктор графа из файла
Graph::Graph(const char* Fname, bool t) {
    type = t;                // тип графа
    vertex_count = 0;
    adj = nullptr;
    sizes = nullptr;
    capacities = nullptr;
    vertex_numbers = nullptr; // массив для хранения "настоящих" номеров вершин

    ifstream fin(Fname);     // открываем файл
    if (!fin) {
        cerr << "Cannot open file!\n"; // ошибка при открытии файла
        exit(1);
    }

    int num_vertices;
    fin >> num_vertices; // считываем количество строк (вершин)

    // Построчно считываем список смежности
    for (int i = 0; i < num_vertices; ++i) {
        int neighbor, weight;
        int current_vertex = i; // временно используем индекс как номер вершины

        // Добавляем вершину в граф, если её ещё нет
        add_vertex(current_vertex);

        // Считываем список смежности для вершины
        while (fin >> neighbor) {
            if (neighbor == -1) break; // -1 = конец списка
            fin >> weight;             // вес ребра

            // Если соседней вершины нет — добавляем
            if (index_of(neighbor) == -1)
                add_vertex(neighbor);

            // Добавляем ребро
            add_edge(current_vertex, neighbor, weight);
        }
    }

    fin.close();
}

// Возвращает количество вершин графа
int Graph::size() {
    return vertex_count;
}

// Увеличение ёмкости массива рёбер для вершины v
void Graph::resize(int v) {
    capacities[v] *= 2;               // удваиваем ёмкость
    Edge* newArr = new Edge[capacities[v]]; // выделяем новый массив
    for (int i = 0; i < sizes[v]; ++i)
        newArr[i] = adj[v][i];        // копируем старые данные
    delete[] adj[v];                  // удаляем старый массив
    adj[v] = newArr;                  // заменяем новым
}

// Добавление новой вершины в граф
void Graph::add_vertex(int vertex) {
    if (index_of(vertex) != -1) return; // если вершина уже есть, выходим

    // Создаём новые массивы увеличенного размера
    Edge** newAdj = new Edge * [vertex_count + 1];
    int* newSizes = new int[vertex_count + 1];
    int* newCaps = new int[vertex_count + 1];
    int* newNumbers = new int[vertex_count + 1];

    // Копируем старые данные
    for (int i = 0; i < vertex_count; ++i) {
        newAdj[i] = adj[i];
        newSizes[i] = sizes[i];
        newCaps[i] = capacities[i];
        newNumbers[i] = vertex_numbers[i];
    }

    // Инициализируем новую вершину
    newSizes[vertex_count] = 0;
    newCaps[vertex_count] = 2;            // начальная ёмкость
    newAdj[vertex_count] = new Edge[2];   // выделяем массив рёбер
    newNumbers[vertex_count] = vertex;    // сохраняем "номер" вершины

    // Удаляем старые массивы
    delete[] adj;
    delete[] sizes;
    delete[] capacities;
    delete[] vertex_numbers;

    // Обновляем ссылки на новые массивы
    adj = newAdj;
    sizes = newSizes;
    capacities = newCaps;
    vertex_numbers = newNumbers;
    vertex_count++; // увеличиваем количество вершин
}

// Добавление ребра (v1 -> v2, вес w)
void Graph::add_edge(int v1, int v2, int w) {
    // Если места для ребра не хватает — увеличиваем ёмкость
    if (sizes[v1] == capacities[v1]) resize(v1);
    adj[v1][sizes[v1]++] = { v2, w }; // добавляем ребро

    // Если граф неориентированный, добавляем обратное ребро
    if (type && v1 != v2) {
        if (sizes[v2] == capacities[v2]) resize(v2);
        adj[v2][sizes[v2]++] = { v1, w };
    }
}

// Проверка наличия ребра v1 -> v2
bool Graph::is_edge(int v1, int v2) {
    for (int i = 0; i < sizes[v1]; ++i)
        if (adj[v1][i].to == v2) return true;
    return false;
}

// Получение веса ребра v1 -> v2
int Graph::get_weight(int v1, int v2) {
    for (int i = 0; i < sizes[v1]; ++i)
        if (adj[v1][i].to == v2) return adj[v1][i].weight;
    return -1; // если ребра нет
}

// Вывод всех рёбер графа
void Graph::list_of_edges() {
    for (int i = 0; i < vertex_count; ++i) {
        cout << "Vertex " << i << ": ";
        for (int j = 0; j < sizes[i]; ++j)
            cout << "(" << adj[i][j].to << "," << adj[i][j].weight << ") ";
        cout << endl;
    }
}

// Вывод рёбер конкретной вершины v
void Graph::list_of_edges(int v) {
    cout << "Vertex " << v << ": ";
    for (int i = 0; i < sizes[v]; ++i)
        cout << "(" << adj[v][i].to << "," << adj[v][i].weight << ") ";
    cout << endl;
}

// Сохранение графа в файл в виде списков смежности
void Graph::save_to_file(const char* Fname) {
    ofstream fout(Fname);
    if (!fout) {
        cerr << "варнинг save!" << endl;
        return;
    }

    fout << vertex_count << endl; // записываем количество вершин
    for (int i = 0; i < vertex_count; ++i) {
        for (int j = 0; j < sizes[i]; ++j)
            fout << adj[i][j].to << " " << adj[i][j].weight << " ";
        fout << -1 << endl; // -1 = конец списка смежности
    }
    fout.close();
}

// Поиск индекса вершины по её "настоящему номеру"
int Graph::index_of(int vertex) {
    for (int i = 0; i < vertex_count; ++i)
        if (vertex_numbers[i] == vertex) return i;
    return -1; // если не нашли
}
void Graph::dijkstra(int start_vertex) {
    const int INF = numeric_limits<int>::max();

    int n = vertex_count;
    int* dist = new int[n];
    bool* visited = new bool[n];

    for (int i = 0; i < n; i++) {
        dist[i] = INF;
        visited[i] = false;
    }

    dist[start_vertex] = 0;

    for (int i = 0; i < n; i++) {
        // выбираем вершину с минимальной дистанцией
        int v = -1;
        for (int j = 0; j < n; j++) {
            if (!visited[j] && (v == -1 || dist[j] < dist[v])) {
                v = j;
            }
        }

        if (dist[v] == INF) break; // оставшиеся вершины недостижимы
        visited[v] = true;

        // релаксация рёбер
        for (int k = 0; k < sizes[v]; k++) {
            int u = adj[v][k].to;       // сосед
            int w = adj[v][k].weight;   // вес
            if (dist[v] + w < dist[u]) {
                dist[u] = dist[v] + w;
            }
        }
    }

    // вывод результатов
    cout << "Кратчайшие расстояния от вершины " << start_vertex << ":\n";
    for (int i = 0; i < n; i++) {
        if (dist[i] == INF)
            cout << "До " << i << ": недостижимо\n";
        else
            cout << "До " << i << ": " << dist[i] << "\n";
    }

    delete[] dist;
    delete[] visited;
}


// Алгоритм Прима — нахождение минимального остовного дерева
// ------------------------------------------------------------
// Работает только для неориентированных графов (type == true).
// Идея алгоритма:
// 1. Начинаем с произвольной вершины (start_vertex).
// 2. На каждом шаге выбираем ребро с минимальным весом,
//    которое соединяет уже выбранные вершины с новыми.
// 3. Добавляем вершину, которую соединяет это ребро, в остовное дерево.
// 4. Повторяем, пока все вершины не будут добавлены.
// 
// Результат: множество рёбер, образующих минимальное по весу дерево,
//             которое соединяет все вершины графа.


void Graph::prim(int start_vertex) {
    // Проверяем, подходит ли тип графа
    if (!type) {
        cout << "Ошибка: Алгоритм Прима работает только для неориентированных графов!\n";
        return;
    }

    const int INF = numeric_limits<int>::max(); // бесконечность
    int n = vertex_count; // количество вершин

    // Вспомогательные массивы
    int* parent = new int[n];   // parent[v] — родитель вершины v в остовном дереве
    int* key = new int[n];      // key[v] — минимальный вес ребра для подключения вершины v
    bool* inMST = new bool[n];  // inMST[v] — включена ли вершина v в остовное дерево

    // Инициализация всех массивов
    for (int i = 0; i < n; i++) {
        key[i] = INF;       // изначально расстояния бесконечны
        inMST[i] = false;   // ни одна вершина не включена в дерево
        parent[i] = -1;     // у вершин нет родителей
    }

    key[start_vertex] = 0; // начинаем с заданной вершины

    // Основной цикл алгоритма: добавляем по одной вершине в остов
    for (int count = 0; count < n - 1; count++) {
        // Находим вершину u с минимальным значением key[u],
        // которая ещё не включена в остовное дерево
        int u = -1;
        for (int v = 0; v < n; v++) {
            if (!inMST[v] && (u == -1 || key[v] < key[u])) {
                u = v;
            }
        }

        if (u == -1) break; // если не нашли подходящую вершину — граф несвязный

        inMST[u] = true; // включаем вершину в остов

        // Обновляем веса рёбер для всех соседей вершины u
        for (int i = 0; i < sizes[u]; i++) {
            int v = adj[u][i].to;     // соседняя вершина
            int w = adj[u][i].weight; // вес ребра u–v

            // Если вершина v ещё не в остове и вес меньше текущего key[v]
            if (!inMST[v] && w < key[v]) {
                key[v] = w;       // обновляем минимальный вес
                parent[v] = u;    // запоминаем родителя (из какой вершины пришли)
            }
        }
    }

    // Вывод результата
    cout << "\nМинимальное остовное дерево (алгоритм Прима):\n";
    int totalWeight = 0;

    for (int i = 0; i < n; i++) {
        if (parent[i] != -1) {
            cout << parent[i] << " - " << i
                << " (вес: " << get_weight(parent[i], i) << ")\n";
            totalWeight += get_weight(parent[i], i);
        }
    }

    cout << "Общий вес остовного дерева: " << totalWeight << "\n";

    // Освобождаем динамическую память
    delete[] parent;
    delete[] key;
    delete[] inMST;
}
