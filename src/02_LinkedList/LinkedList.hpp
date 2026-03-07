#ifndef MY_LINKED_LIST_HPP
#define MY_LINKED_LIST_HPP

#include <iostream>
#include <stdexcept>
#include <memory> // for std::unique_ptr

// 模板类，支持泛型
template <typename T>
class MyLinkedList {
private:
    // 内部节点结构
    struct Node {
        T data;
        std::unique_ptr<Node> next; // 使用 unique_ptr 管理下一个节点的生命周期

        // 构造函数
        Node(T val) : data(val), next(nullptr) {}
    };

    // 虚拟头节点 (Dummy Head)，不存储实际数据，简化插入删除逻辑
    std::unique_ptr<Node> dummyHead;
    int size;

public:
    // 构造函数
    MyLinkedList() {
        dummyHead = std::make_unique<Node>(T()); // 初始化 dummyHead
        size = 0;
    }

    // 析构函数：由于使用了 unique_ptr，无需手动 delete 链表节点
    // unique_ptr 会在析构时递归释放链表，但为了防止栈溢出（Stack Overflow），
    // 对于特别长的链表，建议手动循环释放。这里对于一般学习用途，递归释放也足够，
    // 但为了严谨，我们显式实现迭代析构。
    ~MyLinkedList() {
        while (dummyHead->next) {
            dummyHead->next = std::move(dummyHead->next->next);
        }
    }

    // 获取链表元素个数 [[nodiscard]] C++17
    [[nodiscard]] int getSize() const {
        return size;
    }

    // 判断链表是否为空 [[nodiscard]] C++17
    [[nodiscard]] bool isEmpty() const {
        return size == 0;
    }

    // 在链表头添加元素
    void addFirst(T e) {
        add(0, e);
    }

    // 在链表尾添加元素
    // 注意：单链表添加尾部如果不维护 tail 指针，复杂度是 O(n)
    void addLast(T e) {
        add(size, e);
    }

    // 在 index 位置插入元素 e
    void add(int index, T e) {
        if (index < 0 || index > size) {
            throw std::out_of_range("Add failed. Illegal index.");
        }

        Node* prev = dummyHead.get();
        // 找到 index 前一个节点
        for (int i = 0; i < index; i++) {
            prev = prev->next.get();
        }

        // 创建新节点
        std::unique_ptr<Node> newNode = std::make_unique<Node>(e);
        // 新节点指向 prev 的下一个节点 (所有权转移)
        newNode->next = std::move(prev->next);
        // prev 指向新节点 (所有权转移)
        prev->next = std::move(newNode);

        size++;
    }

    // 获取 index 位置的元素
    [[nodiscard]] T get(int index) const {
        if (index < 0 || index >= size) {
            throw std::out_of_range("Get failed. Illegal index.");
        }

        Node* cur = dummyHead->next.get();
        for (int i = 0; i < index; i++) {
            cur = cur->next.get();
        }
        return cur->data;
    }

    // 获取第一个元素
    [[nodiscard]] T getFirst() const {
        return get(0);
    }

    // 获取最后一个元素
    [[nodiscard]] T getLast() const {
        return get(size - 1);
    }

    // 修改 index 位置的元素
    void set(int index, T e) {
        if (index < 0 || index >= size) {
            throw std::out_of_range("Set failed. Illegal index.");
        }

        Node* cur = dummyHead->next.get();
        for (int i = 0; i < index; i++) {
            cur = cur->next.get();
        }
        cur->data = e;
    }

    // 查找链表中是否存在元素 e
    [[nodiscard]] bool contains(T e) const {
        Node* cur = dummyHead->next.get();
        while (cur != nullptr) {
            if (cur->data == e) {
                return true;
            }
            cur = cur->next.get();
        }
        return false;
    }

    // 删除 index 位置的元素，返回删除的元素
    T remove(int index) {
        if (index < 0 || index >= size) {
            throw std::out_of_range("Remove failed. Illegal index.");
        }

        Node* prev = dummyHead.get();
        for (int i = 0; i < index; i++) {
            prev = prev->next.get();
        }

        // 获取要删除的节点 (unique_ptr)
        std::unique_ptr<Node> retNode = std::move(prev->next);
        // prev 指向要删除节点的下一个节点
        prev->next = std::move(retNode->next);
        
        size--;
        return retNode->data;
    }

    // 删除第一个元素
    T removeFirst() {
        return remove(0);
    }

    // 删除最后一个元素
    T removeLast() {
        return remove(size - 1);
    }

    // 从链表中删除元素 e
    void removeElement(T e) {
        Node* prev = dummyHead.get();
        while (prev->next != nullptr) {
            if (prev->next->data == e) {
                // 找到目标节点的前一个节点 prev
                std::unique_ptr<Node> delNode = std::move(prev->next);
                prev->next = std::move(delNode->next);
                size--;
                return; // 删除一个后直接返回，如果要删除所有 e，则去掉 return
            }
            prev = prev->next.get();
        }
    }

    // 打印链表
    void print() const {
        Node* cur = dummyHead->next.get();
        std::cout << "LinkedList: size = " << size << "\n[";
        while (cur != nullptr) {
            std::cout << cur->data;
            cur = cur->next.get();
            if (cur != nullptr) {
                std::cout << " -> ";
            }
        }
        std::cout << "]" << std::endl;
    }
};

#endif // MY_LINKED_LIST_HPP
