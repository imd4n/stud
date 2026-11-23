#include "graph.h"

#include <algorithm>
#include <cctype>
#include <functional>
#include <iomanip>
#include <numeric>
#include <sstream>

// ---------------------------------------------------------------------------
// Вспомогательные функции чтения
// ---------------------------------------------------------------------------
namespace {

std::vector<std::string> split_line(const std::string& line) {
    std::vector<std::string> tokens;
    std::istringstream iss(line);
    std::string token;
    while (iss >> token) tokens.push_back(token);
    return tokens;
}

// Похожа ли строка на строку матрицы смежности: ровно n целых чисел.
bool is_matrix_line(const std::string& line, int n) {
    std::vector<std::string> t = split_line(line);
    if ((int)t.size() != n) return false;
    for (const std::string& s : t) {
        size_t pos = (!s.empty() && s[0] == '-') ? 1 : 0;
        if (pos >= s.size()) return false;
        for (size_t i = pos; i < s.size(); ++i) {
            if (!std::isdigit((unsigned char)s[i])) return false;
        }
    }
    return true;
}

std::string join_lines(const std::vector<std::string>& lines) {
    return std::accumulate(lines.begin(), lines.end(), std::string(),
        [](const std::string& a, const std::string& b) { return a + b + "\n"; });
}

} // namespace

// ---------------------------------------------------------------------------
// Конструкторы, базовые операции
// ---------------------------------------------------------------------------
Graph::Graph(bool directed) : vertex_count(0), type(directed) {}

Graph::~Graph() = default;

int Graph::index_of(int vertex) const {
    return (vertex >= 0 && vertex < vertex_count) ? vertex : -1;
}

int Graph::size() const { return vertex_count; }

bool Graph::directed() const { return type; }

void Graph::add_vertex(int /*num*/) {
    adj.emplace_back();
    vertex_count = (int)adj.size();
}

void Graph::add_edge(int v1, int v2, int w) {
    if (v1 < 0 || v1 >= vertex_count || v2 < 0 || v2 >= vertex_count) return;
    adj[v1].push_back({v2, w});
    if (!type && v1 != v2) {
        adj[v2].push_back({v1, w});
    }
}

void Graph::add_edge_by_number(int v1, int v2, int w) {
    add_edge(v1 - 1, v2 - 1, w);
}

int Graph::get_weight(int v1, int v2) const {
    if (index_of(v1) == -1 || index_of(v2) == -1) return -1;
    for (const Edge& e : adj[v1]) {
        if (e.to == v2) return e.weight;
    }
    return -1;
}

bool Graph::is_edge(int v1, int v2) const {
    return get_weight(v1, v2) != -1;
}

void Graph::list_of_edges() const {
    for (int i = 0; i < vertex_count; ++i) {
        std::cout << "Vertex " << i + 1 << ": ";
        for (const Edge& e : adj[i]) {
            std::cout << "(" << e.to + 1 << "," << e.weight << ") ";
        }
        std::cout << '\n';
    }
}

void Graph::list_of_edges(int v) const {
    int i = index_of(v - 1);
    if (i == -1) return;
    std::cout << "Vertex " << v << ": ";
    for (const Edge& e : adj[i]) {
        std::cout << "(" << e.to + 1 << "," << e.weight << ") ";
    }
    std::cout << '\n';
}

// ---------------------------------------------------------------------------
// Чтение входных файлов (матрица / список рёбер / список смежности)
// ---------------------------------------------------------------------------
void Graph::read_matrix(std::istream& fin, int n) {
    // Сначала считываем всю матрицу: она может быть несимметричной
    // (старый graph.txt), поэтому для неориентированного графа берём
    // ненулевое значение из любой из двух клеток.
    std::vector<std::vector<int>> m(n, std::vector<int>(n, 0));
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            fin >> m[i][j];
        }
    }

    adj.assign(n, {});
    vertex_count = n;
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            if (type) {
                if (m[i][j] != 0) adj[i].push_back({j, m[i][j]}); // дуга i -> j
            } else if (i < j) {
                int w = (m[i][j] != 0) ? m[i][j] : m[j][i];
                if (w != 0) add_edge(i, j, w);                    // ребро i -- j один раз
            }
        }
    }
}

void Graph::read_edges(std::istream& fin) {
    adj.clear();
    vertex_count = 0;
    int u, v, w;
    while (fin >> u >> v >> w) {
        int need = std::max(u, v);                   // номера вершин с 1
        while (vertex_count < need) add_vertex(vertex_count);
        add_edge_by_number(u, v, w);
    }
}

