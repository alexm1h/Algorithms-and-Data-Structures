// Set_bitarray.h - реализация через битовые массивы
#ifndef SET_BITARRAY_H
#define SET_BITARRAY_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <iostream>

using namespace std;

#define N 16
#define UNIVERSUM "0123456789ABCDEF"

// Класс - контроллер
class Set {
private:
    static int cnt;
    int n;
    char S;
    int* bitArray; // динамическая память (указатель)

    int charToIndex(char c) const {
        if (c >= '0' && c <= '9') return c - '0';
        return c - 'A' + 10;
    }

    char indexToChar(int index) const {
        if (index < 10) return '0' + index;
        return 'A' + index - 10;
    }

    void updatePower() {
        n = 0;
        for (int i = 0; i < N; ++i) {
            if (bitArray[i]) n++;
        }
    }

    void bitArrayToArray(char* array) const {
        int index = 0;
        for (int i = 0; i < N; ++i) {
            if (bitArray[i]) {
                array[index++] = indexToChar(i);
            }
        }
        array[index] = '\0';
    }

public:
    // 1. Конструктор по умолчанию
    Set() : n(0), S('A' + (cnt % 26)), bitArray(new int[N]) {
        cnt++;
        for (int i = 0; i < N; ++i) bitArray[i] = 0;
    }

    // Конструктор со случайной генерацией
    Set(char) : S('A' + (cnt % 26)), n(0), bitArray(new int[N]) {
        cnt++;
        for (int i = 0; i < N; ++i) {
            bitArray[i] = rand() % 2;
            if (bitArray[i]) n++;
        }
        char temp[N + 1];
        bitArrayToArray(temp);
        cout << S << " = [" << temp << "]" << endl;
    }

    // 2. Конструктор копирования
    Set(const Set& B) : S('A' + (cnt % 26)), n(B.n), bitArray(new int[N]) {
        cnt++;
        for (int i = 0; i < N; ++i) bitArray[i] = B.bitArray[i];
    }

    // 3. Конструктор перемещения
    Set(Set&& B) noexcept : S(B.S), n(B.n), bitArray(B.bitArray) {
        cnt++;
        B.bitArray = nullptr; // Забираем владение
        B.n = 0;
    }

    // 4. Оператор копирующего присваивания
    Set& operator=(const Set& B) {
        // Проверка самоприсваивания
        if (this == &B) return *this;

        n = B.n;
        for (int i = 0; i < N; ++i) bitArray[i] = B.bitArray[i];
        S = 'A' + (cnt % 26);
        cnt++;
        return *this;
    }

    // 5. Оператор перемещающего присваивания
    Set& operator=(Set&& B) noexcept {
        // Проверка самоприсваивания
        if (this == &B) return *this;

        // Освобождаем текущие ресурсы
        delete[] bitArray;

        // Забираем ресурсы из B
        bitArray = B.bitArray;
        n = B.n;
        S = B.S;

        // Обнуляем B
        B.bitArray = nullptr;
        B.n = 0;

        return *this;
    }

    // 6. Деструктор
    ~Set() {
        delete[] bitArray;
    }

    // Остальные операторы...
    Set operator|(const Set& B) const {
        Set C(*this);
        return (C |= B);
    }

    Set operator&(const Set& B) const {
        Set C(*this);
        return (C &= B);
    }

    Set operator~() const {
        Set C;
        for (int i = 0; i < N; ++i) {
            C.bitArray[i] = !bitArray[i];
        }
        C.updatePower();
        return C;
    }

    Set& operator|=(const Set& B) {
        for (int i = 0; i < N; ++i) {
            bitArray[i] = bitArray[i] || B.bitArray[i];
        }
        updatePower();
        return *this;
    }

    Set& operator&=(const Set& B) {
        for (int i = 0; i < N; ++i) {
            bitArray[i] = bitArray[i] && B.bitArray[i];
        }
        updatePower();
        return *this;
    }

    void Show() {
        char temp[N + 1];
        bitArrayToArray(temp);
        cout << S << " = [" << temp << "]" << endl;
    }

    void Out() { Show(); }

    int power() { return n; }
};

int Set::cnt = 0;

#endif
