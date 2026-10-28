#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <windows.h>
#include <iostream>

using namespace std;

const long TEST_ITERATIONS = 100000;

// Раскомментируй нужную реализацию:
//#include "Set_array.h"      // Массивы
#include "Set_list.h"       // Списки
//#include "Set_bitarray.h"   // Битовые массивы
//#include "Set_machine.h"    // Машинные слова

int main() {
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
    srand(time(nullptr));

    printf("=== ОПЕРАЦИИ НАД МНОЖЕСТВАМИ ===\n");
    printf("Множество, содержащее цифры, имеющиеся в A или B, но отсутствующие и в C, и в D\n");
    printf("Универсум: 0123456789ABCDEF\n");
    printf("Количество тестовых итераций: %d\n\n", TEST_ITERATIONS);

    cout << "=== СОЗДАНИЕ МНОЖЕСТВ ===" << endl;
    Set A('A'), B('B'), C('C'), D('D'), E;

    cout << "\n=== ВЫЧИСЛЕНИЕ E = (A | B) & ~(C | D) ===" << endl;

    clock_t begin = clock();
    for (long q = 0; q < TEST_ITERATIONS; q++) {
        E = (A | B) & ~(C | D);
    }
    clock_t end = clock();

    cout << "\n=== РЕЗУЛЬТАТЫ ===" << endl;
    E.Out();
    cout << "Средняя мощность = "
         << (A.power() + B.power() + C.power() + D.power() + E.power()) / 5
         << "\nВремя = " << (end - begin) << " тактов для " << TEST_ITERATIONS << " итераций"
         << "\nВремя на операцию = " << (double)(end - begin) / TEST_ITERATIONS << " тактов" << endl;

    cout << "\nНажмите Enter для выхода...";
    cin.get();
    return 0;
}
