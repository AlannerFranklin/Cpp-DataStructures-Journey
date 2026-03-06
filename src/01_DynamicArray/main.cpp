#include <iostream>
#include "MyVector.hpp"

int main() {
    // 创建一个初始容量为 10 的动态数组
    MyVector<int> arr(10);
    
    std::cout << "=== 测试添加操作 ===" << std::endl;
    for(int i = 0; i < 10; i++) {
        arr.addLast(i);
    }
    arr.print(); // [0, 1, 2, 3, 4, 5, 6, 7, 8, 9]

    std::cout << "\n=== 测试插入和扩容 ===" << std::endl;
    arr.add(1, 100); // 在索引 1 插入 100
    arr.print(); // 触发扩容: size=11, capacity=20
    
    arr.addFirst(-1); // 在开头插入 -1
    arr.print(); // size=12, capacity=20

    std::cout << "\n=== 测试删除和缩容 ===" << std::endl;
    arr.remove(2); // 删除索引 2 的元素
    arr.print();
    
    arr.removeElement(4); // 删除元素 4
    arr.print();

    arr.removeFirst(); // 删除第一个元素
    arr.print();

    // 连续删除直到触发缩容
    std::cout << "连续删除直到触发缩容..." << std::endl;
    while(arr.getSize() > 4) { // 只要大于 4 个就删，看缩容效果
        arr.removeLast();
    }
    arr.print(); // 应该看到 capacity 减小

    return 0;
}
