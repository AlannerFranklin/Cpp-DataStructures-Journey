# 01. 动态数组 (Dynamic Array / Vector)

本模块实现了一个简易版的 `std::vector`，名为 `MyVector`。
它展示了如何使用 **C++17** 的现代特性来管理动态内存，并实现了自动扩容与缩容机制。

## 核心特性

### 1. 自动扩缩容 (Resize)
- **扩容**：当数组满 (`size == capacity`) 时，容量翻倍 (`2 * capacity`)。
- **懒缩容**：当元素数量降至容量的 1/4 (`size == capacity / 4`) 时，容量减半 (`capacity / 2`)。
  - *设计意图*：避免在临界点反复添加/删除元素导致频繁申请/释放内存（复杂度震荡）。

### 2. 现代 C++17 特性应用

#### `std::unique_ptr` (智能指针)
不再使用原始指针 (`new`/`delete`) 管理数组内存，而是使用 `std::unique_ptr<T[]>`。
- **优势**：
  - **自动内存管理**：无需手动写析构函数释放内存，避免内存泄漏。
  - **异常安全**：即使构造过程中抛出异常，内存也能被正确释放。
  - **明确所有权**：表明该数组由 `MyVector` 独占。

#### `[[nodiscard]]` (属性)
用于修饰 `getSize()`, `isEmpty()`, `get()` 等函数。
- **作用**：如果调用者忽略了这些函数的返回值（例如只写了 `arr.isEmpty();` 而没做判断），编译器会发出警告。
- **意义**：减少因忽略返回值而导致的逻辑错误。

#### Move Semantics (移动语义)
- 显式删除了拷贝构造函数 (`MyVector(const MyVector&) = delete`)，因为 `unique_ptr` 不支持拷贝。
- 保留了默认的移动构造函数 (`MyVector(MyVector&&) = default`)，支持高效的资源转移。

## API 概览

| 方法 | 描述 | 时间复杂度 |
| --- | --- | --- |
| `addLast(e)` | 在末尾添加元素 | O(1) [均摊] |
| `add(index, e)` | 在指定位置插入 | O(n) |
| `remove(index)` | 删除指定位置元素 | O(n) |
| `get(index)` | 获取元素 | O(1) |
| `set(index, e)` | 修改元素 | O(1) |
| `find(e)` | 查找元素索引 | O(n) |
| `resize()` | 调整容量 | O(n) |

## 如何运行

请参考根目录下的 [README](../../README.md) 中的 "Makefile 使用指南"。

```bash
make
./main
```
