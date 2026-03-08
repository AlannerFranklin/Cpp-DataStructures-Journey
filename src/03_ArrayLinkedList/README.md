# 03. 数组链表与高级数据结构 (Array & Linked List Extensions)

本模块包含以下高级数据结构的实现：

## 1. 环形数组 (RingBuffer.hpp)
- **概念**：使用固定大小的数组模拟首尾相连的环形队列。
- **应用**：网络数据包缓冲、音频处理、生产者-消费者模型。
- **核心**：利用模运算 (`%`) 实现索引回绕。

## 2. 跳表 (SkipList.hpp)
- **概念**：基于链表的概率性数据结构，通过多层索引加速查找。
- **特点**：查找/插入/删除平均复杂度 O(log n)，是平衡树的替代方案 (如 Redis 的 ZSet)。

## 3. 位图 (Bitmap.hpp)
- **概念**：用二进制位 (bit) 来标记元素是否存在。
- **优势**：极度节省空间 (10亿个整数只需约 120MB)。
- **应用**：布隆过滤器基础、大量数据去重/排序。

## 运行方法

本模块包含独立的 `Makefile`。

```bash
cd src/03_ArrayLinkedList

# 编译所有
make

# 运行环形缓冲区测试
./RingBuffer_main

# 运行跳表测试
./SkipList_main

# 运行位图测试
./Bitmap_main

# 清理
make clean
```
