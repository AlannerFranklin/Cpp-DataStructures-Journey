CXX = g++
CXXFLAGS = -std=c++17 -Wall -g

# 目标文件名
TARGET_SINGLE = LinkedList_main
TARGET_DOUBLE = DoubleLinkedList_main

# 源文件
SRCS_SINGLE = LinkedList_main.cpp
SRCS_DOUBLE = DoubleLinkedList_main.cpp

# 头文件 (依赖)
HEADERS_SINGLE = LinkedList.hpp
HEADERS_DOUBLE = DoubleLinkedList.hpp

# 默认目标: 编译所有
all: $(TARGET_SINGLE) $(TARGET_DOUBLE)

# 单链表编译规则
$(TARGET_SINGLE): $(SRCS_SINGLE) $(HEADERS_SINGLE)
	$(CXX) $(CXXFLAGS) -o $(TARGET_SINGLE) $(SRCS_SINGLE)

# 双链表编译规则
$(TARGET_DOUBLE): $(SRCS_DOUBLE) $(HEADERS_DOUBLE)
	$(CXX) $(CXXFLAGS) -o $(TARGET_DOUBLE) $(SRCS_DOUBLE)

# 清理规则
clean:
	rm -f $(TARGET_SINGLE) $(TARGET_DOUBLE) main
