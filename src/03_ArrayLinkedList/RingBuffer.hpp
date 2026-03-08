#ifndef RING_BUFFER_HPP
#define RING_BUFFER_HPP

#include <iostream>
#include <vector>
#include <stdexcept>
#include <optional> // C++17
#include <memory>
// 环形缓冲区 (Circular Buffer)
template <typename T>
class RingBuffer {
    // 待实现
private:
    // 内部节点结构
    std::unique_ptr<T[]> buffer; // 智能指针管理数组内存
    size_t capacity;             // 最大容量
    
    size_t head;                 // 读位置 (Read Index)
    size_t tail;                 // 写位置 (Write Index)
    size_t size;                 // 当前元素个数
public:
    // 构造函数：初始化容量，分配内存
    explicit RingBuffer(size_t cap) 
        : capacity(cap), head(0), tail(0), size(0) {
        
        // 分配数组内存
        buffer = std::make_unique<T[]>(capacity);
    }
    bool isEmpty() {
        return size == 0;
    }
    // 1. 状态判断
    // 注意：size 是 C++ 标准库常用名，但为了避免和成员变量 size 混淆，
    // 我们这里用 count 表示元素个数，capacity 表示总容量
    size_t getCount() const { return size; }
    size_t getCapacity() const { return capacity; }
    
    // isEmpty 已经有了，补全 isFull
    bool isFull() const {
        return size == capacity;
    }
    // 2. 入队 (Push) - 覆盖模式
    void push(T value) {
        // 1. 先判断满没满
        if (isFull()) {
            // 情况 A: 满了
            buffer[tail] = value;          // 覆盖当前 tail (也就是 head) 指向的老数据
            tail = (tail + 1) % capacity;  // 尾巴往前挪
            head = (head + 1) % capacity;  // 头也被迫往前挪 (因为老头被覆盖了)
            // size 不变，还是满的
        } else {
            // 情况 B: 没满
            buffer[tail] = value;          // 写入数据
            tail = (tail + 1) % capacity;  // 尾巴往前挪
            size++;                        // 数量 +1
        }
    }
    // 3. 出队 (Pop)
    std::optional<T> pop() {
        if (isEmpty()) {
            return std::nullopt; // 返回空值
        }

        T value = buffer[head];
        head = (head + 1) % capacity;
        size--;
        
        return value;
    }
    // 4. 调试打印 (Helper)
    void print() const {
        std::cout << "Buffer: [";
        for (size_t i = 0; i < size; ++i) {
            // 计算实际的物理索引：(head + i) % capacity
            size_t index = (head + i) % capacity;
            std::cout << buffer[index];
            if (i < size - 1) std::cout << ", ";
        }
        std::cout << "] (H=" << head << ", T=" << tail << ", N=" << size << ")" << std::endl;
    }
};

#endif
