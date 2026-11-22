#include "graph.h"
#include <vector>
#include <algorithm>

// Конструктор пустого графа
Graph::Graph(bool t) {
    type = t;
    vertex_count = 0;
    adj = nullptr;
    sizes = nullptr;
    capacities = nullptr;
    vertex_numbers = nullptr;
}

// Деструктор графа
Graph::~Graph() {
    if (adj) {
        for (int i = 0; i < vertex_count; ++i) {
            delete[] adj[i];
        }
        delete[] adj;
    }
    delete[] sizes;
    delete[] capacities;
    delete[] vertex_numbers;
}

// Конструктор графа из файла с матрицей смежности
Graph::Graph(const char* Fname, bool t) {
    type = t;
    vertex_count = 0;
    adj = nullptr;
    sizes = nullptr;
    capacities = nullptr;
    vertex_numbers = nullptr;

    ifstream fin(Fname);
    if (!fin) {
        cerr << "Cannot open file!\n";
        exit(1);
    }

    int n;
    fin >> n;

    vector<vector<int>> matrix(n, vector<int>(n));
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            fin >> matrix[i][j];
        }
    }

    // Добавляем вершины 0..n-1
    for (int i = 0; i < n; ++i) {
        add_vertex(i);
    }

    // Читаем матрицу смежности
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            if (i == j) continue;

            if (type) {
                // Неориентированный граф: добавляем ребро только один раз
                if (i < j) {
                    int w = matrix[i][j];
                    if (w == 0) w = matrix[j][i]; // на случай несимметричной записи
                    if (w != 0) {
                        add_edge(i, j, w);
                    }
                }
            }
            else {
                // Ориентированный граф: добавляем дугу i -> j
                int w = matrix[i][j];
                if (w != 0) {
                    add_edge(i, j, w);
                }
            }
        }
    }

    fin.close();
}

int Graph::size() {
    return vertex_count;
}

void Graph::resize(int v) {
    capacities[v] *= 2;
    Edge* newArr = new Edge[capacities[v]];

    for (int i = 0; i < sizes[v]; ++i) {
        newArr[i] = adj[v][i];
    }

    delete[] adj[v];
    adj[v] = newArr;
}

void Graph::add_vertex(int vertex) {
    if (index_of(vertex) != -1) return;

    Edge** newAdj = new Edge * [vertex_count + 1];
    int* newSizes = new int[vertex_count + 1];
    int* newCaps = new int[vertex_count + 1];
    int* newNumbers = new int[vertex_count + 1];

    for (int i = 0; i < vertex_count; ++i) {
        newAdj[i] = adj[i];
        newSizes[i] = sizes[i];
        newCaps[i] = capacities[i];
        newNumbers[i] = vertex_numbers[i];
    }

    newSizes[vertex_count] = 0;
    newCaps[vertex_count] = 2;
    newAdj[vertex_count] = new Edge[2];
    newNumbers[vertex_count] = vertex;

    delete[] adj;
    delete[] sizes;
    delete[] capacities;
    delete[] vertex_numbers;

    adj = newAdj;
    sizes = newSizes;
    capacities = newCaps;
    vertex_numbers = newNumbers;
    vertex_count++;
}

void Graph::add_edge(int v1, int v2, int w) {
    if (sizes[v1] == capacities[v1]) resize(v1);
    adj[v1][sizes[v1]++] = { v2, w };

    if (type && v1 != v2) {
        if (sizes[v2] == capacities[v2]) resize(v2);
        adj[v2][sizes[v2]++] = { v1, w };
    }
}

bool Graph::is_edge(int v1, int v2) {
    for (int i = 0; i < sizes[v1]; ++i) {
        if (adj[v1][i].to == v2) return true;
    }
    return false;
}

int Graph::get_weight(int v1, int v2) {
    for (int i = 0; i < sizes[v1]; ++i) {
        if (adj[v1][i].to == v2) return adj[v1][i].weight;
    }
    return -1;
}

void Graph::list_of_edges() {
    for (int i = 0; i < vertex_count; ++i) {
        cout << "Vertex " << i << ": ";
        for (int j = 0; j < sizes[i]; ++j) {
            cout << "(" << adj[i][j].to << "," << adj[i][j].weight << ") ";
        }
        cout << endl;
    }
}

void Graph::list_of_edges(int v) {
    cout << "Vertex " << v << ": ";
    for (int i = 0; i < sizes[v]; ++i) {
        cout << "(" << adj[v][i].to << "," << adj[v][i].weight << ") ";
    }
    cout << endl;
}

