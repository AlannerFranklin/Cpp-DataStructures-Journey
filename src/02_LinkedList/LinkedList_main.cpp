#include <iostream>
#include "LinkedList.hpp"

int main() {
    std::cout << "=== Start Testing LinkedList ===" << std::endl;

    MyLinkedList<int> list;

    // 1. 测试 addFirst
    std::cout << "\n[Test] addFirst(0...4)" << std::endl;
    for (int i = 0; i < 5; i++) {
        list.addFirst(i);
        std::cout << list.getFirst() << " added." << std::endl;
    }
    // 应该输出: [4 -> 3 -> 2 -> 1 -> 0]
    list.print();

    // 2. 测试 addLast
    std::cout << "\n[Test] addLast(10)" << std::endl;
    list.addLast(10);
    list.print(); // [4 -> 3 -> 2 -> 1 -> 0 -> 10]

    // 3. 测试 add (index)
    std::cout << "\n[Test] add(2, 666)" << std::endl;
    list.add(2, 666);
    list.print(); // [4 -> 3 -> 666 -> 2 -> 1 -> 0 -> 10]

    // 4. 测试 get / set
    std::cout << "\n[Test] get(2) and set(2, 888)" << std::endl;
    std::cout << "Element at index 2: " << list.get(2) << std::endl;
    list.set(2, 888);
    std::cout << "After set(2, 888): ";
    list.print();

    // 5. 测试 remove
    std::cout << "\n[Test] remove(2)" << std::endl;
    list.remove(2);
    list.print(); // [4 -> 3 -> 2 -> 1 -> 0 -> 10]

    // 6. 测试 removeFirst / removeLast
    std::cout << "\n[Test] removeFirst() and removeLast()" << std::endl;
    list.removeFirst();
    list.removeLast();
    list.print(); // [3 -> 2 -> 1 -> 0]

    // 7. 测试 removeElement
    std::cout << "\n[Test] removeElement(2)" << std::endl;
    list.removeElement(2);
    list.print(); // [3 -> 1 -> 0]

    // 8. 测试异常处理
    std::cout << "\n[Test] Exception Handling" << std::endl;
    try {
        // 显式忽略返回值以消除 [[nodiscard]] 警告，或者打印它
        // (void)list.get(100); 
        // 更好的方式是模拟真实使用场景：尝试获取值
        int val = list.get(100);
        std::cout << "Should not reach here: " << val << std::endl;
    } catch (const std::out_of_range& e) {
        std::cout << "Caught expected exception: " << e.what() << std::endl;
    }

    std::cout << "\n=== All Tests Passed ===" << std::endl;

    return 0;
}
