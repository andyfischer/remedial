// A dynamically resized array.
#pragma once

#include <cstddef>  // for size_t

#include <cassert>
#include <cstdlib>
#include <string>
#include <exception>

// "Remedial Template Library"
namespace rtl {

// Other than the typedef's at the top, I believe that this is pretty much
// the STL's vector class. It seemed like a good starting point.
template <typename T>
class vector {
public:
    typedef size_t size_type;
    typedef T* iterator;
    typedef const T* const_iterator;

    explicit vector()
    {
        _count = 0;
        _capacity = 0;
        _data = NULL;
    }

    explicit vector(size_type n, const T& x = T())
    {
        _count = 0;
        _capacity = 0;
        _data = NULL;
        resize(n, x);
    }

    vector(const vector<T>& v)
    {
        _count = 0;
        _capacity = 0;
        _data = NULL;

       reserve(v.size());
       for (size_t i=0; i < v.size(); i++)
           new (&_data[i]) T(v[i]);

       _count = v.size();
    }

    template <typename I> vector(I first, I last)
    {
        _count = 0;
        _capacity = 0;
        _data = NULL;

        // Do one iterator pass to figure out size.
        size_t size = 0;
        for (I it = first; it != last; ++it)
            size++;

        reserve(size);

        // Now copy each element.
        size_t my_it = 0;
        for (I their_it = first; their_it != last; ++their_it, ++my_it)
            new (&_data[my_it]) T(*their_it);

        _count = size;
    }

    ~vector()
    {
        clear();
        free(_data);
    }

    iterator begin()
    {
        return _data;
    }

    const_iterator begin() const
    {
        return _data;
    }

    iterator end()
    {
        return _data + _count;
    }

    const_iterator end() const
    {
        return _data + _count;
    }

    iterator rbegin()
    {
        return _data + _count - 1;
    }
    const_iterator rbegin() const
    {
        return _data + _count - 1;
    }

    iterator rend()
    {
        return _data - 1;
    }
    const_iterator rend() const
    {
        return _data - 1;
    }

    size_type size() const
    {
        return _count;
    }

    size_type max_size() const
    {
        return (size_t) -1;
    }
    void resize(size_type size, T const& copy = T())
    {
        reserve(size);

        if (_count < size) {
            // Initialize new elements
            for (size_t i=_count; i < size; i++)
                new (&_data[i]) T(copy);

        } else if (_count > size) {
            // Delete extra elements
            for (size_t i=size; i < _count; i++)
                _data[i].~T();
        }

        _count = size;
    }

    size_type capacity() const
    {
        return _capacity;
    }
    bool empty() const
    {
        return _count == 0;
    }
    void reserve(size_type n)
    {
        if (n > _capacity) {

            size_t newCapacity;

            // Figure out the next size.
            if (n <= 8) {
                // Minimum size is 8.
                newCapacity = 8;
            } else {
                // Otherwise, round up to the next power of two.
                newCapacity = _capacity;
                while (newCapacity < n)
                    newCapacity *= 2;
            }

            size_t mallocSize = newCapacity * sizeof(T);
            T* newData = (T*) malloc(mallocSize);

            // Copy-construct new elements.
            for (size_t i=0; i < _count; i++)
                new (&newData[i]) T(at(i));

            free(_data);
            _data = newData;
            _capacity = newCapacity;
        }
    }

    T& operator[](size_type n)
    {
        assert(n < _count);
        return _data[n];
    }
    const T& operator[](size_type n) const
    {
        assert(n < _count);
        return _data[n];
    }
    T& at(size_type n)
    {
        assert(n < _count);
        return _data[n];
    }
    const T& at(size_type n) const
    {
        assert(n < _count);
        return _data[n];
    }
    T& front()
    {
        assert(!empty());
        return _data[0];
    }
    const T& front() const
    {
        assert(!empty());
        return _data[0];
    }
    T& back()
    {
        assert(!empty());
        return _data[_count - 1];
    }
    const T& back() const
    {
        assert(!empty());
        return _data[_count - 1];
    }

    template <typename I> void assign(I first, I last);
    void assign(size_type n, const T& x);

    void push_back(const T& x)
    {
        resize(_count + 1, x);
    }
    void pop_back()
    {
        resize(_count - 1);
    }
    iterator insert(iterator p, const T& x);
    void insert(iterator p, size_type n, const T& x);
    template <typename I> void insert(I p, I first, I last);
    iterator erase(iterator p);
    iterator erase(iterator first, iterator last);

    void swap(vector<T>& v)
    {
        T* temp_data = _data;
        size_t temp_count = _count;
        _data = v._data;
        _count = v._count;
        v._data = temp_data;
        v._count = temp_count;
    }
    void clear()
    {
        for (size_t i=0; i < _count; i++)
            _data[i].~T();
        _count = 0;
    }

private:
    T* _data;
    size_t _count;
    size_t _capacity;
};

}  // namespace rtl
