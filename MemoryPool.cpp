#include "MemoryPool.h"
#include <iostream>
#define k 1024
#define default_block_num 64
#define default_block_size 4 * k
MemoryPool::MemoryPool(size_t bn, size_t bs){
    if(bn < default_block_num) block_num = default_block_num;
    else block_num = bn;
    if(bs < default_block_size )block_size = default_block_size;
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

MemoryPool::MemoryPool():MemoryPool(default_block_num, default_block_size) {
}

void* MemoryPool::allocate(size_t sz) {
    // 大于block_size，直接从os分配
    if( sz > block_size){
        //std::cout<<"allocat memory of size "<<sz<<" directly from os "<<std::endl;
        return ::operator new(sz);
    }
    else{
        //当前内存池还可以放入
        //std::cout<<"allocat memory of size "<<sz<<" in id "<<this->id<<std::endl;
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

MemoryPool* MemoryPool::deallocate(MemoryPool* cursor,MemoryPool* end,void* ptr,size_t sz) {
    // 指针在内存池中
    if (cursor != nullptr) {
        void *Pool_Header = cursor->Pool_Header;
        // 指针在内存池中
        if (Pool_Header < ptr && ptr < (void *) ((char *) Pool_Header + cursor->pool_size)) {
            block_ptr curr = reinterpret_cast<block_ptr>(static_cast<char *>(ptr) - sizeof(block));
            if (curr != cursor->alloc_Header) {
                curr->prev->next = curr->next;
                if (curr->next != nullptr)curr->next->prev = curr->prev;
            } else {
                cursor->alloc_Header = curr->next;
                if (curr->next != nullptr)curr->next->prev = nullptr;
            }
            // 加到空块链表中
            curr->next = cursor->free_Header;
            if (cursor->free_Header != nullptr) {
                cursor->free_Header->prev = curr;
            }
            cursor->free_Header = curr;
            //已经移除指针,返回当前内存池的首地址
            return cursor;
        }else if( cursor != end && cursor->Next!= nullptr) {
            //还有下一块,查找下一块
            return deallocate(cursor->Next, end, ptr, sz);
        }else{
            //没有空的内存池了
            return nullptr;
        }
    }
    return nullptr;
}
MemoryPool::~MemoryPool() {
    if (Pool_Header != nullptr)
        ::operator delete(Pool_Header);
}