void Graph::save_to_file(const char* Fname) {
    ofstream fout(Fname);
    if (!fout) {
        cerr << "Ошибка сохранения!" << endl;
        return;
    }

    fout << vertex_count << endl;

    // Строим матрицу смежности
    vector<vector<int>> matrix(vertex_count, vector<int>(vertex_count, 0));

    for (int u = 0; u < vertex_count; ++u) {
        for (int k = 0; k < sizes[u]; ++k) {
            int v = adj[u][k].to;
            int w = adj[u][k].weight;
            matrix[u][v] = w;
        }
    }

    for (int i = 0; i < vertex_count; ++i) {
        for (int j = 0; j < vertex_count; ++j) {
            fout << matrix[i][j] << (j + 1 == vertex_count ? "" : " ");
        }
        fout << endl;
    }

    fout.close();
}

int Graph::index_of(int vertex) {
    for (int i = 0; i < vertex_count; ++i) {
        if (vertex_numbers[i] == vertex) return i;
    }
    return -1;
}

// ------------------------------------------------------------
// 3b. Алгоритм Беллмана–Форда
// ------------------------------------------------------------
void Graph::bellman_ford(int start_vertex) {
    const long long INF = numeric_limits<long long>::max() / 4;

    int n = vertex_count;

    if (start_vertex < 0 || start_vertex >= n) {
        cout << "Ошибка: неверная стартовая вершина!\n";
        return;
    }

    vector<long long> dist(n, INF);
    dist[start_vertex] = 0;

    // Релаксация рёбер n-1 раз
    for (int step = 0; step < n - 1; ++step) {
        bool updated = false;

        for (int u = 0; u < n; ++u) {
            if (dist[u] == INF) continue;

            for (int k = 0; k < sizes[u]; ++k) {
                int v = adj[u][k].to;
                int w = adj[u][k].weight;

                if (dist[v] > dist[u] + w) {
                    dist[v] = dist[u] + w;
                    updated = true;
                }
            }
        }

        if (!updated) break;
    }

    // Проверка на цикл отрицательного веса
    bool negative_cycle = false;

    for (int u = 0; u < n && !negative_cycle; ++u) {
        if (dist[u] == INF) continue;

        for (int k = 0; k < sizes[u]; ++k) {
            int v = adj[u][k].to;
            int w = adj[u][k].weight;

            if (dist[v] > dist[u] + w) {
                negative_cycle = true;
                break;
            }
        }
    }

    cout << "Кратчайшие расстояния (Беллман-Форд) от вершины "
        << start_vertex << ":\n";

    for (int i = 0; i < n; ++i) {
        if (dist[i] == INF) {
            cout << "До " << i << ": недостижимо\n";
        }
        else {
            cout << "До " << i << ": " << dist[i] << "\n";
        }
    }

    if (negative_cycle) {
        cout << "Внимание: обнаружен цикл отрицательного веса, "
            << "кратчайшие расстояния могут быть некорректны.\n";
    }
}

// ------------------------------------------------------------
// 4a. Алгоритм Краскала
// ------------------------------------------------------------
void Graph::kruskal() {
    struct KEdge {
        int u;
        int v;
        int w;
    };

    struct DSU {
        vector<int> parent;
        vector<int> rank;

        DSU(int n) : parent(n), rank(n, 0) {
            for (int i = 0; i < n; ++i) {
                parent[i] = i;
            }
        }

        int find(int x) {
            if (parent[x] != x) {
                parent[x] = find(parent[x]);
            }
            return parent[x];
        }

        bool unite(int a, int b) {
            a = find(a);
            b = find(b);

            if (a == b) return false;

            if (rank[a] < rank[b]) {
                swap(a, b);
            }

            parent[b] = a;

            if (rank[a] == rank[b]) {
                rank[a]++;
            }

            return true;
        }
    };

    int n = vertex_count;
    vector<KEdge> edges;

    for (int u = 0; u < n; ++u) {
        for (int k = 0; k < sizes[u]; ++k) {
            int v = adj[u][k].to;
            int w = adj[u][k].weight;

            if (u == v) continue;

            // Для неориентированного графа каждое ребро хранится дважды,
            // поэтому берём только u < v.
            if (type && u > v) continue;

            edges.push_back({ u, v, w });
        }
    }

    sort(edges.begin(), edges.end(),
        [](const KEdge& a, const KEdge& b) {
            return a.w < b.w;
        });

    DSU dsu(n);
    vector<KEdge> mst;
    int total_weight = 0;

    for (const KEdge& e : edges) {
        if (dsu.unite(e.u, e.v)) {
            mst.push_back(e);
            total_weight += e.w;

            if ((int)mst.size() == n - 1) {
                break;
            }
        }
    }

    cout << "\nМинимальное остовное дерево (алгоритм Краскала):\n";

    for (const KEdge& e : mst) {
        cout << e.u << " - " << e.v
            << " (вес: " << e.w << ")\n";
    }

    cout << "Общий вес остовного дерева: " << total_weight << "\n";

    if ((int)mst.size() < n - 1) {
        cout << "Граф несвязный: найден минимальный остовный лес.\n";
    }
}