// Set_list.h - реализация через списки
#ifndef SET_LIST_H
#define SET_LIST_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <iostream>

using namespace std;

#define N 16
#define UNIVERSUM "0123456789ABCDEF"

struct Node {
    char digit;
    Node* next;

    // Убрана перегрузка new/delete - используем стандартные
    Node(char d) : digit(d), next(nullptr) {}
};

// Класс - контроллер
class Set {
private:
    static int cnt;
    int n;
    char S;
    Node* head;

    void freeList() {
        Node* current = head;
        while (current) {
            Node* temp = current;
            current = current->next;
            delete temp;  // Используем delete вместо free
        }
        head = nullptr;
        n = 0;
    }

    void copyList(const Node* otherHead) {
        if (!otherHead) return;

        head = new Node(otherHead->digit);  // new вместо malloc
        Node* current = head;
        const Node* otherCurrent = otherHead->next;

        while (otherCurrent) {
            current->next = new Node(otherCurrent->digit);  // new вместо malloc
            current = current->next;
            otherCurrent = otherCurrent->next;
        }
    }

    bool isInList(char digit) const {
        Node* current = head;
        while (current) {
            if (current->digit == digit) return true;
            current = current->next;
        }
        return false;
    }

    void listToArray(char* array) const {
        int index = 0;
        Node* current = head;
        while (current && index < N) {
            array[index++] = current->digit;
            current = current->next;
        }
        array[index] = '\0';
    }

public:
    // 1. Конструктор по умолчанию
    Set() : n(0), S('A' + (cnt % 26)), head(nullptr) {
        cnt++;
    }

    // Конструктор со случайной генерацией
    Set(char) : S('A' + (cnt % 26)), n(0), head(nullptr) {
        cnt++;
        for (int i = 0; i < N; ++i) {
            if (rand() % 2 && !isInList(UNIVERSUM[i])) {
                Node* newNode = new Node(UNIVERSUM[i]);  // new вместо malloc
                if (!head) {
                    head = newNode;
                } else {
                    Node* current = head;
                    while (current->next) current = current->next;
                    current->next = newNode;
                }
                n++;
            }
        }
        char temp[N + 1];
        listToArray(temp);
        cout << S << " = [" << temp << "]" << endl;
    }

    // 2. Конструктор копирования
    Set(const Set& B) : S('A' + (cnt % 26)), n(B.n), head(nullptr) {
        cnt++;
        copyList(B.head);
    }

    // 3. Конструктор перемещения
    Set(Set&& B) noexcept : S(B.S), n(B.n), head(B.head) {
        cnt++;
        B.head = nullptr;
        B.n = 0;
    }

    // 4. Оператор копирующего присваивания
    Set& operator=(const Set& B) {
        if (this == &B) return *this;

        freeList();
        copyList(B.head);
        n = B.n;
        S = 'A' + (cnt % 26);
        cnt++;
        return *this;
    }

    // 5. Оператор перемещающего присваивания
    Set& operator=(Set&& B) noexcept {
        if (this == &B) return *this;

        freeList();
        head = B.head;
        n = B.n;
        S = B.S;
        B.head = nullptr;
        B.n = 0;
        return *this;
    }

    // 6. Деструктор
    ~Set() {
        freeList();  // ОСТАВЛЯЕМ - она теперь использует delete
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
            if (!isInList(UNIVERSUM[i])) {
                Node* newNode = new Node(UNIVERSUM[i]);  // new вместо malloc
                if (!C.head) {
                    C.head = newNode;
                } else {
                    Node* current = C.head;
                    while (current->next) current = current->next;
                    current->next = newNode;
                }
                C.n++;
            }
        }
        return C;
    }

    Set& operator|=(const Set& B) {
        Node* current = B.head;
        while (current) {
            if (!isInList(current->digit)) {
                Node* newNode = new Node(current->digit);  // new вместо malloc
                if (!head) {
                    head = newNode;
                } else {
                    Node* tail = head;
                    while (tail->next) tail = tail->next;
                    tail->next = newNode;
                }
                n++;
            }
            current = current->next;
        }
        return *this;
    }

    Set& operator&=(const Set& B) {
        Set C;
        Node* current = head;
        while (current) {
            if (B.isInList(current->digit)) {
                Node* newNode = new Node(current->digit);  // new вместо malloc
                if (!C.head) {
                    C.head = newNode;
                } else {
                    Node* tail = C.head;
                    while (tail->next) tail = tail->next;
                    tail->next = newNode;
                }
                C.n++;
            }
            current = current->next;
        }
        freeList();
        head = C.head;
        n = C.n;
        C.head = nullptr;
        return *this;
    }

    void Show() {
        char temp[N + 1];
        listToArray(temp);
        cout << S << " = [" << temp << "]" << endl;
    }

    void Out() { Show(); }

    int power() { return n; }
};

int Set::cnt = 0;

#endif
