#ifndef DOUBLE_LINKED_LIST_HPP
#define DOUBLE_LINKED_LIST_HPP

#include <iostream>
#include <stdexcept>
#include <memory> // unique_ptr
#include <vector> // for iterative sort

// 模板类，支持泛型
template <typename T>
class DoubleLinkedList {
private:
    // 内部节点结构
    struct Node {
        T data;
        std::unique_ptr<Node> next; // 强引用：拥有下一个节点 (Owner)
        Node* prev;                 // 弱引用：指向前一个节点 (Observer)，防止循环引用

        Node(T val) : data(val), next(nullptr), prev(nullptr) {}
    };

    std::unique_ptr<Node> head; // 头节点 (拥有整个链表的入口)
    Node* tail;                 // 尾节点 (弱引用，方便尾插)
    int size;

    // --- 归并排序辅助函数 (unique_ptr 版本) ---

    // 1. 分割链表：将 headNode 开头的链表切成两半，返回后半部分的头节点 (unique_ptr)
    //    核心逻辑：使用快慢指针找到中点，然后用 std::move 断开链接
    std::unique_ptr<Node> split(Node* headNode) {
        if (!headNode || !headNode->next) {
            return nullptr;
        }

        // 快慢指针找中点
        Node* slow = headNode;
        Node* fast = headNode->next.get();

        while (fast && fast->next) {
            slow = slow->next.get();
            fast = fast->next->next.get();
        }

        // slow 是中点，slow->next 是后半段的开头
        // 【关键点】std::move(slow->next) 做了两件事：
        // 1. 返回后半段的所有权 (unique_ptr)
        // 2. 将 slow->next 置为 nullptr，从而切断了前半段和后半段的连接
        std::unique_ptr<Node> secondHalf = std::move(slow->next);
        
        // 维护后半段的 prev 指针
        if (secondHalf) {
            secondHalf->prev = nullptr; // 后半段变成了独立链表，头节点的 prev 必须为空
        }
        
        return secondHalf;
    }

    // 2. 合并两个有序链表：l1 和 l2
    //    返回合并后的链表头 (unique_ptr)
    std::unique_ptr<Node> merge(std::unique_ptr<Node> l1, std::unique_ptr<Node> l2) {
        // 递归终止条件
        if (!l1) return l2;
        if (!l2) return l1;

        // 谁小谁做头，剩下的去递归合并
        if (l1->data < l2->data) {
            // l1 做头，递归合并 "l1 的剩余部分" 和 "l2"
            // std::move(l1->next) 把 l1 后面的节点拿出来参与合并
            // 合并完的结果重新挂载回 l1->next
            l1->next = merge(std::move(l1->next), std::move(l2));
            
            // 维护双向链接的 prev 指针
            if (l1->next) {
                l1->next->prev = l1.get();
            }
            return l1; // 返回 l1 作为新头
        } else {
            // l2 做头
            l2->next = merge(std::move(l1), std::move(l2->next));
            
            // 维护双向链接的 prev 指针
            if (l2->next) {
                l2->next->prev = l2.get();
            }
            return l2; // 返回 l2 作为新头
        }
    }

    // 3. 递归排序主函数
    std::unique_ptr<Node> mergeSort(std::unique_ptr<Node> node) {
        // Base case: 0 或 1 个节点不用排
        if (!node || !node->next) {
            return node;
        }

        // 切分：split 需要裸指针来遍历，但不能转移所有权，所以传 node.get()
        std::unique_ptr<Node> secondHalf = split(node.get());
        
        // 递归排序两半
        // 注意：node 在 split 后只剩前半段了
        node = mergeSort(std::move(node));
        secondHalf = mergeSort(std::move(secondHalf));

        // 合并两个有序链表
        return merge(std::move(node), std::move(secondHalf));
    }

