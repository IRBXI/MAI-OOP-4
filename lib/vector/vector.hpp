#pragma once

#include <initializer_list>
#include <type_traits>

namespace lib::vector {

const std::size_t INITIAL_CAPACITY = 10;

template <typename T>
class Vector {
    //     Я не использую shared_ptr<T[]> как тип data_, как об этом просят в
    // условии лабы, потому что в случае вектора (как и в принципе любого
    // другого контейнера) это не имеет практически никакого смысла и несет
    // только минусы
    // Использование умных указателей имеет смысл только там, где нам сложно
    // отследить, кто в какой момент владеет памятью и кто должен быть
    // ответственнен за ее освобождение, например несколько обьектов имеют
    // доступ к одному общему обьекту и мы не знаем точно, когда все обьекты,
    // которым нужно иметь доступ к обьекту под умным указателем, будут
    // уничтожены. Вектор же является одной из самых простых структур данных, в
    // которой мы всегда знаем, где и когда мы аллоцируем и освобождаем память.
    // Тут нет необходимости в умных указателях, их использование лишь
    // бессмысленно усложнит код.
    // Также если мы пишем стандартный контейнер, по типу std::vector, мы хотим,
    // чтобы он работал насколько быстро, насколько мы можем его оптимизировать,
    // использование shared_ptr ведет же к тому, что размер класса вектора
    // увеличивается на 8 байт (так как sizeof(shared_ptr) == 16, так как он
    // хранит указатель на сам обьект и на control block). Также в случае, если
    // мы не используем make_shared при каждой реаллокации увеличивается
    // количество сисколов, так как shared_ptr также должен аллоцировать память
    // под новый control block
    // Использование unique_ptr имело бы хоть какой-то смысл, но тогда мы не
    // смогли бы ничего возвращать в методе Data() (в случае shared_ptr
    // возвращали бы weak_ptr)
    // Если использование умных указателей здесь нужно лишь для того, чтобы
    // показать, что я умею ими пользоваться, то я могу продемострировать это на
    // защите
  private:
    std::size_t capacity_ = 0;
    std::size_t size_ = 0;
    T* data_ = nullptr;

  public:
    Vector() noexcept;
    Vector(std::size_t count, const T& value)
        requires std::is_copy_constructible_v<T>;
    Vector(const std::initializer_list<T>& init);
    Vector(const Vector& other)
        requires std::is_copy_constructible_v<T>;
    Vector(Vector&& other) noexcept;
    ~Vector();

    Vector& operator=(const Vector& other);
    Vector& operator=(Vector&& other) noexcept;

    T& Front() noexcept;
    const T& Front() const noexcept;
    T& Back() noexcept;
    const T& Back() const noexcept;

    T& operator[](std::size_t pos) noexcept;
    const T& operator[](std::size_t pos) const noexcept;
    T& At(std::size_t pos);
    const T& At(std::size_t pos) const;

    inline __attribute__((always_inline)) std::size_t Size() const noexcept;
    inline __attribute__((always_inline)) std::size_t Capacity() const noexcept;
    inline __attribute__((always_inline)) T const* Data() const noexcept;
    inline __attribute__((always_inline)) T* Data() noexcept;

    inline __attribute__((always_inline)) bool IsEmpty() const noexcept;

    template <typename U>
    void Resize(std::size_t count, U&& value);
    void Reserve(std::size_t new_cap);
    void Clear() noexcept;

    template <typename U>
    void Insert(std::size_t pos, U&& value);
    void Erase(std::size_t begin_pos, std::size_t end_pos);
    template <typename U>
    void PushBack(U&& value);
    template <typename... Args>
    void EmplaceBack(Args&&... args);
    void PopBack();

    template <typename U>
    friend void swap(Vector<U>& a, Vector<U>& b);
};

} // namespace lib::vector

#include "vector.ipp"
