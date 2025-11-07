# MAI-OOP-4

Вариант - 30

[Документ со всеми вариантами](https://github.com/DVDemon/mai_oop_examples_public/blob/main/homeworks/2025_%D0%9B%D0%A0_%D0%9E%D0%9E%D0%9F_4.pdf)

## Структура проекта

- `lib/` - исходный код библиотек
- `tests/` - unit-тесты с использованием Google Test
- `CMakeLists.txt` - конфигурация CMake

## Сборка и запуск

```bash
# Создание директории для сборки
mkdir build
cd build

# Конфигурация CMake
cmake ..

# Сборка проекта
make

# Запуск тестов вектора 
./tests/vector_unit

# Запуск тестов для правильных многоугольников  
./tests/figures/regular_polygons

# Запуск всех тестов сразу
./tests/unit
