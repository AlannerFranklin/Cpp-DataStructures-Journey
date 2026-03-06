#ifndef MY_VECTOR_HPP
#define MY_VECTOR_HPP

#include <iostream>
#include <stdexcept>
#include <algorithm> // for std::copy
#include <memory>    // for std::unique_ptr (C++11/14)

// 使用模板 T，使其可以存储任意类型的数据
template <typename T>
class MyVector {
private:
    // C++17 风格：使用智能指针管理动态数组，避免手动 delete[]
    // std::unique_ptr<T[]> 在 C++11 引入，但在 C++14/17 中配合 make_unique 更常用
    std::unique_ptr<T[]> data;
    int size;       // 当前元素数量
    int capacity;   // 数组容量

    // 调整数组容量
    void resize(int newCapacity) {
        // 创建新数组 (使用 unique_ptr 自动管理内存)
        std::unique_ptr<T[]> newData = std::make_unique<T[]>(newCapacity);
        
        // 将旧数组数据复制到新数组
        // C++17 之前可能用 std::copy，这里手动循环也没问题
        for (int i = 0; i < size; i++) {
            newData[i] = data[i];
        }
        
        // 更新指针 (旧数组会自动析构释放，无需 delete[])
        data = std::move(newData);
        capacity = newCapacity;
    }

public:
    // 构造函数：初始化默认容量
    MyVector(int initCapacity = 10) {
        if (initCapacity <= 0) {
            throw std::invalid_argument("Capacity must be positive");
        }
        capacity = initCapacity;
        size = 0;
        // C++14 引入 make_unique，不仅安全而且简洁
        data = std::make_unique<T[]>(capacity);
    }

    // 析构函数：由于使用了 unique_ptr，不再需要手动写 ~MyVector()
    // 编译器生成的默认析构函数会自动调用 data 的析构函数
    ~MyVector() = default;

    // 禁止拷贝构造和拷贝赋值 (因为 unique_ptr 不能拷贝)
    // 如果需要拷贝，必须手动实现深拷贝逻辑 (Rule of Five)
    MyVector(const MyVector&) = delete;
    MyVector& operator=(const MyVector&) = delete;

    // 允许移动构造 (Move Constructor) - C++11 特性
    MyVector(MyVector&&) = default;
    MyVector& operator=(MyVector&&) = default;

    // [[nodiscard]] 是 C++17 特性：如果调用者忽略了返回值，编译器会警告
    // 提醒调用者：获取 size 而不使用通常是逻辑错误
    [[nodiscard]] int getSize() const {
        return size;
    }

    [[nodiscard]] int getCapacity() const {
        return capacity;
    }

    [[nodiscard]] bool isEmpty() const {
        return size == 0;
    }

    // 向数组末尾添加元素
    void addLast(T e) {
        add(size, e);
    }

    // 向数组开头添加元素
    void addFirst(T e) {
        add(0, e);
    }

    // 在 index 位置插入元素 e
    void add(int index, T e) {
        if (index < 0 || index > size) {
            throw std::out_of_range("Add failed. Index is illegal.");
        }

        // 如果当前元素数量达到容量，进行扩容 (2倍)
        if (size == capacity) {
            resize(2 * capacity);
        }

        // 将 index 及其之后的元素向后移动一位
        for (int i = size - 1; i >= index; i--) {
            data[i + 1] = data[i];
        }

        data[index] = e;
        size++;
    }

    // 获取 index 索引位置的元素
    [[nodiscard]] T get(int index) const {
        if (index < 0 || index >= size) {
            throw std::out_of_range("Get failed. Index is illegal.");
        }
        return data[index];
    }

    // 修改 index 索引位置的元素为 e
    void set(int index, T e) {
        if (index < 0 || index >= size) {
            throw std::out_of_range("Set failed. Index is illegal.");
        }
        data[index] = e;
    }

    // 查找数组中是否有元素 e
    [[nodiscard]] bool contains(T e) const {
        for (int i = 0; i < size; i++) {
            if (data[i] == e) {
                return true;
            }
        }
        return false;
    }

    // 查找数组中元素 e 所在的索引，如果不存在元素 e，则返回 -1
    [[nodiscard]] int find(T e) const {
        for (int i = 0; i < size; i++) {
            if (data[i] == e) {
                return i;
            }
        }
        return -1;
    }

    // 删除 index 索引位置的元素，返回删除的元素
    T remove(int index) {
        if (index < 0 || index >= size) {
            throw std::out_of_range("Remove failed. Index is illegal.");
        }

        T ret = data[index];

        // 将 index 之后的元素向前移动一位
        for (int i = index + 1; i < size; i++) {
            data[i - 1] = data[i];
        }

        size--;

        // 如果当前元素数量是容量的 1/4，并且容量大于 1，则缩容 (1/2)
        // 懒缩容策略：防止复杂度震荡
        if (size == capacity / 4 && capacity / 2 != 0) {
            resize(capacity / 2);
        }

        return ret;
    }

    // 删除第一个元素
    T removeFirst() {
        return remove(0);
    }

    // 删除最后一个元素
    T removeLast() {
        return remove(size - 1);
    }

    // 从数组中删除元素 e
    void removeElement(T e) {
        int index = find(e);
        if (index != -1) {
            remove(index);
        }
    }

    // 打印数组信息 (用于调试)
    void print() const {
        std::cout << "MyVector: size = " << size << ", capacity = " << capacity << std::endl;
        std::cout << "[";
        for (int i = 0; i < size; i++) {
            std::cout << data[i];
            if (i != size - 1) {
                std::cout << ", ";
            }
        }
        std::cout << "]" << std::endl;
    }
};

#endif // MY_VECTOR_HPP
