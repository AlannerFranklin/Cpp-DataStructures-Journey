#ifndef BITMAP_HPP
#define BITMAP_HPP

#include <iostream>
#include <vector>
#include <cstdint> // uint64_t
#include <stdexcept>

// 位图 (Bitmap)
class Bitmap {
    // 待实现
private:
    std::vector<uint64_t> bits;
    size_t size; // 能够容纳的最大数字范围 (不是字节数)
public:
    // 构造函数：range 是我们要处理的最大数字 (例如 1000)
    explicit Bitmap(size_t range) : size(range) {
        // 计算需要多少个 uint64_t
        // 公式：(range + 63) / 64 相当于 ceil(range / 64.0)
        // 示例：range=64 -> 1个; range=65 -> 2个
        size_t numWords = (range + 63) / 64;
        bits.resize(numWords, 0); // 初始化全为 0
    }
    // 1. 标记数字 n 存在 (Set bit to 1)
    void set(size_t n) {
        if (n >= size) return; // 或者抛异常
        
        size_t index = n / 64;   // 位于第几个 uint64_t
        size_t offset = n % 64;  // 位于该 uint64_t 的第几位
        
        // 核心逻辑：
        // 1ULL << offset: 将 1 左移 offset 位，例如 0001000...
        // | (或运算): 将对应位置 1，不影响其他位
        bits[index] |= (1ULL << offset); 
    }
    // 2. 检查数字 n 是否存在 (Test bit)
    bool test(size_t n) const {
        if (n >= size) return false;
        
        size_t index = n / 64;
        size_t offset = n % 64;
        
        // 核心逻辑：
        // & 运算，如果那一位是 0，结果就是 0；如果是 1，结果非 0
        return (bits[index] & (1ULL << offset)) != 0;
    }
    // 3. 清除数字 n (Set bit to 0)
    void reset(size_t n) {
        if (n >= size) return;
        
        size_t index = n / 64;
        size_t offset = n % 64;
        
        // 核心逻辑：
        // 1. (1ULL << offset) 构造掩码，例如 ...0001000...
        // 2. ~ 取反，变成 ...1110111...
        // 3. & 与运算，只把那一位变成 0，其他位保持不变
        bits[index] &= ~(1ULL << offset);
    }
    
    // 调试用：清除所有
    void clear() {
        std::fill(bits.begin(), bits.end(), 0);
    }
};

#endif
