#include <Windows.h>
#include <iostream>
#include <vector>
#include <chrono>
#include <cstdlib>
#include <ctime>
#include <stdexcept>
#include <algorithm>

using namespace std;

// Узел дерева с хранением высоты (ДДПв)
template<typename T>
struct TreeNode {
    T key;
    int height;
    TreeNode* left;
    TreeNode* right;
    TreeNode* next;     // для последовательности

    TreeNode(const T& k) : key(k), height(1), left(nullptr), right(nullptr), next(nullptr) {}
};

// AVL-дерево (ДДПв с автобалансировкой)
template<typename T>
class AVLTree {
private:
    TreeNode<T>* root;
    TreeNode<T>* sequenceHead;
    TreeNode<T>* sequenceTail;
    size_t treeSize;

    int getHeight(TreeNode<T>* node) {
        return node ? node->height : 0;
    }

    void updateHeight(TreeNode<T>* node) {
        if (node) {
            node->height = 1 + max(getHeight(node->left), getHeight(node->right));
        }
    }

    int getBalance(TreeNode<T>* node) {
        return node ? getHeight(node->left) - getHeight(node->right) : 0;
    }

    // Правый поворот
    TreeNode<T>* rotateRight(TreeNode<T>* y) {
        TreeNode<T>* x = y->left;
        TreeNode<T>* T2 = x->right;
        x->right = y;
        y->left = T2;
        updateHeight(y);
        updateHeight(x);
        return x;
    }

    // Левый поворот
    TreeNode<T>* rotateLeft(TreeNode<T>* x) {
        TreeNode<T>* y = x->right;
        TreeNode<T>* T2 = y->left;
        y->left = x;
        x->right = T2;
        updateHeight(x);
        updateHeight(y);
        return y;
    }

    // Балансировка узла
    TreeNode<T>* balance(TreeNode<T>* node) {
        if (!node) return nullptr;

        updateHeight(node);
        int balance = getBalance(node);

        // Левое поддерево тяжелее
        if (balance > 1) {
            if (getBalance(node->left) < 0) {
                node->left = rotateLeft(node->left);
            }
            return rotateRight(node);
        }

        // Правое поддерево тяжелее
        if (balance < -1) {
            if (getBalance(node->right) > 0) {
                node->right = rotateRight(node->right);
            }
            return rotateLeft(node);
        }

        return node;
    }

    // Вставка узла
    TreeNode<T>* insertNode(TreeNode<T>* node, const T& key) {
        if (!node) {
            TreeNode<T>* newNode = new TreeNode<T>(key);
            // Добавление в последовательность
            if (!sequenceHead) {
                sequenceHead = sequenceTail = newNode;
            } else {
                sequenceTail->next = newNode;
                sequenceTail = newNode;
            }
            treeSize++;
            return newNode;
        }

        if (key < node->key) {
            node->left = insertNode(node->left, key);
        } else if (key > node->key) {
            node->right = insertNode(node->right, key);
        } else {
            return node;
        }

        return balance(node);
    }

    // Поиск минимального узла
    TreeNode<T>* findMin(TreeNode<T>* node) {
        while (node && node->left) node = node->left;
        return node;
    }

    // Удаление узла
    TreeNode<T>* removeNode(TreeNode<T>* node, const T& key) {
        if (!node) return nullptr;

        if (key < node->key) {
            node->left = removeNode(node->left, key);
        } else if (key > node->key) {
            node->right = removeNode(node->right, key);
        } else {
            if (!node->left || !node->right) {
                TreeNode<T>* temp = node->left ? node->left : node->right;
                delete node;
                treeSize--;
                return temp;
            } else {
                TreeNode<T>* temp = findMin(node->right);
                node->key = temp->key;
                node->right = removeNode(node->right, temp->key);
            }
        }

        return balance(node);
    }

    // Поиск узла
    bool containsNode(TreeNode<T>* node, const T& key) const {
        if (!node) return false;
        if (key < node->key) return containsNode(node->left, key);
        if (key > node->key) return containsNode(node->right, key);
        return true;
    }

    // Симметричный обход для получения упорядоченного вектора
    void inorderCollect(TreeNode<T>* node, vector<T>& result) const {
        if (!node) return;
        inorderCollect(node->left, result);
        result.push_back(node->key);
        inorderCollect(node->right, result);
    }

