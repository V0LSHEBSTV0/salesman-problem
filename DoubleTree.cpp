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
    
    void AlgPrima() { // постройка остовного дерева
        std::vector<int> U = { 0 };
        
        while(U.size() != n) {
            float min = std::numeric_limits<float>::max();
            std::pair<int, int>imin = { -1, -1 }; // ребра не могут иметь отрицательные веса в евклидовом графе


            for (auto const& j : U) {
                for (int k = 0; k < n; k++) {
                    if ((std::find(U.begin(), U.end(), k) == U.end()) && graph[j][k] != 0.0) {
                        if (graph[j][k] < min) {
                            min = graph[j][k];
                            imin = {j, k };
                        }
                    }
                }
            }
            

            if(imin.first != -1 && imin.second != -1){
                ost[imin.first][imin.second] = 2;
                ost[imin.second][imin.first] = 2; // дублирование остовного дерева
                U.push_back(imin.second);
            }
        }


    }

    //void print() {

    //    for (auto const& ost_ : ost) {
    //        for (auto const& ost__ : ost_) {
    //            std::cout << ost__ << " ";
    //        }
    //        std::cout << std::endl;
    //    }
    //}


    // Поиск начальной вершины, т.к. если в графе нет вершин с нечётной степенью, мы можем выбрать любую вершину в
    // качестве начальной, в противном случае, когда одна 
    // из вершин имеет нечётную степень, мы должны выбрать её в первую очередь
    int findStart() {
        int i = 0;

        while(i < n) {
            int step = 0;
            for (int j = 0; j < n; j++) {
                if (ost[i][j] != 0) {
                    step += ost[i][j] != 0 ? 1 : 0;
                }
            }
            if (step % 2 != 0) return i ;
            i += 1;
        }

        return -1;
    }

    void dfs(int u, std::vector<bool>& visited) {
        visited[u] = true;
        for (int v = 0; v < n; ++v)
            if (ost[u][v] > 0 && !visited[v])
                dfs(v, visited);
    }

    bool checkBrig(int u, int v) { //провекра на мостовость
        if (ost[u][v] == 0) return false;
        ost[u][v]--;
        ost[v][u]--;
        std::vector<bool> visited(n, false);
        dfs(u, visited);
        ost[u][v]++;
        ost[v][u]++;


        return !visited[v];
    }

    // void Fleri() {

    //     std::stack<int> st;
    //     int start = findStart();
    //     st.push(start);

    //     while (!st.empty()) {
    //         int v = st.top();
    //         bool found = false;

    //         for (int i = 0; i < n; i++) {
    //             if (ost[v][i] > 0){
                    
    //                 int countDegree = 0;
    //                 for (int j = 0; j < n; j++) countDegree += ost[v][j];

    //                 if (countDegree > 1 && checkBrig(v, i)) continue;

    //                 st.push(i);
    //                 ost[v][i]--;
    //                 ost[i][v]--;
    //                 found = true;
    //                 break;
                    
    //             }
    //         }

    //         if (!found) {
    //             cicEul.push_back(v);
    //             st.pop();
    //         }
    //     }

    //     for (int i = 0; i < cicEul.size(); i++) {
    //         for (int j = i + 1; j < cicEul.size(); ) {
    //             if (cicEul[i] == cicEul[j]) {
    //                 for (int k = j; k + 1 < cicEul.size(); k++) {
    //                     cicEul[k] = cicEul[k + 1];
    //                 }
    //                 cicEul.pop_back();
    //             }
    //             else {
    //                 j++;
    //             }
    //         }
    //     }

    //     cicEul.push_back(0);

    //     for (int i = 0; i < n; i++) {
    //         sumPath += graph[cicEul[i]][cicEul[i+1]];
    //     }
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
    // void callAlgPrima(DoubleTree* dt) { dt->AlgPrima(); } //функция из С вызывает метод с++
    // void callFleri(DoubleTree* dt) { dt->Fleri(); } // 

    int* getCycleDoubleTree(float** graph,  int* n_) {
        int n = *n_;
        std::vector<std::vector<float>> graphVec(n, std::vector<float>(n)); //граф из си переводится в вектор
        for (int i = 0; i < n; ++i)
            for (int j = 0; j < n; ++j)
                graphVec[i][j] = graph[i][j];
        
        DoubleTree* dt = new DoubleTree(graphVec, n);

        auto cycle = dt->getCycle();
        // *out_size = cycle.size();
        int* arr = new int[cycle.size()];
        std::copy(cycle.begin(), cycle.end(), arr);

        delete dt;
        return arr;
    }
}
