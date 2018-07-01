//
// Created by kk on 6/15/18.
//

#ifndef FINAL_STL_ALLOCATOR_H
#define FINAL_STL_ALLOCATOR_H
#include <iostream>
#include <limits>
#include "MemoryPool.h"

template <typename T>
class Allocator {
public :
    //    typedefs
    using value_type = T;
    using pointer = value_type* ;
    using const_pointer =  const value_type* ;
    using reference = value_type&;
    using const_reference = const value_type&;
    using size_type = std::size_t;
    using difference_type = std::ptrdiff_t;

public :
    //    convert an allocator<T> to allocator<U>
    template<typename U>
    struct rebind { typedef Allocator<U> other; };

public :
    inline Allocator() = default;
    inline ~Allocator() = default;
    inline Allocator(Allocator const&) = default;
    template<typename U>
    inline explicit Allocator(Allocator<U> const&) {}

    //    地址信息
    inline pointer address(reference r) { return &r; }
    inline const_pointer address(const_reference r) { return &r; }

    //   内存分配
    inline pointer allocate(size_type cnt, typename std::allocator<void>::const_pointer = 0) {
        return reinterpret_cast<pointer>(Mempool.allocate(cnt * sizeof(T)));
    }
    inline void deallocate(pointer p, size_type n) {
        Mempool.deallocate(p, n);
    }

    //  返回大小
    inline size_type max_size() const {
        return std::numeric_limits<size_type>::max() / sizeof(T);
    }

    // 构造函数/析构函数
    inline void construct(pointer p, const T& t) { new(p) T(t); }
    inline void destroy(pointer p) { p->~T(); }

    inline bool operator==(Allocator const&) { return true; }
    inline bool operator!=(Allocator const& a) { return !operator==(a); }
private:
    static MemoryPool_List Mempool;
};   

template <typename T> MemoryPool_List Allocator<T>::Mempool;

#endif //FINAL_STL_ALLOCATOR_H