void Graph::read_adjacency_lists(std::istream& fin, int n) {
    adj.assign(n, {});
    vertex_count = n;
    std::string line;
    for (int i = 0; i < n && std::getline(fin, line); ++i) {
        for (const std::string& token : split_line(line)) {
            size_t colon = token.find(':');
            if (colon == std::string::npos || colon == 0) continue;
            int v = std::stoi(token.substr(0, colon)) - 1;
            int w = std::stoi(token.substr(colon + 1));
            if (v < 0 || v >= n) continue;
            if (type) {
                adj[i].push_back({v, w});
            } else {
                if (i < v) add_edge(i, v, w);
            }
        }
    }
}

Graph::Graph(const char* Fname, bool directed) : vertex_count(0), type(directed) {
    std::ifstream fin(Fname);
    if (!fin) {
        std::cerr << "Cannot open file: " << Fname << '\n';
        return;
    }

    // Первая строка: "n U" / "n D" (новый формат) или просто "n" (старый).
    std::string first_line;
    std::getline(fin, first_line);
    if (!first_line.empty() && first_line.back() == '\r') first_line.pop_back();
    std::vector<std::string> head = split_line(first_line);

    int n = 0;
    if (!head.empty()) {
        try { n = std::stoi(head[0]); } catch (...) { n = 0; }
    }
    if (head.size() > 1) {
        char tp = head[1][0];
        if (tp == 'D' || tp == 'd') type = true;
        if (tp == 'U' || tp == 'u') type = false;
    }
    if (n <= 0) {
        std::cerr << "Invalid first line in file: " << Fname << '\n';
        return;
    }

    std::vector<std::string> rest;
    std::string line;
    while (std::getline(fin, line)) {
        if (!line.empty() && line.back() == '\r') line.pop_back();
        rest.push_back(line);
    }

    std::istringstream ss(join_lines(rest));
    if (!rest.empty() && rest[0].find(':') != std::string::npos) {
        read_adjacency_lists(ss, n);
    } else if (!rest.empty() && is_matrix_line(rest[0], n)) {
        read_matrix(ss, n);
    } else {
        read_edges(ss);
    }
}

Graph::Graph(const char* Fname) : Graph(Fname, false) {}

void Graph::save_to_file(const char* Fname) const {
    std::ofstream fout(Fname);
    if (!fout) {
        std::cerr << "Cannot create file: " << Fname << '\n';
        return;
    }
    fout << vertex_count << ' ' << (type ? 'D' : 'U') << '\n';
    std::vector<std::vector<int>> m(vertex_count, std::vector<int>(vertex_count, 0));
    for (int u = 0; u < vertex_count; ++u) {
        for (const Edge& e : adj[u]) m[u][e.to] = e.weight;
    }
    for (int i = 0; i < vertex_count; ++i) {
        for (int j = 0; j < vertex_count; ++j) {
            fout << m[i][j] << (j + 1 == vertex_count ? "" : " ");
        }
        fout << '\n';
    }
}

