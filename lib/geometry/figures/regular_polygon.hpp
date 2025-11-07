#pragma once

#include <math.h>

#include "figure.hpp"
#include "vec2.hpp"

namespace lib::geometry {

template <std::floating_point T, std::size_t N>
class RegularPolygon : public Figure<T> {
  private:
    static constexpr T ROTATION_ANGLE = M_PI - ((N - 2) * M_PI) / N;

  private:
    // Я не храню точки как std::unique_ptr<Vec2<T>>, как об этом просят в
    // описании лабы
    // Потому что это не имеет смысла в данном случае
    // При использовании unique_ptr:
    //     Минусы:
    //         1. Понижение производительности за счет +1 перехода по указателю
    //         для получения любой точки
    //         2. Понижение производительности за счет в худшем случае 5
    //         сисколов на выделение памяти
    //         3. Повышение общего потребления памяти + 8 * 5 байт в Pentagon и
    //         те же sizeof(Vec2<T>) * 5 только теперь в хипе
    //         4. Понижение производительности за счет того, что точки теперь
    //         будут лежать в памяти не последовательно => реже попадаем в кеш
    //         процессора
    //         4. Понижение удобства доступа к точкам в методах класса
    //     Плюсы:
    //         1. ?
    // Если использование здесь умных указателей нужно только для того, чтобы
    // понять что я умею ими пользоваться, то проверить это можно будет
    // поговорив о них на защите лабы
    Vec2<T> points_[N];
    T side_;

  public:
    RegularPolygon() noexcept;

    // The radius vector indicates the position of the pentagon on the plane
    // The radius vector is one of the points of the pentagon
    // The guiding vector should be colinear with one of the sides of the
    // pentagon
    // The pentagon will be situated to the left of the guiding vector
    // The length of the side will be equal to the length of the guiding vector
    RegularPolygon(const Vec2<T>& radius_vector,
                   const Vec2<T>& guiding_vector) noexcept;

    explicit operator double() const noexcept final;
    Vec2<T> Centroid() const noexcept final;

    // Reading Pentagon from a stream uses the same structure as a constructor
    // with radius vector and a guiding vector
    // Essentially it just reads two vectors from the stream and when calls the
    // constructor
    template <std::floating_point T1, std::size_t N1>
    friend std::istream& operator>>(std::istream& is,
                                    RegularPolygon<T1, N1>& pol);

    template <std::floating_point T1, std::size_t N1>
    friend std::ostream& operator<<(std::ostream& os,
                                    const RegularPolygon<T1, N1>& pol);
};

} // namespace lib::geometry

#include "regular_polygon.ipp"
