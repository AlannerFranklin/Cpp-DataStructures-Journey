#include <iostream>
#include <cassert>
#include "SkipList.hpp"

void testSkipList() {
    std::cout << "[Test] SkipList Basic Operations" << std::endl;
    SkipList skipList(4); // Max level 4 for easy viewing

    std::cout << "Insert: 3, 6, 7, 9, 12, 19, 17, 26, 21, 25" << std::endl;
    skipList.insert(3, 30);
    skipList.insert(6, 60);
    skipList.insert(7, 70);
    skipList.insert(9, 90);
    skipList.insert(12, 120);
    skipList.insert(19, 190);
    skipList.insert(17, 170);
    skipList.insert(26, 260);
    skipList.insert(21, 210);
    skipList.insert(25, 250);

    skipList.print();

    std::cout << "\nSearch Test:" << std::endl;
    assert(skipList.search(19) == true);
    std::cout << "Search 19: Found (PASS)" << std::endl;
    
    assert(skipList.search(100) == false);
    std::cout << "Search 100: Not Found (PASS)" << std::endl;
}

int main() {
    testSkipList();
    return 0;
}
