#include <iostream>
#include <cassert>

void test_root(double A, double B, double Z, double expected_root) {
// Здесь можно вызвать вашу MPI программу и передать параметры
// Например, через систему или с использованием библиотеки для запуска MPI
// В этом примере просто выводим ожидаемые результаты
std::cout << "Testing with A=" << A << ", B=" << B << ", Z=" << Z << std::endl;

// Здесь будет логика проверки результата
// Например, если результат совпадает с expected_root, то тест пройден
// assert(computed_root == expected_root);
}

int main() {
// Тесты на правильность
test_root(-3, 2, 3, 1); // Известный корень
test_root(-4, 4, 3, 2); // Двойной корень
test_root(1, 1, 3, std::numeric_limits<double>::max()); // Без корня
test_root(0, -1, 3, -1); // Множественные корни
test_root(0, 0, 0, 0); // Границы

// Тесты на эффективность
// Здесь можно реализовать тесты на производительность

return 0;
}
