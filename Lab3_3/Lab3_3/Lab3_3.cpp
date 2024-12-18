// mpi_root_finder.cpp

#include <mpi.h>
#include <iostream>
#include <cmath>
#include <limits>


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
        if (prev_value * curr_value <= 0) { // Проверка смены знака
            root = x - step / 2; // Приближение корня
            return true;
        }
        prev_value = curr_value;
    }
    return false; // Корень не найден
}


int main(int argc, char* argv[]) {
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    // Входные данные
    double A, B, Z;
    double step = 0.001; // Шаг поиска

    if (rank == 0) {
        // Главный процесс запрашивает параметры
        cout << "Введите A, B, Z: "<<endl;
        cin >> A >> B >> Z;
    }
    

    // Рассылаем параметры всем процессам
    MPI_Bcast(&A, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    MPI_Bcast(&B, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    MPI_Bcast(&Z, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    // Вычисляем подотрезки
    double interval_length = 2 * Z / size;
    double start = -Z + rank * interval_length;
    double end = start + interval_length;

    
    // Поиск корня на подотрезке
    double root = numeric_limits<double>::max();
    bool found = find_root_on_subinterval(start, end, step, A, B, root);

    
    // Собираем результаты от всех процессов
    double global_root = numeric_limits<double>::max();
    MPI_Reduce(&root, &global_root, 1, MPI_DOUBLE, MPI_MIN, 0, MPI_COMM_WORLD);

    
    // Вывод результата
    if (rank == 0) {
        if (global_root != numeric_limits<double>::max()) {
            cout << "Корень найден: " << global_root << endl;
        }
        else {
            cout << "Не найден корень на интервале [-" << Z << "; " << Z << "]" << endl;
        }
    }

    MPI_Finalize();
    return 0;
}
