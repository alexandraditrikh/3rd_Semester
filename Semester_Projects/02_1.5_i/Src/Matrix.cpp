#include "Matrix.h"

#include <iostream>
#include <stdexcept>
#include <limits>

using namespace std;

Matrix:: Matrix() {
    for (int i = 0; i < N_MAX; i++) {
        for (int j = 0; j < N_MAX; j++) {
            AdjMatrix[i][j] = 0;
        }
    }
}

void Matrix::enter_matrix() {
    int i, j;
    int dist;
    int max_index = 0;

    while (true) {
        cout << "\nEnter i, j: ";

        if (!(cin >> i >> j)) {
            if (cin.eof()) { 
                // Обработка EOF (конец файла)
                break;
            }
            // Очищаем флаги ошибок cin и игнорируем оставшуюся строку 
            cin.clear(); 
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            return;
        }

        cout << "Enter distance (x_" << i << ", x_" << j << "): ";

        if (!(cin >> dist)) {
            if (cin.eof()) { 
                break;
            }
            // Ошибка: нечисловой ввод для dist.
            cin.clear();
            return;
        }

        // 3. Валидация данных с использованием ИСКЛЮЧЕНИЙ
        if (i < 0 || j < 0 || i >= N_MAX || j >= N_MAX) {
            // Выбрасываем исключение для недопустимых индексов
            throw runtime_error("Error: Index i or j is out of bounds [0, N_MAX).");
        }
        if (i == j) {
            // Выбрасываем исключение, если есть ребро в себя
             throw runtime_error("Error: Edge from a node to itself (i == j) is not allowed.");
        }
        if (dist < 0) {
            // Выбрасываем исключение для отрицательной дистанции (если не поддерживается)
            throw runtime_error("Error: Negative distance is not allowed.");
        }

        AdjMatrix[i][j] = dist;
        AdjMatrix[j][i] = dist; // Симметричность

        // Обновление максимального используемого индекса
        if (max_index < i) max_index = i;
        if (max_index < j) max_index = j;
    }

    size = max_index + 1;
}
