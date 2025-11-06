#include <limits>
#include <iostream>

#include "List.h"
#include "Matrix.h"

using namespace std;

const int INF = std::numeric_limits<int>::max();

Path* creating_base_path(const Matrix& Matrix, int& final_path) {
    if (Matrix.size < 3) {
        cerr << "ERROR Matrix.size < 3" << endl;
        return nullptr;
    }

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

    if (x_i == -1) {
        cerr << "ERROR x_i == -1" << endl;
        return nullptr;
    }

    min_dist = INF; 
    int x_k = -1;

    for (int k = 0; k < n; k++) {
        if (k != x_i && k != x_j && Matrix.AdjMatrix[x_i][k] < min_dist) {
            min_dist = Matrix.AdjMatrix[x_i][k];
            x_k = k;
        }
    }
    
    if (x_k == -1) {
        cerr << "ERROR x_k == -1" << endl;
        return nullptr; 
    }

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
    if (!P_i) {
        cerr << "ERROR !P_i = creating_base_path(Matrix, final_path);" << endl;
        return nullptr;
    } 

    int n = Matrix.size;
    
    // Массив для отслеживания использованных узлов.
    // Узлы в P_i уже должны быть помечены как true.
    bool* used_nodes = new bool[n];
    for (int i = 0; i < n; i++)
        used_nodes[i] = false;

    // Помечаем узлы в начальном пути P_i как использованные (true).
    Node* temp_cur = P_i->top;
    do {
        used_nodes[temp_cur->id] = true; // <-- Устанавливаем в true
        temp_cur = temp_cur->next;
    } while (temp_cur != P_i->top);
    
    // Итерации: начинаем с i=3, пока не добавим все n узлов.
    for (int i = 3; i < n; i++) {
        
        int best_l = -1; // Новый узел x_l (вне пути)
        int best_m = -1; // Узел x_m (в пути), перед которым вставим x_l
        int best_r = -1; // Узел x_r (в пути), который идет после x_m (т.е. cur->next)

        // Минимальное увеличение длины пути (min_increase = D(xm, xl) + D(xl, xr) - D(xm, xr))
        int min_increase = INF; 

        // 1. Ищем лучший узел x_l (не в пути) для вставки
        for (int l = 0; l < n; l++) {
            // Пропускаем узлы, которые уже в пути
            if (used_nodes[l]) {
                continue;
            }

            // 2. Ищем лучшее место (ребро x_m -> x_r) для вставки x_l
            Node* cur_m = P_i->top;
            do {
                int m_id = cur_m->id;
                int r_id = cur_m->next->id;
                
                // Рассчитываем увеличение длины: D(m, l) + D(l, r) - D(m, r)
                // Проверяем, что рёбра существуют (AdjMatrix > 0 или не INF)
                int dist_ml = Matrix.AdjMatrix[m_id][l];
                int dist_lr = Matrix.AdjMatrix[l][r_id];
                int dist_mr = Matrix.AdjMatrix[m_id][r_id];

                // Проверка на корректные расстояния (чтобы избежать переполнения при сложении с INF)
                if (dist_ml != INF && dist_lr != INF && dist_mr != INF) {
                    
                    int current_increase = dist_ml + dist_lr - dist_mr;
                    
                    if (current_increase < min_increase) {
                        min_increase = current_increase;
                        best_l = l;
                        best_m = m_id;
                        best_r = r_id; // Фактически не используется в insert_node_after, но полезно для отладки
                    }
                }
                
                cur_m = cur_m->next;
            } while (cur_m != P_i->top); // Циклический обход

        } // Конец цикла по l

        // Проверяем, нашли ли мы лучший узел для вставки
        if (best_l == -1 || best_m == -1) {
             // Если не найдено ни одного пути, это ошибка или граф не связный
             cerr << "ERROR best_l == -1 || best_m == -1" << endl;
             delete[] used_nodes;
             return nullptr;
        }

        // 3. Вставляем найденный узел x_l после x_m
        P_i->insert_node_after(best_m, best_l);
        
        // 4. Обновляем использованные узлы и общую длину пути
        used_nodes[best_l] = true;
        final_path += min_increase;
    } // Конец цикла по i

    delete[] used_nodes;
    return P_i;
}

int main() {
    Matrix Matrix;
    Matrix.enter_matrix();
    if (Matrix.size == 0) {
        cerr << "Matrix.size == 0" << endl;
        return 1;
    }

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
    } else cerr << "ERROR !res path\n";
    
    cout << final_path << endl;

    return 0;
}