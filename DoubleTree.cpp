#include <iostream>
#include <vector>
#include <stack>
#include <limits>

// Добавил для конекта с мейном си
#include <algorithm>  
#include <climits>   
#include <limits>

class DoubleTree {
private:
    std::vector<std::vector<float>>graph;
    int n;
    std::vector<std::vector<int>> ost;

    float sumPath = 0;
    std::vector<int> cicEul;
    
public:
    DoubleTree(std::vector<std::vector<float>> graph_, int n_) {
        graph = graph_;
        n = n_;
        ost = std::vector<std::vector<int>>(n, std::vector<int>(n, 0));
    }
    
    void AlgPrima() {  // постройка остовного дерева, теперь O(N^2)
        std::vector<int> U = { 0 };  // замена вектор индексов на флаги
        std::vector<float> min(n, std::numeric_limits<float>::max());
        std::vector<int> par(n, -1);
    
        // инициализация мин. весов для стартовой вершины
        for (int i = 0; i < n; i++) {
            if (graph[0][i] != 0.0) {
                min[i] = graph[0][i];
                par[i] = 0;
            }
        }
    
        while (U.size() != n) {
            float min_weight = std::numeric_limits<float>::max();
            std::pair<int, int> imin = { -1, -1 }; // ребра не могут иметь отрицательные веса в евклидовом графе
    
    
    
            for (int i = 0; i < n; i++) {
                if (std::find(U.begin(), U.end(), i) == U.end() && min[i] < min_weight) {
                    min_weight = min[i];
                    imin = { par[i], i };
                }
            }
    
            if (imin.second == -1) break; // граф несвязный
    
            
            U.push_back(imin.second);
            ost[imin.first][imin.second] = 2;
            ost[imin.second][imin.first] = 2; // дублирование остовного дерева
    
            
            for (int i = 0; i < n; i++) {//восстановление
                if (graph[imin.second][i] != 0.0 && graph[imin.second][i] < min[i] &&
                    std::find(U.begin(), U.end(), i) == U.end()) {
                    min[i] = graph[imin.second][i];
                    par[i] = imin.second;
                }
            }
        }
    }

    void Fleri() {
        std::stack<int> st;
        int start = 0; // Start from vertex 0 (all degrees even in doubled MST)
        st.push(start);
    
        std::vector<bool> visited_edges(n * n, false); // Track visited edges
    
        while (!st.empty()) {
            int v = st.top();
            bool found = false;
    
            for (int i = 0; i < n; i++) {
                int edge_idx = v * n + i;
                if (ost[v][i] > 0 && !visited_edges[edge_idx]) {
                    st.push(i);
                    ost[v][i]--;
                    ost[i][v]--;
                    visited_edges[edge_idx] = true;
                    visited_edges[i * n + v] = true; // Mark symmetric edge
                    found = true;
                    break;
                }
            }
    
            if (!found) {
                cicEul.push_back(v);
                st.pop();
            }
        }
    
        // Remove duplicates, keeping first occurrence
        std::vector<bool> seen(n, false);
        int write_idx = 0;
        for (int i = 0; i < cicEul.size(); i++) {
            int v = cicEul[i];
            if (!seen[v]) {
                cicEul[write_idx++] = v;
                seen[v] = true;
            }
        }
        cicEul.resize(write_idx);
    
        // Add 0 to close the cycle
        cicEul.push_back(cicEul[0]);
    
        // Compute sumPath (corrected to use cicEul.size() - 1)
        sumPath = 0;
        for (int i = 0; i < cicEul.size() - 1; i++) {
            sumPath += graph[cicEul[i]][cicEul[i + 1]];
        }
    }

    std::vector<int> getCycle(){

        AlgPrima();
        Fleri();

        return cicEul;
    }

    int getsum() {
        return sumPath;
    }
};

extern "C" {

    int* getCycleDoubleTree(float** graph,  int* n_) {
        int n = *n_;
        std::vector<std::vector<float>> graphVec(n, std::vector<float>(n)); //граф из си переводится в вектор
        for (int i = 0; i < n; ++i)
            for (int j = 0; j < n; ++j)
                graphVec[i][j] = graph[i][j];
        
        DoubleTree* dt = new DoubleTree(graphVec, n);

        auto cycle = dt->getCycle();
        int* arr = new int[cycle.size()];
        std::copy(cycle.begin(), cycle.end(), arr);

        delete dt;
        return arr;
    }
}
