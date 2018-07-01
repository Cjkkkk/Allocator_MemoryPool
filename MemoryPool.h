//
// Created by kk on 6/18/18.
//

#ifndef FINAL_MEMORYPOOL_H
#define FINAL_MEMORYPOOL_H

#include <iostream>
using block_ptr = struct block*;
struct block{
    struct block *prev = nullptr;
    struct block *next = nullptr;
};

class MemoryPool {
public:
    MemoryPool();
    MemoryPool(size_t num,size_t size);
    ~MemoryPool();
    void* allocate(size_t size);
    MemoryPool* deallocate(MemoryPool* cursor,MemoryPool* end,void* ptr,size_t size);


    block_ptr free_Header = nullptr;
    block_ptr alloc_Header = nullptr;
    void* Pool_Header = nullptr;
    size_t block_size;//每个块的大小
    size_t block_num;//块的数目
    size_t pool_size;//(block的大小 + 维护block结构体大小) * block 数目
    int id = 0;
    MemoryPool* Next;
    MemoryPool* Prev;
};

class MemoryPool_List{
public:
    MemoryPool* free_pool = nullptr;
    MemoryPool* first_pool = nullptr;
    MemoryPool_List(){
        MemoryPool* p = new MemoryPool();
        free_pool = p;
        first_pool = p;
    };
    void* allocate(size_t size){
        if(free_pool->free_Header == nullptr){
            //当前内存池满了
            if(free_pool->Next == nullptr){
                //下一个内存池还没分配好
                free_pool->Next = new MemoryPool();
                free_pool->Next->id = free_pool->id + 1;
            }
            //下一个内存池已经分配好了
            //移除空闲内存池链表头
            free_pool = free_pool->Next;
        }
        return free_pool->allocate(size);
    };
    void deallocate(void* ptr,size_t size) {
        MemoryPool *cursor = first_pool->deallocate(first_pool, free_pool, ptr, size);//从第一个pool中开始查找
        if( cursor == nullptr){
            //不在内存池中
            //std::cout<<"directly delete from os of size "<< size <<std::endl;
            operator delete(ptr);
            return;
        }
        else if(cursor->alloc_Header == nullptr) {
            //std::cout << "find and delete size : " << size << " in id " << cursor->id << std::endl;
            if (cursor != free_pool) {
                if (cursor != first_pool) {
                    cursor->Prev->Next = cursor->Next;
                    cursor->Next->Prev = cursor->Prev;
                } else {
                    first_pool = first_pool->Next;
                    first_pool->Prev = nullptr;
                }
                cursor->Next = free_pool->Next;
                if (free_pool->Next != nullptr) free_pool->Next->Prev = cursor;
                cursor->Prev = free_pool;
                free_pool->Next = cursor;

            }
        }
        else{
            //std::cout << "find and delete size : " << size << " in id " << cursor->id << std::endl;
        }
    }
};

#endif //FINAL_MEMORYPOOL_H
