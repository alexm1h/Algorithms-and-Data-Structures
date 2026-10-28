// Set_array.h - реализация через массивы
#ifndef SET_ARRAY_H
#define SET_ARRAY_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <iostream>

using namespace std;

#define MAX_SET_SIZE 17
#define UNIVERSUM "0123456789ABCDEF"

// Класс - контроллер
class Set {
private:
    static int N, cnt;
    int n;
    char S;
    char* A; // динамическая память

    void removeDuplicates() {
        for (int i = 0; i < n; i++) {
            for (int j = i + 1; j < n; j++) {
                if (A[i] == A[j]) {
                    for (int k = j; k < n; k++) {
                        A[k] = A[k + 1];
                    }
                    n--;
                    j--;
                }
            }
        }
        A[n] = '\0';
    }

public:
    // 1. Конструктор по умолчанию
    Set() : n(0), S('A' + (cnt % 26)), A(new char[N + 1]) {
        cnt++;
        A[0] = '\0';
    }

    // Конструктор со случайной генерацией
    Set(char) : S('A' + (cnt % 26)), n(0), A(new char[N + 1]) {
        cnt++;
        for (int i = 0; i < N; ++i) {
            if (rand() % 2) A[n++] = UNIVERSUM[i];
        }
        A[n] = '\0';
        removeDuplicates();
        cout << S << " = [" << A << "]" << endl;
    }

    // 2. Конструктор копирования
    Set(const Set& B) : S('A' + (cnt % 26)), n(B.n), A(new char[N + 1]) {
        cnt++;
        strcpy(A, B.A);
    }

    // 3. Конструктор перемещения
    Set(Set&& B) noexcept : S(B.S), n(B.n), A(B.A) {
        cnt++;
        B.A = nullptr; // Забираем владение
        B.n = 0;
    }

    // 4. Оператор копирующего присваивания
    Set& operator=(const Set& B) {
        if (this == &B) return *this;

        n = B.n;
        delete[] A;
        A = new char[N + 1];
        strcpy(A, B.A);
        S = 'A' + (cnt % 26);
        cnt++;
        return *this;
    }

    // 5. Оператор перемещающего присваивания
    Set& operator=(Set&& B) noexcept {
        if (this == &B) return *this;

        delete[] A;
        A = B.A;
        n = B.n;
        S = B.S;
        B.A = nullptr;
        B.n = 0;
        return *this;
    }

    // 6. Деструктор
    ~Set() {
        delete[] A;
    }

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
            bool found = false;
            for (int j = 0; j < n; ++j) {
                if (UNIVERSUM[i] == A[j]) {
                    found = true;
                    break;
                }
            }
            if (!found) C.A[C.n++] = UNIVERSUM[i];
        }
        C.A[C.n] = '\0';
        return C;
    }

    Set& operator|=(const Set& B) {
        for (int i = 0; i < B.n; ++i) {
            bool found = false;
            for (int j = 0; j < n; ++j) {
                if (B.A[i] == A[j]) {
                    found = true;
                    break;
                }
            }
            if (!found) {
                if (n < N) A[n++] = B.A[i];
            }
        }
        A[n] = '\0';
        removeDuplicates();
        return *this;
    }

    Set& operator&=(const Set& B) {
        Set C(*this);
        n = 0;
        for (int i = 0; i < C.n; ++i) {
            for (int j = 0; j < B.n; ++j) {
                if (C.A[i] == B.A[j]) {
                    A[n++] = C.A[i];
                    break;
                }
            }
        }
        A[n] = '\0';
        return *this;
    }

    void Show() {
        cout << S << " = [" << A << "]" << endl;
    }

    void Out() { Show(); }

    int power() { return n; }
};

int Set::N = 16;
int Set::cnt = 0;

#endif
