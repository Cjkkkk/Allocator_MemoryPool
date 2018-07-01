#include "MemoryPool.h"

#include <iostream>

MemoryPool::MemoryPool(size_t bn, size_t bs){
    if(bn < 64) block_num = 64;
    else block_num = bn;
    if(bs < 4096 )block_size = 4096;
    else block_size = bs;
    pool_size = block_num * (block_size + sizeof(block));
    Pool_Header = operator new(pool_size);
    Next = nullptr;
    // succeess create the pool
    if (Pool_Header != nullptr) {
        for (auto i = 0; i < block_num; ++i) {
            block_ptr curr = reinterpret_cast<block_ptr>(static_cast<char*>(Pool_Header) + i * (block_size + sizeof(block)));
            curr->prev = nullptr;
            curr->next = free_Header;

            if (free_Header != nullptr) {
                free_Header->prev = curr;
            }

            free_Header = curr;
        }
    }
}

MemoryPool::MemoryPool():MemoryPool(64, 4096) {
}

void* MemoryPool::allocate(size_t sz) {
    // 大于block_size，直接从os分配
    if( sz > block_size){
        std::cout<<"allocat memory of size "<<sz<<" directly from os "<<std::endl;
        return ::operator new(sz);
    }
//    else if (free_Header == nullptr ||Pool_Header == nullptr){
//        std::cout<<"allocat memory of size "<<sz<<" in id "<<this->id<<std::endl;
//        if(Next == nullptr){
//            //没有下一块
//            //分配下一块
//            Next = new MemoryPool();
//            Next->id = this->id + 1;
//            return Next->allocate(sz);
//        } else{
//            //有下一块
//            return Next->allocate(sz);
//        }
//    }
    else{
        //当前内存池还可以放入
        std::cout<<"allocat memory of size "<<sz<<" in id "<<this->id<<std::endl;
        block_ptr curr = free_Header;

        free_Header = curr->next;
        if (free_Header != nullptr) {
            free_Header->prev = nullptr;
        }

        curr->next = alloc_Header;
        if (alloc_Header != nullptr) {
            alloc_Header->prev = curr;
        }
        alloc_Header = curr;

        return static_cast<void *>(reinterpret_cast<char*>(curr) + sizeof(block));
    }
}

bool MemoryPool::deallocate(void *ptr, size_t sz) {
    // 指针在内存池中
//    if (Pool_Header < ptr && ptr < (void *)((char *)Pool_Header + pool_size)) {
//        block_ptr curr = reinterpret_cast<block_ptr>(static_cast<char *>(ptr) - sizeof(block));
//        //从占用块链表中删除
//        alloc_Header = curr->next;
//        if (alloc_Header != nullptr) {
//            alloc_Header->prev = nullptr;
//        }
//
//        // 加到空块链表中
//        curr->next = free_Header;
//        if (free_Header != nullptr) {
//            free_Header->prev = curr;
//        }
//        free_Header = curr;
//
//        //将这个pool添加到free_pool中去
//    } else if(Next != nullptr){
//          //还有下一块,查找下一块
//        Next->deallocate(ptr,sz);
//    }else operator delete(ptr);
    if (Pool_Header < ptr && ptr < (void *)((char *)Pool_Header + pool_size)) {
        block_ptr curr = reinterpret_cast<block_ptr>(static_cast<char *>(ptr) - sizeof(block));
        //从占用块链表中删除
        alloc_Header = curr->next;
        if (alloc_Header != nullptr) {
            alloc_Header->prev = nullptr;
        }

        // 加到空块链表中
        curr->next = free_Header;
        if (free_Header != nullptr) {
            free_Header->prev = curr;
        }
        free_Header = curr;
        return true;
    }
   return false;

}

MemoryPool::~MemoryPool() {
    if (Pool_Header != nullptr)
        ::operator delete(Pool_Header);
}
