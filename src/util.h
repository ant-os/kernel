#pragma once
#include <stdint.h>
#include <stdlib.h>
#include "memory/heap.h"
#include "antstatus.h"
#include "types.h"
/*
template <typename T>
__STD_CLASS dynamic_allocator{
    public:

        dynamic_allocator(size_t size);
        ~dynamic_allocator();

        T& operator[](size_t index);
        ANT_STATUS operator+=(T object);

        __inline__ size_t size(){
            return m_size;
        };
        __inline__ size_t compacity(){
            return m_size;
        }
        __inline__ ANT_STATUS relocate(){
            T* newBuffer = (T*)realloc(m_buffer, (m_compacity + 1) * sizeof(T));
            if (newBuffer == NULL) return STATUS_OUT_OF_MEMROY;
            m_buffer = newBuffer;
            
        }


    private:
        T* m_buffer;
        size_t m_compacity;
        size_t m_size;

};

template<typename T>
__inline__ dynamic_allocator<T>::dynamic_allocator(size_t _size)
{
    m_size = _size;
    m_compacity = m_size + 1;
    m_buffer = (T*)malloc((m_size + 1) * sizeof(T));
}

template<typename T>
__inline__ dynamic_allocator<T>::~dynamic_allocator()
{
    free(m_buffer);
}

template<typename T>
__inline__ T& dynamic_allocator<T>::operator[](size_t index) 
{
    return m_buffer[index];
}

/// @brief Add Element to Buffer.
/// @tparam T 
/// @param object 
/// @return Status
template<typename T>
__inline__ ANT_STATUS dynamic_allocator<T>::operator+=(T object)
{
    if (m_size == m_compacity) {
        m_compacity *= 1.5;
        T* newBuffer = (T*)realloc(m_buffer, m_compacity * sizeof(T));
        if (newBuffer == NULL) return STATUS_OUT_OF_MEMROY;
        
        m_buffer = newBuffer;
    }

    m_buffer[m_size] = object;

    ++m_size;
};

__STD_TYPE dynamic_allocator<char> dynamic_string;

template <typename T, typename Alloc=dynamic_allocator<T>>
__STD_CLASS list : __public_allocator{
    public:
        list()
            : Alloc(10){}
};

extern "C++"{
template <typename T>
__inline__ T* clone_ptr(T* __ptr, size_t __size){
    T* newPtr = (T*)(__malloc (__size));
    #if T == dynamic_allocator
    #warning "Cannnot copy dynamic memory!"
    #endif
    memcpy(__ptr, newPtr, __size);
    return newPtr;
};
} __UNSAFE;

*/

namespace std{
struct range
{
    uint64_t Start;
    uint64_t End;
    
    inline range(uint64_t start, uint64_t end){
        Start = start;
        End = end;
    }
    inline bool contains(uint64_t value){
        return (value <= End && value >= Start);
    }

    inline uint64_t delta(){
        return (End - Start);
    }
};
}