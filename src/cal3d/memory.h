#pragma once

#include <map>
#include <set>
#include "cal3d/platform.h"

namespace cal3d {
    void* allocate_aligned_data(size_t size);

    // Can't use std::vector w/ __declspec(align(16)) :(
    // http://ompf.org/forum/viewtopic.php?f=11&t=686
    // http://social.msdn.microsoft.com/Forums/en-US/vclanguage/thread/0adabdb5-f732-4db7-a8de-e3e83af0e147/
    template<typename T>
    struct SSEArray : boost::noncopyable {
        typedef T value_type;
        typedef size_t size_type;
        typedef ptrdiff_t difference_type;

        typedef T* iterator;
        typedef T const* const_iterator;

        SSEArray()
            : data(0)
            , _size(0)
        {}

        SSEArray(size_t initial_size)
            : data(reinterpret_cast<T*>(allocate_aligned_data(sizeof(T) * initial_size)))
            , _size(initial_size)
        {
            std::fill(data, data + _size, T());
        }

        ~SSEArray() {
            CAL3D_ALIGNED_FREE(data);
        }

        // does not preserve array contents
        void destructive_resize(size_t new_size) {
            if (_size != new_size) {
                T* new_data = reinterpret_cast<T*>(allocate_aligned_data(sizeof(T) * new_size));

                if (data) {
                    CAL3D_ALIGNED_FREE(data);
                }
                _size = new_size;
                data = new_data;
            }
        }

        T& operator[](size_t idx) {
            return data[idx];
        }

        const T& operator[](size_t idx) const {
            return data[idx];
        }

        size_t size() const {
            return _size;
        }

        void push_back(const T& v) {
        }

        iterator begin() { return data; }
        const_iterator begin() const { return data; }
        iterator end() { return data + _size; }
        const_iterator end() const { return data + _size; }

        T* data;

    private:
        size_t _size;
    };

    template<unsigned Alignment>
    struct AlignedMemory {
        void* operator new(size_t size) {
            return CAL3D_ALIGNED_MALLOC(size, Alignment);
        }
        void operator delete(void* p) {
            CAL3D_ALIGNED_FREE(p);
        }
    };

    template<typename T, typename A>
    T* pointerFromVector(std::vector<T, A>& v) {
        if (v.empty()) {
            return 0;
        } else {
            return &v[0];
        }
    }

    template<typename T, typename A>
    const T* pointerFromVector(const std::vector<T, A>& v) {
        if (v.empty()) {
            return 0;
        } else {
            return &v[0];
        }
    }

    template<typename T>
    T* pointerFromVector(SSEArray<T>& v) {
        return v.data;
    }

    template<typename T>
    const T* pointerFromVector(const SSEArray<T>& v) {
        return v.data;
    }
}

#define CAL3D_DEFINE_SIZE(T) \
    inline size_t sizeInBytes(T const&) { \
        return sizeof(T); \
    }

CAL3D_DEFINE_SIZE(unsigned);

template<typename T>
size_t sizeInBytes(const cal3d::SSEArray<T>& v) {
    return sizeof(T) * v.size();
}

template<typename T>
size_t sizeInBytes(const std::vector<T>& v) {
    size_t r = sizeof(T) * (v.capacity() - v.size());
    for (typename std::vector<T>::const_iterator i = v.begin(); i != v.end(); ++i) {
        r += sizeInBytes(*i);
    }
    return r;
}

template<typename T>
size_t sizeInBytes(const std::list<T>& v) {
    size_t r = 8 * v.size(); // pointers each way
    for (typename std::list<T>::const_iterator i = v.begin(); i != v.end(); ++i) {
        r += sizeInBytes(*i);
    }
    return r;
}

template<typename T>
size_t sizeInBytes(const std::set<T>& s) {
    size_t r = 20 * s.size();
    for (typename std::set<T>::const_iterator i = s.begin(); i != s.end(); ++i) {
        r += sizeInBytes(*i);
    }
    return r;
}

template<typename K, typename V>
size_t sizeInBytes(const std::map<K, V>& m) {
    size_t r = 20 * m.size();
    for (typename std::map<K, V>::const_iterator i = m.begin(); i != m.end(); ++i) {
        r += sizeInBytes(i->first) + sizeInBytes(i->second);
    }
    return r;
}