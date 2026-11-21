#pragma once

#include <cstddef>
#include <memory>
#include <stdexcept>
#include <utility>

namespace restaurant {

template <typename T>
class DynamicArray {
public:
    typedef T* iterator;
    typedef const T* const_iterator;

    DynamicArray() noexcept
        : data_(nullptr), size_(0), capacity_(0) {}

    DynamicArray(const DynamicArray& other)
        : data_(nullptr), size_(0), capacity_(0) {
        if (other.size_ == 0) {
            return;
        }

        data_ = allocator_.allocate(other.size_);
        capacity_ = other.size_;
        try {
            for (; size_ < other.size_; ++size_) {
                allocator_.construct(data_ + size_, other.data_[size_]);
            }
        } catch (...) {
            destroyElements();
            allocator_.deallocate(data_, capacity_);
            data_ = nullptr;
            capacity_ = 0;
            throw;
        }
    }

    DynamicArray(DynamicArray&& other) noexcept
        : data_(other.data_), size_(other.size_), capacity_(other.capacity_) {
        other.data_ = nullptr;
        other.size_ = 0;
        other.capacity_ = 0;
    }

    ~DynamicArray() {
        destroyElements();
        if (data_ != nullptr) {
            allocator_.deallocate(data_, capacity_);
        }
    }

    DynamicArray& operator=(const DynamicArray& other) {
        if (this != &other) {
            DynamicArray copy(other);
            swap(copy);
        }
        return *this;
    }

    DynamicArray& operator=(DynamicArray&& other) noexcept {
        if (this != &other) {
            clear();
            if (data_ != nullptr) {
                allocator_.deallocate(data_, capacity_);
            }

            data_ = other.data_;
            size_ = other.size_;
            capacity_ = other.capacity_;

            other.data_ = nullptr;
            other.size_ = 0;
            other.capacity_ = 0;
        }
        return *this;
    }

    void swap(DynamicArray& other) noexcept {
        using std::swap;
        swap(data_, other.data_);
        swap(size_, other.size_);
        swap(capacity_, other.capacity_);
    }

    std::size_t size() const noexcept {
        return size_;
    }

    std::size_t capacity() const noexcept {
        return capacity_;
    }

    bool empty() const noexcept {
        return size_ == 0;
    }

    void reserve(std::size_t newCapacity) {
        if (newCapacity <= capacity_) {
            return;
        }

        T* newData = allocator_.allocate(newCapacity);
        std::size_t constructed = 0;
        try {
            for (; constructed < size_; ++constructed) {
                allocator_.construct(
                    newData + constructed,
                    std::move_if_noexcept(data_[constructed]));
            }
        } catch (...) {
            while (constructed > 0) {
                --constructed;
                allocator_.destroy(newData + constructed);
            }
            allocator_.deallocate(newData, newCapacity);
            throw;
        }

        destroyElements();
        if (data_ != nullptr) {
            allocator_.deallocate(data_, capacity_);
        }
        data_ = newData;
        capacity_ = newCapacity;
        size_ = constructed;
    }

    void pushBack(const T& value) {
        if (size_ == capacity_) {
            T valueCopy(value);
            grow();
            allocator_.construct(data_ + size_, std::move_if_noexcept(valueCopy));
        } else {
            allocator_.construct(data_ + size_, value);
        }
        ++size_;
    }

    void pushBack(T&& value) {
        if (size_ == capacity_) {
            T valueCopy(std::move(value));
            grow();
            allocator_.construct(data_ + size_, std::move(valueCopy));
        } else {
            allocator_.construct(data_ + size_, std::move(value));
        }
        ++size_;
    }

    T& at(std::size_t index) {
        if (index >= size_) {
            throw std::out_of_range("DynamicArray index out of range");
        }
        return data_[index];
    }

    const T& at(std::size_t index) const {
        if (index >= size_) {
            throw std::out_of_range("DynamicArray index out of range");
        }
        return data_[index];
    }

    T& operator[](std::size_t index) noexcept {
        return data_[index];
    }

    const T& operator[](std::size_t index) const noexcept {
        return data_[index];
    }

    T& front() {
        if (empty()) {
            throw std::out_of_range("DynamicArray is empty");
        }
        return data_[0];
    }

    const T& front() const {
        if (empty()) {
            throw std::out_of_range("DynamicArray is empty");
        }
        return data_[0];
    }

    T& back() {
        if (empty()) {
            throw std::out_of_range("DynamicArray is empty");
        }
        return data_[size_ - 1];
    }

    const T& back() const {
        if (empty()) {
            throw std::out_of_range("DynamicArray is empty");
        }
        return data_[size_ - 1];
    }

    template <typename Predicate>
    std::size_t findIndex(Predicate predicate) const {
        for (std::size_t i = 0; i < size_; ++i) {
            if (predicate(data_[i])) {
                return i;
            }
        }
        return npos;
    }

    bool removeAt(std::size_t index) {
        if (index >= size_) {
            return false;
        }

        for (std::size_t i = index; i + 1 < size_; ++i) {
            data_[i] = std::move(data_[i + 1]);
        }
        allocator_.destroy(data_ + size_ - 1);
        --size_;
        return true;
    }

    void clear() noexcept {
        destroyElements();
        size_ = 0;
    }

    iterator begin() noexcept {
        return data_;
    }

    const_iterator begin() const noexcept {
        return data_;
    }

    const_iterator cbegin() const noexcept {
        return data_;
    }

    iterator end() noexcept {
        return data_ == nullptr ? nullptr : data_ + size_;
    }

    const_iterator end() const noexcept {
        return data_ == nullptr ? nullptr : data_ + size_;
    }

    const_iterator cend() const noexcept {
        return data_ == nullptr ? nullptr : data_ + size_;
    }

    static const std::size_t npos = static_cast<std::size_t>(-1);

private:
    void grow() {
        const std::size_t newCapacity = capacity_ == 0 ? 1 : capacity_ * 2;
        if (newCapacity < capacity_) {
            throw std::length_error("DynamicArray capacity overflow");
        }
        reserve(newCapacity);
    }

    void destroyElements() noexcept {
        while (size_ > 0) {
            --size_;
            allocator_.destroy(data_ + size_);
        }
    }

    std::allocator<T> allocator_;
    T* data_;
    std::size_t size_;
    std::size_t capacity_;
};

template <typename T>
const std::size_t DynamicArray<T>::npos;

template <typename T>
void swap(DynamicArray<T>& left, DynamicArray<T>& right) noexcept {
    left.swap(right);
}

}  // namespace restaurant
