#include <iostream>
#include <cstdlib>
#include <ctime>
#include "Tree.h"

int main() {
    std::srand(std::time(nullptr));

    Tree tree('a', 'z', 5, 20, 12, 80);

    std::cout << "=== TREES: variant 16 ===" << std::endl;
    std::cout << "Binary tree with reverse labeling" << std::endl;
    std::cout << "DFS traversal to count left leaves" << std::endl;
    std::cout << "Constraints: depth <= 5, nodes <= 20" << std::endl;
    std::cout << "=====================================" << std::endl;

    tree.generateTree();

    if (!tree.isEmpty()) {
        std::cout << "\nTREE VISUALIZATION:" << std::endl;
        std::cout << "(labels in reverse order)" << std::endl;
        tree.displayTree();

        int leftLeaves = tree.countLeftLeavesDFS();
        std::cout << "\nRESULTS:" << std::endl;
        std::cout << "Left leaves count: " << leftLeaves << std::endl;
        std::cout << "Total nodes: " << tree.getNodeCount() << std::endl;
        std::cout << "Tree height: " << tree.getTreeHeight() << std::endl;
    } else {
        std::cout << "Tree is empty!" << std::endl;
    }

    std::cout << "\nPress Enter to exit...";
    std::cin.get();
    return 0;
}
