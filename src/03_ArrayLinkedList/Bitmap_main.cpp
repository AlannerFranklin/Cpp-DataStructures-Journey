#include <iostream>
#include <cassert>
#include "Bitmap.hpp"

void testBitmap() {
    std::cout << "[Test] Bitmap Operations" << std::endl;
    // 创建一个能容纳 0~99 的位图
    Bitmap bm(100);

    // 1. 测试 Set
    std::cout << "Set: 5, 63, 64, 99" << std::endl;
    bm.set(5);
    bm.set(63); // 边界测试：第一个 uint64_t 的最后一位
    bm.set(64); // 边界测试：第二个 uint64_t 的第一位
    bm.set(99);

    // 2. 测试 Test (Expect True)
    assert(bm.test(5) == true);
    assert(bm.test(63) == true);
    assert(bm.test(64) == true);
    assert(bm.test(99) == true);
    std::cout << "Test Positive: PASS" << std::endl;

    // 3. 测试 Test (Expect False)
    assert(bm.test(0) == false);
    assert(bm.test(6) == false);
    assert(bm.test(100) == false); // Out of bounds
    std::cout << "Test Negative: PASS" << std::endl;

    // 4. 测试 Reset
    std::cout << "Reset: 63" << std::endl;
    bm.reset(63);
    assert(bm.test(63) == false);
    assert(bm.test(5) == true); // 其他位不受影响
    std::cout << "Reset: PASS" << std::endl;

    // 5. 测试 Clear
    std::cout << "Clear All" << std::endl;
    bm.clear();
    assert(bm.test(5) == false);
    assert(bm.test(64) == false);
    std::cout << "Clear: PASS" << std::endl;
}

int main() {
    testBitmap();
    return 0;
}
