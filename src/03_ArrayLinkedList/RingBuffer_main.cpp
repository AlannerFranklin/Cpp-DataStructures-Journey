#include <iostream>
#include <cassert>
#include "RingBuffer.hpp"

void testBasicPushPop() {
    std::cout << "[Test] Basic Push/Pop" << std::endl;
    RingBuffer<int> rb(3);
    
    assert(rb.isEmpty());
    rb.push(1);
    rb.push(2);
    rb.push(3);
    assert(rb.isFull());
    rb.print(); // Expect: [1, 2, 3]

    assert(rb.pop() == 1);
    assert(rb.pop() == 2);
    assert(rb.pop() == 3);
    assert(rb.isEmpty());
    assert(rb.pop() == std::nullopt);
    
    std::cout << "PASS" << std::endl;
}

void testOverwrite() {
    std::cout << "\n[Test] Overwrite (Circular)" << std::endl;
    RingBuffer<int> rb(3);
    
    rb.push(1);
    rb.push(2);
    rb.push(3);
    rb.print(); // [1, 2, 3]

    // 插入第 4 个，应该覆盖 1
    std::cout << "Push 4 (Should overwrite 1)" << std::endl;
    rb.push(4); 
    rb.print(); // Expect: [2, 3, 4]
    
    assert(rb.pop() == 2); // 最老的是 2
    assert(rb.pop() == 3);
    assert(rb.pop() == 4);
    assert(rb.isEmpty());

    std::cout << "PASS" << std::endl;
}

int main() {
    testBasicPushPop();
    testOverwrite();
    return 0;
}
