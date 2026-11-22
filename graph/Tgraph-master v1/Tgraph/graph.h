#include <iostream>
#include <fstream>
#include <limits>
using namespace std;

// Структура ребра: вершина назначения и вес
struct Edge {
    int to;
    int weight;
};

class Graph {
private:
    Edge** adj;          // массив списков смежности
    int* sizes;          // количество рёбер для каждой вершины
    int* capacities;     // ёмкость каждого списка
    int vertex_count;    // количество вершин
    bool type;           // true - неориентированный, false - ориентированный
    int* vertex_numbers; // реальные номера вершин

    void resize(int v);  // увеличить список смежности
    int index_of(int vertex); // возвращает индекс массива по номеру вершины

public:
    Graph(bool type);                       // пустой граф
    Graph(const char* Fname, bool type);    // граф из файла (списки смежности)
    ~Graph();  // деструктор
    int size();                             // количество вершин
    int get_weight(int v1, int v2);         // вес ребра
    bool is_edge(int v1, int v2);           // проверка наличия ребра
    void add_vertex(int num); // добавление вершины с конкретным номером
    void add_edge(int v1, int v2, int w);   // добавление ребра
    void list_of_edges();                    // вывод всех рёбер
    void list_of_edges(int v);              // вывод рёбер одной вершины
    void save_to_file(const char* Fname);   // сохранение графа в файл
    void dijkstra(int start_vertex);
    void prim(int start_vertex);
};