    // 4. 非递归合并两个有序链表 (Iterative Merge)
    //    【关键技巧】使用二级指针 (std::unique_ptr<Node>* currLink) 来消除虚拟头节点 (Dummy Head)
    //    解答用户疑问：非递归是否必须虚拟头节点？
    //    答：不是必须的。如果不使用虚拟头节点，我们需要一个指针指向"当前需要被赋值的 next 指针"。
    //       - 刚开始，这个指针指向 head (即 &head)。
    //       - 之后，这个指针指向前一个节点的 next (即 &prev->next)。
    //       这样，无论是处理头节点还是后续节点，逻辑都是统一的：*currLink = std::move(node)。
    std::unique_ptr<Node> mergeIterative(std::unique_ptr<Node> l1, std::unique_ptr<Node> l2) {
        std::unique_ptr<Node> resultHead = nullptr;
        
        // currLink 是一个指向 "std::unique_ptr<Node>" 的指针
        // 它永远指向 "下一个要把节点接上去的位置"
        // 初始时，它指向 resultHead 本身
        std::unique_ptr<Node>* currLink = &resultHead;
        
        Node* prevNode = nullptr; // 用于维护双向链表的 prev 指针

        while (l1 && l2) {
            if (l1->data < l2->data) {
                // 1. 将 l1 移动到当前链接位置 (resultHead 或 上一个节点的 next)
                *currLink = std::move(l1);
                
                // 2. 此时 *currLink 拥有了原 l1 的头节点。
                //    原 l1 变量现在为空。
                //    我们需要把 "剩下的链表" (即 (*currLink)->next) 取出来，放回 l1 变量，以便下轮循环继续处理
                l1 = std::move((*currLink)->next);
            } else {
                *currLink = std::move(l2);
                l2 = std::move((*currLink)->next);
            }
            
            // 3. 维护 prev 指针
            (*currLink)->prev = prevNode;
            prevNode = (*currLink).get(); // 更新 prevNode 为当前节点

            // 4. currLink 移动到当前节点的 next 字段的地址
            //    准备接收下一个节点
            currLink = &((*currLink)->next);
        }

        // 处理剩余部分 (l1 或 l2 肯定有一个不为空，或者都为空)
        if (l1) {
            *currLink = std::move(l1);
        } else {
            *currLink = std::move(l2);
        }
        
        // 如果接上了剩余部分，记得把剩余部分的头节点的 prev 接好
        if (*currLink) {
            (*currLink)->prev = prevNode;
        }

        return resultHead;
    }

    // 5. 非递归排序主函数 (Iterative Merge Sort)
    std::unique_ptr<Node> mergeSortIterative(std::unique_ptr<Node> headNode) {
        if (!headNode || !headNode->next) {
            return headNode;
        }

        std::vector<std::unique_ptr<Node>> workList;

        // 1. 拆分：将链表拆成单个节点，全部放入 workList
        while (headNode) {
            std::unique_ptr<Node> nextNode = std::move(headNode->next);
            
            // 断开连接，使其成为独立节点
            headNode->prev = nullptr; 
            // headNode->next 已经是 nullptr (因为被 move 走了)
            
            workList.push_back(std::move(headNode));
            headNode = std::move(nextNode);
        }

        // 2. 合并：两两合并，直到只剩一个
        while (workList.size() > 1) {
            std::vector<std::unique_ptr<Node>> nextRoundList;
            
            for (size_t i = 0; i < workList.size(); i += 2) {
                if (i + 1 < workList.size()) {
                    // 使用非递归的 mergeIterative 替代递归的 merge
                    nextRoundList.push_back(mergeIterative(std::move(workList[i]), std::move(workList[i+1])));
                } else {
                    // 落单了 (Odd number of lists)
                    nextRoundList.push_back(std::move(workList[i]));
                }
            }
            // 更新 workList 为下一轮的列表
            workList = std::move(nextRoundList);
        }

        return std::move(workList[0]);
    }

public:
    // 构造函数
    DoubleLinkedList() : head(nullptr), tail(nullptr), size(0) {}

    // 析构函数 (默认即可，unique_ptr 会自动递归释放)
    // 但为了防止栈溢出，手动循环释放是个好习惯
    ~DoubleLinkedList() {
        while (head) {
            head = std::move(head->next);
        }
    }

    // --- 基础查询 ---

    [[nodiscard]] int getSize() const {
        return size;
    }

    [[nodiscard]] bool isEmpty() const {
        return size == 0;
    }

    [[nodiscard]] T get(int index) const {
        if (index < 0 || index >= size) {
            throw std::out_of_range("Index out of bounds");
        }
        Node* cur = head.get();
        for(int i = 0; i < index; i++) {
            cur = cur->next.get();
        }
        return cur->data;
    }

    // --- 核心添加操作 ---

    // 头插法：复杂度 O(1)
    void addFirst(T e) {
        // 1. 创建新节点
        auto newNode = std::make_unique<Node>(e);

        // 2. 如果链表本来就是空的
        if (isEmpty()) {
            tail = newNode.get();      // tail 指向新节点
            head = std::move(newNode); // head 拥有新节点
        } 
        // 3. 如果链表不为空
        else {
            newNode->next = std::move(head); // 新节点的 next 接管旧 head
            newNode->next->prev = newNode.get(); // 旧 head 的 prev 指向新节点
            head = std::move(newNode);       // head 更新为新节点
        }
        size++;
    }