    // Уничтожение дерева
    void destroyTree(TreeNode<T>* node) {
        if (!node) return;
        destroyTree(node->left);
        destroyTree(node->right);
        delete node;
    }

public:
    AVLTree() : root(nullptr), sequenceHead(nullptr), sequenceTail(nullptr), treeSize(0) {}

    ~AVLTree() {
        destroyTree(root);
    }

    void insert(const T& key) {
        if (!contains(key)) {
            root = insertNode(root, key);
        }
    }

    bool remove(const T& key) {
        if (contains(key)) {
            // Удаление из последовательности
            TreeNode<T>* prev = nullptr;
            TreeNode<T>* curr = sequenceHead;
            while (curr && curr->key != key) {
                prev = curr;
                curr = curr->next;
            }
            if (curr) {
                if (prev) prev->next = curr->next;
                else sequenceHead = curr->next;
                if (curr == sequenceTail) sequenceTail = prev;
            }

            root = removeNode(root, key);
            return true;
        }
        return false;
    }

    bool contains(const T& key) const {
        return containsNode(root, key);
    }

    size_t size() const {
        return treeSize;
    }

    bool empty() const {
        return treeSize == 0;
    }

    vector<T> toVector() const {
        vector<T> result;
        inorderCollect(root, result);
        return result;
    }

    vector<T> getSequence() const {
        vector<T> result;
        TreeNode<T>* curr = sequenceHead;
        while (curr) {
            result.push_back(curr->key);
            curr = curr->next;
        }
        return result;
    }

    void setSequence(const vector<T>& seq) {
        sequenceHead = sequenceTail = nullptr;
        for (const auto& key : seq) {
            TreeNode<T>* newNode = new TreeNode<T>(key);
            if (!sequenceHead) {
                sequenceHead = sequenceTail = newNode;
            } else {
                sequenceTail->next = newNode;
                sequenceTail = newNode;
            }
        }
    }

    // ERASE - удаление элементов с позиции p1 по p2
    void eraseSequence(size_t p1, size_t p2) {
        vector<T> seq = getSequence();
        if (p1 >= seq.size() || p2 >= seq.size() || p1 > p2) {
            throw out_of_range("Invalid positions for ERASE");
        }

        vector<T> newSeq;
        for (size_t i = 0; i < seq.size(); i++) {
            if (i < p1 || i > p2) {
                newSeq.push_back(seq[i]);
            } else {
                remove(seq[i]);
            }
        }

        sequenceHead = sequenceTail = nullptr;
        for (const auto& key : newSeq) {
            TreeNode<T>* newNode = new TreeNode<T>(key);
            if (!sequenceHead) {
                sequenceHead = sequenceTail = newNode;
            } else {
                sequenceTail->next = newNode;
                sequenceTail = newNode;
            }
        }
    }

    // SUBST - вставка последовательности в позицию p
    void substSequence(const vector<T>& seqB, size_t p) {
        vector<T> seqA = getSequence();
        if (p > seqA.size()) {
            throw out_of_range("Invalid position for SUBST");
        }

        vector<T> newSeq;
        for (size_t i = 0; i < p; i++) {
            newSeq.push_back(seqA[i]);
        }
        for (const auto& key : seqB) {
            newSeq.push_back(key);
            insert(key);
        }
        for (size_t i = p; i < seqA.size(); i++) {
            newSeq.push_back(seqA[i]);
        }

        setSequence(newSeq);
    }

    // CHANGE - замена элементов с позиции p последовательностью B
    void changeSequence(const vector<T>& seqB, size_t p) {
        vector<T> seqA = getSequence();
        if (p > seqA.size()) {
            throw out_of_range("Invalid position for CHANGE");
        }

        for (size_t i = p; i < seqA.size() && i < p + seqB.size(); i++) {
            remove(seqA[i]);
        }

        vector<T> newSeq;
        for (size_t i = 0; i < p; i++) {
            newSeq.push_back(seqA[i]);
        }
        for (const auto& key : seqB) {
            newSeq.push_back(key);
            insert(key);
        }
        for (size_t i = p + seqB.size(); i < seqA.size(); i++) {
            newSeq.push_back(seqA[i]);
        }

        setSequence(newSeq);
    }

