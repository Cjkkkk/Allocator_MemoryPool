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
    bool deallocate(void* ptr,size_t size);


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
    int time = 0;
    MemoryPool* free_pool = nullptr;
    MemoryPool* occupied_pool = nullptr;
    MemoryPool_List(){
        MemoryPool* p = new MemoryPool();
        free_pool = p;
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
            MemoryPool* curr = free_pool;
            free_pool = free_pool->Next;
            free_pool-> Prev = nullptr;

            //添加占用内存池链表
            curr->Next = occupied_pool;
            if(occupied_pool != nullptr){
                occupied_pool->Prev = curr;
            }
            occupied_pool = curr;
        }
        return free_pool->allocate(size);
    };
    void deallocate(void* ptr,size_t size){
        MemoryPool* c = occupied_pool;//从已经满的块中查找
        while(c != nullptr){
            void* Pool_Header = c->Pool_Header;
            // 指针在内存池中
            if (Pool_Header < ptr && ptr < (void *)((char *)Pool_Header + c->pool_size)) {
                block_ptr curr = reinterpret_cast<block_ptr>(static_cast<char *>(ptr) - sizeof(block));
                //从占用块链表中删除
                c->alloc_Header = curr->next;
                if (c->alloc_Header != nullptr) {
                    c->alloc_Header->prev = nullptr;
                }

                // 加到空块链表中
                curr->next = c->free_Header;
                if (c->free_Header != nullptr) {
                    c->free_Header->prev = curr;
                }
                c->free_Header = curr;

                std::cout<<"find and delete in id "<<c->id <<std::endl;
                if(c->alloc_Header == nullptr){
                    
                }
                return;
            } else if(c->Next != nullptr){
                //还有下一块,查找下一块
                c = c->Next;
            } else{
                break;
            }
        }
        if(free_pool->deallocate(ptr,size))return;
        //不在内存池中
        std::cout<<"directly delete from os of size "<< size <<std::endl;
        time ++;
        operator delete(ptr);
        return;
    };
};

#endif //FINAL_MEMORYPOOL_H
