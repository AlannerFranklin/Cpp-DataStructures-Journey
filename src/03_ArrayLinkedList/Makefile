# 编译器设置
CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -g

# 目标文件
TARGETS = RingBuffer_main SkipList_main Bitmap_main

# 默认目标
all: $(TARGETS)

# 编译规则
RingBuffer_main: RingBuffer_main.cpp RingBuffer.hpp
	$(CXX) $(CXXFLAGS) -o RingBuffer_main RingBuffer_main.cpp

SkipList_main: SkipList_main.cpp SkipList.hpp
	$(CXX) $(CXXFLAGS) -o SkipList_main SkipList_main.cpp

Bitmap_main: Bitmap_main.cpp Bitmap.hpp
	$(CXX) $(CXXFLAGS) -o Bitmap_main Bitmap_main.cpp

# 清理规则
clean:
	rm -f $(TARGETS) *.o *.exe

.PHONY: all clean