    // 尾插法：复杂度 O(1)
    void addLast(T e) {
        // 1. 创建新节点
        auto newNode = std::make_unique<Node>(e);

        // 2. 如果链表为空
        if (isEmpty()) {
            tail = newNode.get();
            head = std::move(newNode);
        } 
        // 3. 如果不为空
        else {
            newNode->prev = tail;        // 新节点的 prev 指向旧 tail
            tail->next = std::move(newNode); // 旧 tail 的 next 接管新节点
            tail = tail->next.get();     // 更新 tail 指针
        }
        size++;
    }

    // 指定位置插入
    void add(int index, T e) {
        if (index < 0 || index > size) {
            throw std::out_of_range("Index out of bounds");
        }
        if (index == 0) {
            addFirst(e);
            return;
        }
        if (index == size) {
            addLast(e);
            return;
        }

        // 找到要插入位置的前一个节点
        Node* prevNode = head.get();
        for (int i = 0; i < index - 1; i++) {
            prevNode = prevNode->next.get();
        }
        
        // 创建新节点
        auto newNode = std::make_unique<Node>(e);

        // 链接 next
        newNode->next = std::move(prevNode->next); // newNode 接管后面的链表
        prevNode->next = std::move(newNode);       // prevNode 接管 newNode
        
        // 维护 prev 指针
        // 此时 prevNode->next 已经是 newNode 了
        prevNode->next->prev = prevNode;           // newNode->prev = prevNode
        if (prevNode->next->next) {
            prevNode->next->next->prev = prevNode->next.get(); // 后继节点的 prev = newNode
        }

        size++;
    }

    // --- 删除操作 ---

    // 删除头部：复杂度 O(1)
    T removeFirst() {
        if (isEmpty()) {
            throw std::out_of_range("Remove failed. List is empty.");
        }

        // 1. 获取要删除的数据
        T retData = head->data;

        // 2. 移动 head 到下一个节点
        // 这一步会隐式释放旧 head 的 unique_ptr
        head = std::move(head->next);

        // 3. 更新 size
        size--;

        // 4. 处理新 head 的 prev 指针
        if (head) {
            head->prev = nullptr; // 新头节点的 prev 必须为空
        } else {
            tail = nullptr;       // 如果链表空了，tail 也要置空
        }

        return retData;
    }

    // 删除尾部：复杂度 O(1)
    T removeLast() {
        if (isEmpty()) {
            throw std::out_of_range("Remove failed. List is empty.");
        }

        // 特殊情况：只有一个节点
        if (size == 1) {
            return removeFirst();
        }

        // 1. 获取数据
        T retData = tail->data;

        // 2. 找到倒数第二个节点 (即 tail->prev)
        Node* newTail = tail->prev;

        // 3. 断开尾节点
        // newTail->next 拥有原 tail，重置它会自动析构原 tail
        newTail->next.reset(); 

        // 4. 更新 tail 指针
        tail = newTail;
        size--;

        return retData;
    }

    // 指定位置删除
    T remove(int index) {
        if (index < 0 || index >= size) {
            throw std::out_of_range("Index out of bounds");
        }
        if (index == 0) return removeFirst();
        if (index == size - 1) return removeLast();

        Node* prevNode = head.get();
        for (int i = 0; i < index - 1; i++) {
            prevNode = prevNode->next.get();
        }

        // 要删除的节点是 prevNode->next
        std::unique_ptr<Node> delNode = std::move(prevNode->next);
        T retData = delNode->data;

        // 链接跳过 delNode
        prevNode->next = std::move(delNode->next);
        
        // 维护 prev 指针
        if (prevNode->next) {
            prevNode->next->prev = prevNode;
        }

        size--;
        return retData;
    }

    // --- 排序接口 ---
    void sort(bool iterative = false) {
        if (size <= 1) return;

        if (iterative) {
            // 使用非递归版本
            head = mergeSortIterative(std::move(head));
        } else {
            // 默认使用递归版本
            head = mergeSort(std::move(head));
        }

        // 排序后，tail 指针可能会乱，需要重新修复 tail
        // 这是一个小代价，为了保证 tail 永远指向最后
        Node* cur = head.get();
        while (cur && cur->next) {
            cur = cur->next.get();
        }
        tail = cur;
    }

    // 打印链表 (正向 + 反向检查)
    void print() const {
        std::cout << "Forward: [";
        Node* cur = head.get();
        while (cur) {
            std::cout << cur->data;
            if (cur->next) std::cout << " <-> ";
            cur = cur->next.get();
        }
        std::cout << "]" << std::endl;

        // 反向遍历检查 prev 指针是否正确
        std::cout << "Reverse: [";
        cur = tail;
        while (cur) {
            std::cout << cur->data;
            if (cur->prev) std::cout << " <-> ";
            cur = cur->prev;
        }
        std::cout << "]" << std::endl;
    }
};

#endif