    void print() const {
        vector<T> vec = toVector();
        cout << "Множество (упорядоченное): { ";
        for (const auto& key : vec) {
            cout << key << " ";
        }
        cout << "}" << endl;

        vector<T> seq = getSequence();
        cout << "Последовательность (порядок вставки): < ";
        for (const auto& key : seq) {
            cout << key << " ";
        }
        cout << ">" << endl;
    }
};

// ============= ОПЕРАЦИИ НАД МНОЖЕСТВАМИ =============

template<typename T>
AVLTree<T> intersection(const AVLTree<T>& A, const AVLTree<T>& B) {
    AVLTree<T> result;
    vector<T> vecA = A.toVector();

    for (const auto& key : vecA) {
        if (B.contains(key)) {
            result.insert(key);
        }
    }
    return result;
}

template<typename T>
AVLTree<T> symmetricDifference(const AVLTree<T>& A, const AVLTree<T>& B) {
    AVLTree<T> result;
    vector<T> vecA = A.toVector();
    vector<T> vecB = B.toVector();

    for (const auto& key : vecA) {
        if (!B.contains(key)) {
            result.insert(key);
        }
    }
    for (const auto& key : vecB) {
        if (!A.contains(key)) {
            result.insert(key);
        }
    }
    return result;
}

template<typename T>
AVLTree<T> difference(const AVLTree<T>& A, const AVLTree<T>& B) {
    AVLTree<T> result;
    vector<T> vecA = A.toVector();

    for (const auto& key : vecA) {
        if (!B.contains(key)) {
            result.insert(key);
        }
    }
    return result;
}

// ============= ВЫЧИСЛЕНИЕ ВЫРАЖЕНИЯ =============
// Выражение: ((A ⊕ (B ∩ C)) \ D) ∩ E

template<typename T>
AVLTree<T> evaluateExpressionSilent(const AVLTree<T>& A, const AVLTree<T>& B,
                                      const AVLTree<T>& C, const AVLTree<T>& D,
                                      const AVLTree<T>& E) {
    AVLTree<T> step1 = intersection(B, C);
    AVLTree<T> step2 = symmetricDifference(A, step1);
    AVLTree<T> step3 = difference(step2, D);
    AVLTree<T> result = intersection(step3, E);
    return result;
}

// ============= ГЕНЕРАЦИЯ СЛУЧАЙНОГО МНОЖЕСТВА =============
// Упрощённая версия с rand() по замечанию преподавателя

AVLTree<int> generateRandomSet(int size, int minVal, int maxVal) {
    AVLTree<int> result;

    for (int i = 0; i < size; i++) {
        int val = minVal + rand() % (maxVal - minVal + 1);
        result.insert(val);
    }
    return result;
}
// ============= ВЫЧИСЛЕНИЕ ВЫРАЖЕНИЯ С ВЫВОДОМ (для демонстрации) =============

template<typename T>
AVLTree<T> evaluateExpression(const AVLTree<T>& A, const AVLTree<T>& B,
                               const AVLTree<T>& C, const AVLTree<T>& D,
                               const AVLTree<T>& E) {

    cout << "\n1. B ∩ C:" << endl;
    AVLTree<T> step1 = intersection(B, C);
    step1.print();

    cout << "\n2. A ⊕ (B ∩ C):" << endl;
    AVLTree<T> step2 = symmetricDifference(A, step1);
    step2.print();

    cout << "\n3. (A ⊕ (B ∩ C)) \\ D:" << endl;
    AVLTree<T> step3 = difference(step2, D);
    step3.print();

    cout << "\n4. ((A ⊕ (B ∩ C)) \\ D) ∩ E:" << endl;
    AVLTree<T> result = intersection(step3, E);
    result.print();

    return result;
}
// ============= ЗАМЕР ВРЕМЕНИ ДЛЯ RG41 =============

