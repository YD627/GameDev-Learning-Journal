// 经典内存池的实现
#include <cstddef>
#include <iostream>
using namespace std;

template <int ObjectSize, int NumofObject = 20>
class MemPool {
private:
    // 空闲节点结构体
    struct FreeNode {
        FreeNode *pNext;
        char data[ObjectSize];
    };
    // 内存块结构体
    struct MemBlock {
        MemBlock *pNext;
        FreeNode data[NumofObject];
    };

    FreeNode *freeNodeHeader;
    MemBlock *memBlockHeader;

public:
    MemPool() {
        freeNodeHeader = NULL;
        memBlockHeader = NULL;
    }
    ~MemPool() {
        MemBlock *ptr;
        while (memBlockHeader) {
            ptr = memBlockHeader->pNext;
            delete memBlockHeader;
            memBlockHeader = ptr;
        }
    }
    void *malloc();
    void free(void *ptr);
};

// 分配空闲节点
template <int ObjectSize, int NumofObject>
void *MemPool<ObjectSize, NumofObject>::malloc() {
    // 如果没有空节点，申请新的内存
    if(freeNodeHeader == NULL) {
        MemBlock *newBlock = new MemBlock();
        newBlock->pNext = NULL;

        freeNodeHeader = &newBlock->data[0];    // 设置内存块的第一个节点为空闲节点链表的首节点
        // 将内存块的其他节点连起来
        for(int i = 1; i < NumofObject; i++) {
            newBlock->data[i-1].pNext = &newBlock->data[i];
        }
        newBlock->data[NumofObject - 1].pNext = NULL;

        // 首次申请内存块
        if(memBlockHeader == NULL) {
            memBlockHeader = newBlock;
        }
        else {
            // 将新内存块加入到内存块链表
            newBlock->pNext = memBlockHeader;
            memBlockHeader = newBlock;
        }
    }
    // 返回空节点链表的第一个节点
        void *freeNode = freeNodeHeader;
        freeNodeHeader = freeNodeHeader->pNext;
        return freeNode;
}

// 释放已经获得的节点
template <int ObjectSize, int NumofObject>
void MemPool<ObjectSize, NumofObject>::free(void *ptr) {
    FreeNode *freeNode = (FreeNode *)ptr;
    freeNode->pNext = freeNodeHeader;
    freeNodeHeader = freeNode;
}

class ActualClass {
    static int count;
    int No;
public:
    ActualClass() {
        No = count++;
    }
    void print() {
        cout << this << ": "<< "the No is: " << No << endl;
    }

    void *operator new(size_t size);
    void operator delete(void *p);
};

// 定义内存池对象
MemPool<sizeof(ActualClass), 2> memPool;

void *ActualClass::operator new(size_t size) {
    return  memPool.malloc();
}

void ActualClass::operator delete(void *p) {
    memPool.free(p);
}

int ActualClass::count = 0;

int main() {
    ActualClass *p1 = new ActualClass();
    p1->print();
    ActualClass *p2 = new ActualClass();
    p2->print();
    delete p1;

    p1 = new ActualClass();
    p1->print();

    ActualClass *p3 = new ActualClass();
    p3->print();

    delete p3;
    delete p2;
    delete p1;
    return 0;
}