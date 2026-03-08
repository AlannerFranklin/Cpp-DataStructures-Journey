#ifndef SKIP_LIST_HPP
#define SKIP_LIST_HPP

#include <iostream>
#include <vector>
#include <random>
#include <ctime>
#include <climits>

// 跳表 (Skip List)
//template <typename K, typename V>
class SkipList {
    // 待实现
private:
    // 跳表节点
    struct Node {
        int key;
        int value;
        
        // forward[i] 表示该节点在第 i 层的下一个节点
        // forward[0] 就是最底层的 next 指针
        std::vector<Node*> forward;

        Node(int k, int v, int level) 
            : key(k), value(v), forward(level, nullptr) {}
    };

    Node* head;          // 虚拟头节点 (Dummy Head)
    int maxLevel;        // 当前跳表的最大层数限制
    int currentLevel;    // 当前实际使用的最高层数
    
    // 随机数生成器相关
    std::mt19937 gen;
    std::uniform_real_distribution<> dis; // 生成 0.0 ~ 1.0

public:
    // 构造函数
    SkipList(int max_lvl = 16) 
        : maxLevel(max_lvl), currentLevel(0), dis(0.0, 1.0) {
        
        // 初始化随机种子
        gen.seed(std::random_device{}());

        // 创建虚拟头节点，它的高度必须是 maxLevel，以便能连接所有层
        head = new Node(INT_MIN, 0, maxLevel);
    }
    // 析构函数 (简单的内存清理)
    ~SkipList() {
        Node* curr = head;
        while (curr) {
            Node* next = curr->forward[0];
            delete curr;
            curr = next;
        }
    }

    // 1. 抛硬币决定层数
    int randomLevel() {
        int lvl = 1;
        // 50% 概率增加一层 (P=0.5)，你可以改成 0.25
        while (dis(gen) < 0.5 && lvl < maxLevel) {
            lvl++;
        }
        return lvl;
    }

    // 2. 插入节点
    void insert(int key, int value) {
        // update[i] 记录的是：在第 i 层，新节点应该插在谁的后面
        std::vector<Node*> update(maxLevel, nullptr);
        Node* curr = head;

        // A. 从最高层往下找插入位置
        for (int i = currentLevel - 1; i >= 0; i--) {
            // 如果下一个节点的 key 小于我们要插的 key，就继续往前走
            while (curr->forward[i] && curr->forward[i]->key < key) {
                curr = curr->forward[i];
            }
            // 记录每一层的"转折点"
            update[i] = curr;
        }

        // curr 现在的 forward[0] 就是第 0 层应该插入的位置
        // 这里可以处理 key 已存在的情况 (更新 value)，我们简单起见允许重复或忽略
        
        // B. 决定新节点的层数
        int newLevel = randomLevel();

        // 如果新层数比当前跳表最高层还高，需要更新 update 数组
        if (newLevel > currentLevel) {
            for (int i = currentLevel; i < newLevel; i++) {
                update[i] = head; // 高出的层，前驱自然是 head
            }
            currentLevel = newLevel; // 更新跳表高度
        }

        // C. 创建新节点并链接
        Node* newNode = new Node(key, value, newLevel);
        for (int i = 0; i < newLevel; i++) {
            // 经典的链表插入：newNode->next = prev->next; prev->next = newNode;
            newNode->forward[i] = update[i]->forward[i];
            update[i]->forward[i] = newNode;
        }
    }

    // 3. 查找节点
    bool search(int key) {
        Node* curr = head;

        // 从最高层开始往下找
        for (int i = currentLevel - 1; i >= 0; i--) {
            while (curr->forward[i] && curr->forward[i]->key < key) {
                curr = curr->forward[i];
            }
        }

        // 现在 curr 在第 0 层，且 curr->key < key
        // 检查 curr 的下一个节点是否等于 key
        curr = curr->forward[0];
        if (curr && curr->key == key) {
            return true;
        }
        return false;
    }
    // 4. 打印跳表结构 (可视化调试)
    void print() {
        std::cout << "\n=== SkipList Structure (MaxLevel: " << currentLevel << ") ===" << std::endl;
        for (int i = currentLevel - 1; i >= 0; i--) {
            Node* curr = head;
            std::cout << "Level " << i << ": ";
            
            // 打印该层的所有节点
            while (curr->forward[i]) {
                std::cout << curr->forward[i]->key << " -> ";
                curr = curr->forward[i];
            }
            std::cout << "NULL" << std::endl;
        }
    }
};

#endif
