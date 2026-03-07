#include <iostream>
#include "DoubleLinkedList.hpp"

int main() {
    std::cout << "=== Testing DoubleLinkedList (C++17) ===" << std::endl;

    DoubleLinkedList<int> list;

    // 1. 测试头插法
    std::cout << "\n[Test 1] addFirst: 3, 2, 1" << std::endl;
    list.addFirst(3);
    list.addFirst(2);
    list.addFirst(1);
    list.print(); 
    // Expect: Forward: [1 <-> 2 <-> 3], Reverse: [3 <-> 2 <-> 1]

    // 2. 测试尾插法
    std::cout << "\n[Test 2] addLast: 4, 5" << std::endl;
    list.addLast(4);
    list.addLast(5);
    list.print();
    // Expect: Forward: [1 <-> 2 <-> 3 <-> 4 <-> 5]

    // 3. 测试随机插入
    std::cout << "\n[Test 3] add(2, 99) -> Insert 99 at index 2" << std::endl;
    list.add(2, 99);
    list.print();
    // Expect: Forward: [1 <-> 2 <-> 99 <-> 3 <-> 4 <-> 5]

    // 4. 测试删除
    std::cout << "\n[Test 4] remove(2) -> Remove 99" << std::endl;
    int removed = list.remove(2);
    std::cout << "Removed: " << removed << std::endl;
    list.print();
    // Expect: Forward: [1 <-> 2 <-> 3 <-> 4 <-> 5]

    // 5. 测试头尾删除
    std::cout << "\n[Test 5] removeFirst & removeLast" << std::endl;
    list.removeFirst(); // remove 1
    list.removeLast();  // remove 5
    list.print();
    // Expect: Forward: [2 <-> 3 <-> 4]

    // 6. 测试乱序与排序
    std::cout << "\n[Test 6] Sort Test" << std::endl;
    list.addLast(1);
    list.addFirst(10);
    list.add(2, 0);
    std::cout << "Before Sort: ";
    list.print();
    // 可能乱序，例如 [10 <-> 2 <-> 0 <-> 3 <-> 4 <-> 1]

    std::cout << "Sorting (Recursive)..." << std::endl;
    list.sort(false); // Default
    
    std::cout << "After Sort (Recursive):  ";
    list.print();
    // Expect: 有序，且 Reverse 必须正确，证明 prev 指针也被正确维护了

    // 6.5 测试非递归排序
    std::cout << "\n[Test 6.5] Sort Test (Iterative)" << std::endl;
    list.addLast(5);
    list.addLast(1);
    list.addLast(3);
    std::cout << "Before Sort (Iterative): ";
    list.print();
    
    std::cout << "Sorting (Iterative)..." << std::endl;
    list.sort(true); // Iterative
    
    std::cout << "After Sort (Iterative):  ";
    list.print();
    // Expect: 有序

    // 7. 测试大数据量排序 (验证性能和稳定性)
    std::cout << "\n[Test 7] Large Random Sort" << std::endl;
    DoubleLinkedList<int> sortList;
    sortList.addLast(5);
    sortList.addLast(2);
    sortList.addLast(9);
    sortList.addLast(1);
    sortList.addLast(5); // 重复元素
    sortList.addLast(6);
    std::cout << "Input: "; sortList.print();
    sortList.sort();
    std::cout << "Output:"; sortList.print();

    return 0;
}
