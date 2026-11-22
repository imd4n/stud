#include <iostream>
#include "graph.h"
using namespace std;

int main() {
    setlocale(LC_ALL, "");

    cout << "Создаем пустой неориентированный граф\n";
    Graph g(true);

    cout << "Добавляем 4 вершины\n";
    g.add_vertex(0);
    g.add_vertex(1);
    g.add_vertex(2);
    g.add_vertex(3);

    cout << "Добавляем ребра\n";
    g.add_edge(0, 1, 5);
    g.add_edge(1, 2, 3);
    g.add_edge(1, 3, 2);

    cout << "Список всех рёбер:\n";
    g.list_of_edges();

    // 3b: Беллман-Форд
    g.bellman_ford(0);

    // 4a: Краскал
    g.kruskal();

    cout << "Проверяем наличие ребра 0->1: "
        << g.is_edge(0, 1) << endl;

    cout << "Вес ребра 1->2: "
        << g.get_weight(1, 2) << endl;

    cout << "\nСохраняем граф в файл graph_out.txt\n";
    g.save_to_file("graph_out.txt");

    cout << "\nСоздаем граф из файла graph.txt "
        << "(матрица смежности, ориентированный)\n";

    Graph g2("graph.txt", false);

    g2.list_of_edges();

    // Беллман-Форд для ориентированного графа
    g2.bellman_ford(0);

    // Краскал для орграфа без учёта направлений дуг
    g2.kruskal();

    return 0;
}