#pragma once

#include "vector.hpp"

#include <algorithm>
#include <cassert>
#include <cstring>
#include <initializer_list>
#include <memory>
#include <stdexcept>
#include <type_traits>

namespace lib::vector {

template <typename T>
Vector<T>::Vector() noexcept {}

template <typename T>
Vector<T>::Vector(std::size_t count, const T& value)
    requires std::is_copy_constructible_v<T>
{
    Reserve(count);
    size_ = count;
    for (std::size_t i = 0; i < count; ++i) {
        std::construct_at(data_ + i, value);
    }
}

template <typename T>
Vector<T>::Vector(const std::initializer_list<T>& init) {
    Reserve(init.size());
    for (const T& x : init) {
        PushBack(x);
    }
}

template <typename T>
Vector<T>::Vector(const Vector& other)
    requires std::is_copy_constructible_v<T>
{
    Reserve(other.capacity_);
    size_ = other.size_;
    for (std::size_t i = 0; i < other.size_; ++i) {
        std::construct_at(data_ + i, other[i]);
    }
}

template <typename T>
Vector<T>::Vector(Vector&& other) noexcept
    : capacity_(other.capacity_), size_(other.size_), data_(other.data_) {
    other.capacity_ = 0;
    other.size_ = 0;
    other.data_ = nullptr;
}

template <typename T>
Vector<T>& Vector<T>::operator=(const Vector& other) {
    if (this != &other) {
        Vector<T> temp(other);
        swap(*this, temp);
    }
    return *this;
}

template <typename T>
Vector<T>& Vector<T>::operator=(Vector&& other) noexcept {
    if (this != &other) {
        Vector<T> temp(std::move(other));
        swap(*this, temp);
    }
    return *this;
}

template <typename T>
T& Vector<T>::Front() noexcept {
    assert(size_ > 0);
    return data_[0];
}

template <typename T>
const T& Vector<T>::Front() const noexcept {
    assert(size_ > 0);
    return data_[0];
}

template <typename T>
T& Vector<T>::Back() noexcept {
    assert(size_ > 0);
    return data_[size_ - 1];
}

template <typename T>
const T& Vector<T>::Back() const noexcept {
    assert(size_ > 0);
    return data_[size_ - 1];
}

template <typename T>
T& Vector<T>::operator[](std::size_t pos) noexcept {
    assert(pos < size_);
    return data_[pos];
}

template <typename T>
const T& Vector<T>::operator[](std::size_t pos) const noexcept {
    assert(pos < size_);
    return data_[pos];
}

template <typename T>
T& Vector<T>::At(std::size_t pos) {
    if (pos >= size_) {
        throw std::out_of_range("The index is out of range of the Vector");
    }
    return data_[pos];
}

template <typename T>
const T& Vector<T>::At(std::size_t pos) const {
    if (pos >= size_) {
        throw std::out_of_range("The index is out of range of the Vector");
    }
    return data_[pos];
}

template <typename T>
inline __attribute__((always_inline)) std::size_t
Vector<T>::Size() const noexcept {
    return size_;
}

template <typename T>
inline __attribute__((always_inline)) std::size_t
Vector<T>::Capacity() const noexcept {
    return capacity_;
}

template <typename T>
inline __attribute__((always_inline)) T* Vector<T>::Data() noexcept {
    return data_;
}

template <typename T>
inline __attribute__((always_inline)) const T*
Vector<T>::Data() const noexcept {
    return data_;
}

template <typename T>
inline __attribute__((always_inline)) bool Vector<T>::IsEmpty() const noexcept {
    return !size_;
}

template <typename T>
template <typename U>
void Vector<T>::Resize(size_t count, U&& value) {
    std::size_t old_size = size_;
    if (count < size_) {
        for (std::size_t i = 0; i < old_size - count; ++i) {
            PopBack();
        }
        return;
    }
    if (count > capacity_) {
        Reserve(count);
    }
    for (std::size_t i = 0; i < count - old_size; ++i) {
        PushBack(std::forward<U>(value));
    }
}

template <typename T>
void Vector<T>::Reserve(std::size_t new_cap) {
    std::allocator<T> alloc;

    if (new_cap > capacity_ or capacity_ == 0) {
        std::size_t alloc_cap = std::max(new_cap, INITIAL_CAPACITY);
        T* new_data = alloc.allocate(alloc_cap);
        if (data_) {
            if constexpr (std::is_trivially_copyable_v<T>) {
                std::memcpy(new_data, data_, sizeof(T) * size_);
            } else {
                std::uninitialized_move(data_, data_ + size_, new_data);
            }

            if constexpr (!std::is_trivially_destructible_v<T>) {
                std::destroy(data_, data_ + size_);
            }
            alloc.deallocate(data_, capacity_);
        }
        data_ = new_data;
        capacity_ = alloc_cap;
    }
}

template <typename T>
void Vector<T>::Clear() noexcept {
    if constexpr (!std::is_trivially_destructible_v<T>) {
        for (std::size_t i = 0; i < size_; ++i) {
            std::destroy_at(data_ + i);
        }
    }
    size_ = 0;
}

template <typename T>
template <typename U>
void Vector<T>::Insert(std::size_t pos, U&& value) {
    if (size_ == capacity_) {
        Reserve(capacity_ * 2);
    }
    if constexpr (std::is_trivially_copyable_v<T> and
                  std::is_trivially_destructible_v<T>) {
        std::memmove(data_ + pos + 1, data_ + pos, (size_ - pos) * sizeof(T));
    } else {
        for (std::size_t i = size_; i > pos; --i) {
            std::uninitialized_move(data_ + i - 1, data_ + i, data_ + i);
            std::destroy_at(data_ + i - 1);
        }
    }
    std::construct_at(data_ + pos, std::forward<U>(value));
    size_++;
}

// Erase in the range [begin_pos, end_pos)
template <typename T>
void Vector<T>::Erase(std::size_t begin_pos, std::size_t end_pos) {
    if (begin_pos >= size_) {
        return;
    }
    if constexpr (!std::is_trivially_destructible_v<T>) {
        for (std::size_t i = begin_pos; i < end_pos; ++i) {
            std::destroy_at(data_ + i);
        }
    }
    if (end_pos == size_) {
        size_ = size_ - (end_pos - begin_pos);
        return;
    }
    if constexpr (std::is_trivially_copyable_v<T>) {
        std::memcpy(data_ + begin_pos, data_ + end_pos,
                    (size_ - end_pos) * sizeof(T));
    } else {
        std::uninitialized_move(data_ + end_pos, data_ + size_,
                                data_ + begin_pos);
    }
    size_ = size_ - (end_pos - begin_pos);
}

template <typename T>
template <typename U>
void Vector<T>::PushBack(U&& value) {
    EmplaceBack(std::forward<U>(value));
}

template <typename T>
template <typename... Args>
void Vector<T>::EmplaceBack(Args&&... args) {
    if (size_ == capacity_) {
        Reserve(capacity_ * 2);
    }
    std::construct_at(data_ + size_, std::forward<Args>(args)...);
    size_++;
}

template <typename T>
void Vector<T>::PopBack() {
    assert(size_ > 0);
    if constexpr (!std::is_trivially_destructible_v<T>) {
        std::destroy_at(data_ + (size_ - 1));
    }
    size_--;
}

template <typename T>
Vector<T>::~Vector() {
    Clear();
    if (data_) {
        std::allocator<T>().deallocate(data_, capacity_);
    }
}

template <typename T>
void swap(Vector<T>& a, Vector<T>& b) {
    std::swap(a.capacity_, b.capacity_);
    std::swap(a.size_, b.size_);
    std::swap(a.data_, b.data_);
}

} // namespace lib::vector