// ---------------------------------------------------------------------------
// Отчёт в формате файлов ans_bpg_*.txt
// ---------------------------------------------------------------------------
void Graph::print_report(std::ostream& out) const {
    const int n = vertex_count;

    // Компоненты связности (для орграфа — слабой связности).
    std::vector<std::vector<int>> und(n);
    for (int u = 0; u < n; ++u) {
        for (int v = 0; v < n; ++v) {
            if (u == v) continue;
            if (get_weight(u, v) != -1 || get_weight(v, u) != -1) {
                und[u].push_back(v);
                und[v].push_back(u);
            }
        }
    }
    std::vector<char> used(n, 0);
    std::vector<std::vector<int>> comps;
    for (int s = 0; s < n; ++s) {
        if (used[s]) continue;
        std::vector<int> comp;
        std::vector<int> st;
        st.push_back(s);
        used[s] = 1;
        while (!st.empty()) {
            int u = st.back();
            st.pop_back();
            comp.push_back(u + 1);
            for (int v : und[u]) {
                if (!used[v]) {
                    used[v] = 1;
                    st.push_back(v);
                }
            }
        }
        std::sort(comp.begin(), comp.end());
        comps.push_back(comp);
    }
    bool connected = (comps.size() == 1);

    // Шапка.
    out << (type ? "Graph is directed.\n" : "Graph is undirected.\n");
    if (type) {
        out << (connected ? "Digraph is weakly connected\n" : "Digraph is not connected\n");
    } else {
        out << (connected ? "Graph is connected\n" : "Graph is not connected\n");
    }
    out << '\n';
    out << (type ? "Weakly connected components:\n" : "Connected components:\n");
    for (const std::vector<int>& c : comps) {
        out << '[';
        for (size_t i = 0; i < c.size(); ++i) {
            if (i) out << ", ";
            out << c[i];
        }
        out << "]\n";
    }
    out << '\n';

    // Кратчайшие расстояния и пути: Беллман-Форд из каждой вершины.
    const long long INF = std::numeric_limits<long long>::max() / 4;
    std::vector<std::vector<long long>> dist(n, std::vector<long long>(n, -1));
    std::vector<std::vector<std::vector<int>>> paths(n);

    for (int s = 0; s < n; ++s) {
        std::vector<long long> d(n, INF);
        std::vector<int> parent(n, -1);
        d[s] = 0;

        for (int iter = 0; iter < n - 1; ++iter) {
            bool changed = false;
            for (int u = 0; u < n; ++u) {
                if (d[u] == INF) continue;
                for (const Edge& e : adj[u]) {
                    if (d[e.to] > d[u] + e.weight) {
                        d[e.to] = d[u] + e.weight;
                        parent[e.to] = u;
                        changed = true;
                    }
                }
            }
            if (!changed) break;
        }

        for (int t = 0; t < n; ++t) {
            if (d[t] == INF) continue;
            dist[s][t] = d[t];
            if (t == s) continue;
            std::vector<int> path;
            int cur = t;
            while (cur != -1) {
                path.push_back(cur + 1);
                if (cur == s) break;
                cur = parent[cur];
            }
            if (!path.empty() && path.back() == s + 1) {
                std::reverse(path.begin(), path.end());
                paths[s].push_back(path);
            }
        }
    }

    for (int i = 0; i < n; ++i) {
        out << "Shortest paths lengths from " << i + 1 << ":\n";
        for (int j = 0; j < n; ++j) {
            out << std::setw(3) << i + 1 << " - " << std::setw(3) << j + 1 << " : ";
            if (dist[i][j] == -1) out << "+Infinity";
            else out << dist[i][j];
            out << '\t';
        }
        out << "\n\n";
    }

    for (int i = 0; i < n; ++i) {
        if (i > 0) out << "\n\n";
        for (const std::vector<int>& p : paths[i]) {
            out << "Path from " << i + 1 << " to " << p.back() << ":\n";
            for (size_t k = 0; k < p.size(); ++k) {
                if (k) out << '-';
                out << p[k];
            }
            out << '\n';
        }
    }
    out << '\n';

    // Минимальное остовное дерево: Краскал, направления дуг не учитываются.
    if (connected) {
        struct KEdge { int u, v, w; };
        std::vector<KEdge> edges;
        for (int u = 0; u < n; ++u) {
            for (int v = u + 1; v < n; ++v) {
                int w1 = get_weight(u, v);
                int w2 = get_weight(v, u);
                int w = -1;
                if (w1 != -1 && w2 != -1) w = std::min(w1, w2);
                else if (w1 != -1) w = w1;
                else if (w2 != -1) w = w2;
                if (w != -1) edges.push_back({u, v, w});
            }
        }
        std::stable_sort(edges.begin(), edges.end(),
            [](const KEdge& a, const KEdge& b) { return a.w < b.w; });

        std::vector<int> p(n);
        std::iota(p.begin(), p.end(), 0);
        std::function<int(int)> find = [&](int x) {
            return p[x] == x ? x : p[x] = find(p[x]);
        };
        std::vector<KEdge> mst;
        long long total = 0;
        for (const KEdge& e : edges) {
            int a = find(e.u), b = find(e.v);
            if (a == b) continue;
            p[a] = b;
            mst.push_back(e);
            total += e.w;
            if ((int)mst.size() == n - 1) break;
        }

        out << "Weight of minimum spanning tree: " << total << "\n";
        out << "Spanning tree:\n";
        for (size_t i = 0; i < mst.size(); ++i) {
            if (i) out << '\n';
            out << mst[i].u + 1 << '-' << mst[i].v + 1 << " : " << mst[i].w;
        }
    }
}

// ---------------------------------------------------------------------------
// Старые методы варианта 3b / 4a (для совместимости)
// ---------------------------------------------------------------------------
void Graph::bellman_ford(int start_vertex) const {
    const long long INF = std::numeric_limits<long long>::max() / 4;
    int n = vertex_count;
    if (start_vertex < 0 || start_vertex >= n) return;
    std::vector<long long> d(n, INF);
    d[start_vertex] = 0;
    for (int iter = 0; iter < n - 1; ++iter) {
        bool changed = false;
        for (int u = 0; u < n; ++u) {
            if (d[u] == INF) continue;
            for (const Edge& e : adj[u]) {
                if (d[e.to] > d[u] + e.weight) {
                    d[e.to] = d[u] + e.weight;
                    changed = true;
                }
            }
        }
        if (!changed) break;
    }
    std::cout << "Bellman-Ford from vertex " << start_vertex << ":\n";
    for (int i = 0; i < n; ++i) {
        std::cout << i << ": "
                  << (d[i] == INF ? std::string("infinity") : std::to_string(d[i])) << '\n';
    }
}

void Graph::kruskal() const {
    std::ostringstream tmp;
    print_report(tmp);
    std::string s = tmp.str();
    size_t pos = s.find("Weight of minimum spanning tree:");
    if (pos == std::string::npos) {
        std::cout << "Graph is disconnected, no spanning tree.\n";
        return;
    }
    std::cout << s.substr(pos);
}
