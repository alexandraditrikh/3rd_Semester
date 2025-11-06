#include <limits>
#include <iostream>

#include "List.h"
#include "Matrix.h"

using namespace std;

const int INF = std::numeric_limits<int>::max();

Path* creating_base_path(const Matrix& Matrix, int& final_path) {
    if (Matrix.size < 3) return nullptr;

    int n = Matrix.size;

    int min_dist = INF;
    int x_i = -1, x_j = -1; 
 
    for (int i = 0; i < n; i++) {
        for (int j = i + 1; j < n; j++) {
            if (Matrix.AdjMatrix[i][j] < min_dist) {
                min_dist = Matrix.AdjMatrix[i][j];
                x_i = i;
                x_j = j;
            }
        }
    }

    if (x_i == -1) return nullptr; 

    min_dist = INF; 
    int x_k = -1;

    for (int k = 0; k < n; k++) {
        if (k != x_i && k != x_j && Matrix.AdjMatrix[x_i][k] < min_dist) {
            min_dist = Matrix.AdjMatrix[x_i][k];
            x_k = k;
        }
    }
    
    if (x_k == -1) return nullptr; 

    final_path = 0;
    Path* path = new Path(x_i); 
    path->append_node(x_k);
    final_path += Matrix.AdjMatrix[x_i][x_k];
    path->append_node(x_j); 
    final_path += Matrix.AdjMatrix[x_k][x_j];
    final_path += Matrix.AdjMatrix[x_j][x_i];
    return path;
}



Path* task(const Matrix& Matrix, int& final_path) {
    Path* P_i = creating_base_path(Matrix, final_path);
    if (!P_i) return nullptr;

    int n = Matrix.size;
    
    bool* used_nodes = new bool[n];
    for (int i = 0; i < n; i++)
        used_nodes[i] = false;

    Node* cur = P_i->top;
    Node* temp_cur = P_i->top;
    do {
        used_nodes[temp_cur->id] = false;
        temp_cur = temp_cur->next;
    } while (temp_cur != P_i->top);
    
    
    for (int i = 3; i < n; i++) {
        int x_l = -1, x_m = -1, min_path = INF;

        for (int l = 0; l < n; l++) {
            int cur_min = INF;

            if (!used_nodes[l]) {
                for (Node* cur = P_i->top; cur != P_i->tail; cur = cur->next) {
                    if (Matrix.AdjMatrix[cur->id][l] < cur_min) {
                        cur_min = Matrix.AdjMatrix[cur->id][l];
                        x_l = l, x_m = cur->id;
                    }
                }
            }

            if (cur_min < min_path) min_path = cur_min;
        }

        if (x_l == -1 || x_m == -1) return nullptr;

        //final_path -= Matrix.AdjMatrix[x_m][x_l];
        P_i->insert_node_after(x_m, x_l);
        //final_path += Matrix.AdjMatrix[x_m][x_l];
    }

    delete[] used_nodes;
    return P_i;
}

int main() {
    Matrix Matrix;
    Matrix.enter_matrix();
    if (Matrix.size == 0) return 1;

    int final_path = 0;
    Path* result_path = task(Matrix, final_path);
    
    if (result_path) {
        int i = -1, j = -1;

        for (Node* cur = result_path->top; cur != result_path->tail; cur = cur->next->next) {
            i = cur->id;
            if (!cur->next) {
                cerr << "!cur->next" << endl;
                return -1;

            }
            j = cur->next->id;

            if (i != -1 && j != -1 && i != j) final_path += Matrix.AdjMatrix[i][j];
        } 
        delete result_path; 

    } else cerr << "!res path\n";
    
    return 0;
}