void measureForRG41() {
    cout << "\n========================================" << endl;
    cout << "Эксперимент по измерению временной сложности" << endl;
    cout << "========================================" << endl;
    cout << "Формат: размер_входа время" << endl;
    cout << "(копируйте эти данные в файл in.txt)" << endl;
    cout << "========================================\n" << endl;

    // Размеры: от 10 до 5000
    vector<int> sizes = {10, 20, 30, 40, 50, 60, 70, 80, 90, 100,
                         150, 200, 300, 400, 500, 600, 800, 1000,
                         1500, 2000, 3000, 4000, 5000};

    for (int size : sizes) {
        // Генерация 5 множеств заданной мощности
        AVLTree<int> A = generateRandomSet(size, 0, 10000);
        AVLTree<int> B = generateRandomSet(size, 5000, 15000);
        AVLTree<int> C = generateRandomSet(size, 10000, 20000);
        AVLTree<int> D = generateRandomSet(size, 0, 10000);
        AVLTree<int> E = generateRandomSet(size, 5000, 15000);

        auto start = chrono::high_resolution_clock::now();

        // Вычисляем выражение
        AVLTree<int> result = evaluateExpressionSilent(A, B, C, D, E);

        auto end = chrono::high_resolution_clock::now();
        auto duration = chrono::duration_cast<chrono::microseconds>(end - start);

        cout << size << " " << duration.count() << endl;
    }
}

// ============= ОСНОВНАЯ ПРОГРАММА =============

int main() {
    SetConsoleOutputCP(CP_UTF8);
    // Инициализация генератора случайных чисел (упрощённо)
    srand(static_cast<unsigned>(time(nullptr)));

    cout << "========================================" << endl;
    cout << "Лабораторная работа №3" << endl;
    cout << "Вариант 16" << endl;
    cout << "Выражение: (A ⊕ B ∩ C) \\ D ∩ E" << endl;
    cout << "Правильный порядок: ((A ⊕ (B ∩ C)) \\ D) ∩ E" << endl;
    cout << "Структура: ДДПв (AVL-дерево с хранением высоты)" << endl;
    cout << "Операции с последовательностью: ERASE, SUBST, CHANGE" << endl;
    cout << "========================================" << endl;

    // ============= ДЕМОНСТРАЦИЯ ДЛЯ n = 32 (по заданию) =============

    cout << "\n=== Демонстрация для мощности 32 (по заданию) ===" << endl;

    AVLTree<int> A = generateRandomSet(32, 0, 100);
    AVLTree<int> B = generateRandomSet(32, 50, 150);
    AVLTree<int> C = generateRandomSet(32, 100, 200);
    AVLTree<int> D = generateRandomSet(32, 0, 100);
    AVLTree<int> E = generateRandomSet(32, 50, 150);

    cout << "\nМножество A:" << endl;
    A.print();
    cout << "\nМножество B:" << endl;
    B.print();
    cout << "\nМножество C:" << endl;
    C.print();
    cout << "\nМножество D:" << endl;
    D.print();
    cout << "\nМножество E:" << endl;
    E.print();

    AVLTree<int> result = evaluateExpression(A, B, C, D, E);

    cout << "\n========================================" << endl;
    cout << "РЕЗУЛЬТАТ ВЫЧИСЛЕНИЯ ВЫРАЖЕНИЯ:" << endl;
    cout << "========================================" << endl;
    result.print();

    // ============= ДЕМОНСТРАЦИЯ ОПЕРАЦИЙ С ПОСЛЕДОВАТЕЛЬНОСТЬЮ =============

    cout << "\n========================================" << endl;
    cout << "Демонстрация операций с последовательностью" << endl;
    cout << "========================================" << endl;

    // Создаём тестовую последовательность
    AVLTree<int> testTree;
    vector<int> testSeq;
    for (int i = 0; i < 10; i++) {
        int val = rand() % 100;
        testSeq.push_back(val);
        testTree.insert(val);
    }

    cout << "\nИсходная последовательность: < ";
    for (int key : testSeq) cout << key << " ";
    cout << ">" << endl;
    testTree.print();

    // ERASE
    cout << "\n--- ERASE(2, 5) ---" << endl;
    testTree.eraseSequence(2, 5);
    testTree.print();

    // SUBST
    cout << "\n--- SUBST([999,888,777], 3) ---" << endl;
    testTree.substSequence({999, 888, 777}, 3);
    testTree.print();

    // CHANGE
    cout << "\n--- CHANGE([111,222,333,444], 2) ---" << endl;
    testTree.changeSequence({111, 222, 333, 444}, 2);
    testTree.print();

    // ============= ЗАМЕР ВРЕМЕНИ ДЛЯ RG41 =============

    measureForRG41();

    cout << "\n========================================" << endl;
    cout << "Программа завершена!" << endl;
    cout << "========================================" << endl;

    return 0;
}
