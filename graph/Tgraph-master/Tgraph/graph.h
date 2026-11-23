#ifndef GRAPH_H
#define GRAPH_H

#include <fstream>
#include <iostream>
#include <limits>
#include <string>
#include <vector>

// Ребро / дуга: to — индекс конца (0..size-1), weight — вес.
struct Edge {
    int to;
    int weight;
};

class Graph {
private:
    std::vector<std::vector<Edge>> adj; // списки смежности
    int vertex_count;                   // число вершин
    bool type;                          // true — орграф, false — неориентированный граф

    int index_of(int vertex) const;     // поиск вершины по её номеру
    void read_matrix(std::istream& fin, int n);
    void read_edges(std::istream& fin);
    void read_adjacency_lists(std::istream& fin, int n);

public:
    Graph(bool directed);                       // пустой граф
    Graph(const char* Fname, bool directed);    // чтение файла (первая строка: "n U"/"n D" или просто n)
    explicit Graph(const char* Fname);          // новый формат: "n U"/"n D" + матрица/списки
    ~Graph();

    int size() const;                           // количество вершин
    bool directed() const;                      // тип графа
    int get_weight(int v1, int v2) const;       // вес ребра/дуги (по индексам 0..n-1)
    bool is_edge(int v1, int v2) const;         // есть ли ребро/дуга

    void add_vertex(int num);                   // добавить вершину с внешним номером num
    void add_edge(int v1, int v2, int w);       // добавить ребро/дугу (по индексам)
    void add_edge_by_number(int v1, int v2, int w); // добавить ребро по внешним номерам (1..n)

    void list_of_edges() const;                 // все рёбра
    void list_of_edges(int v) const;            // рёбра, инцидентные вершине v
    void save_to_file(const char* Fname) const; // сохранить в новом формате

    // --- ANALYSIS / ВЫВОД В ФОРМАТЕ ans_bpg_*.txt ---
    void print_report(std::ostream& out) const;

    // старые методы варианта 1c 2a 3b 4a (оставлены для совместимости)
    void bellman_ford(int start_vertex) const;
    void kruskal() const;
};

#endif
