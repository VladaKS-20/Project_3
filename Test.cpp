#include <mpi.h>
#include <iostream>
#include <cmath>
#include <limits>
#include <cassert>
#include <cstring>

using namespace std;

// Функция f(x) = x^2 + A*x + B
double f(double x, double A, double B) {
    return x * x + A * x + B;
}

// Поиск корня на подотрезке методом перебора
bool find_root_on_subinterval(double start, double end, double step, double A, double B, double& root) {
    double prev_value = f(start, A, B);
    for (double x = start; x <= end; x += step) {
        double curr_value = f(x, A, B);
        if (prev_value * curr_value <= 0) {  // Проверка смены знака
            root = x - step / 2;             // Приближение корня
            return true;
        }
        prev_value = curr_value;
    }
    return false;     // Корень не найден
}

// Тестовая функция
void test_root(int rank, double A, double B, double Z, double expected_root) {
    if (rank == 0) {
        cout << "Тестирование с A=" << A << ", B=" << B << ", Z=" << Z << endl;
    }

    double step = 0.001;
    double root = numeric_limits<double>::max();

    // Имитируем выполнение алгоритма
    double interval_length = 2 * Z / 1; // Один процесс для теста
    double start = -Z;
    double end = start + interval_length;
    bool found = find_root_on_subinterval(start, end, step, A, B, root);

    // Проверка результата
    if (expected_root == numeric_limits<double>::max()) {
        assert(!found || root == numeric_limits<double>::max());
    }
    else {
        assert(found && abs(root - expected_root) < 0.001);
    }

    if (rank == 0) {
        root = round(root * 1000) / 1000; // Округляем до трёх знаков
        cout << "Тест пройден, найденные корни: " << root << endl;
    }
}

int main(int argc, char* argv[]) {
    setlocale(LC_ALL, "RUS");
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    
        if (rank == 0) {
            cout << "Запуск тестов..." << endl;
        }

        // Запуск тестов
    test_root(rank, -3, 2, 3, 1);                       // Известный корень
    test_root(rank, -4, 4, 3, 2);                       // Двойной корень
    test_root(rank, 1, 1, 3, numeric_limits<double>::max()); // Без корня
    test_root(rank, 0, -1, 3, -1);                      // Множественные корни

    // Синхронизация процессов
    MPI_Barrier(MPI_COMM_WORLD);
        if (rank == 0) {
            
            cout << "Все тесты пройдены!" << endl;
        }


    MPI_Finalize();
    return 0;
}
