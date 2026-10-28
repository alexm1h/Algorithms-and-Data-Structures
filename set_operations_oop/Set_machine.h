// Set_machine.h - реализация через машинные слова
#ifndef SET_MACHINE_H
#define SET_MACHINE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <iostream>

using namespace std;

#define N 16
#define UNIVERSUM "0123456789ABCDEF"

// Класс - значение
class Set {
private:
    static int cnt;
    int n;
    char S;
    unsigned short word;

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
            if (word & (1 << i)) n++;
        }
    }

    void wordToArray(char* array) const {
        int index = 0;
        for (int i = 0; i < N; ++i) {
            if (word & (1 << i)) {
                array[index++] = indexToChar(i);
            }
        }
        array[index] = '\0';
    }

public:
    // 1. Конструктор по умолчанию
    Set() : n(0), S('A' + (cnt % 26)), word(0) {
        cnt++;
    }

    // Конструктор со случайной генерацией
    Set(char) : S('A' + (cnt % 26)), n(0), word(0) {
        cnt++;
        for (int i = 0; i < N; ++i) {
            if (rand() % 2) {
                word |= (1 << i);
                n++;
            }
        }
        char temp[N + 1];
        wordToArray(temp);
        cout << S << " = [" << temp << "]" << endl;
    }

    // Убраны явные конструктор копирования и операторы присваивания -
    // компилятор сгенерирует их автоматически

    Set operator|(const Set& B) const {
        Set C;
        C.word = word | B.word;
        C.updatePower();
        return C;
    }

    Set operator&(const Set& B) const {
        Set C;
        C.word = word & B.word;
        C.updatePower();
        return C;
    }

    Set operator~() const {
        Set C;
        C.word = ~word & ((1 << N) - 1);
        C.updatePower();
        return C;
    }

    Set& operator|=(const Set& B) {
        word |= B.word;
        updatePower();
        return *this;
    }

    Set& operator&=(const Set& B) {
        word &= B.word;
        updatePower();
        return *this;
    }

    void Show() {
        char temp[N + 1];
        wordToArray(temp);
        cout << S << " = [" << temp << "]" << endl;
    }

    void Out() { Show(); }

    int power() { return n; }
};

int Set::cnt = 0;

#endif
