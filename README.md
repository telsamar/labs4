[![Build Status](https://travis-ci.com/Toliak/lab0304.svg?branch=master)](https://travis-ci.com/Toliak/lab0304)
[![coverage](https://gitlab.toliak.ru/bmstu-cpp-3-sem/lab0304/badges/master/pipeline.svg)](https://gitlab.toliak.ru/bmstu-cpp-3-sem/lab0304/pipelines)
![Code size](https://img.shields.io/github/languages/code-size/Toliak/lab0304.svg)
![GitHub Repo size](https://img.shields.io/github/repo-size/Toliak/lab0304.svg)

# Лабораторная работа №4

## Задание 1
Реализовать некопируемый перемещаемый шаблон класса **stack** с методами `pop`, `push`, `head`. 
```cpp
template <typename T>
class stack
{
public:
  void push(T&& value);
  void push(const T& value);
  void pop();
  const T& head() const;
};
```
⚠️ *Стандартными контейнерами пользоваться **запрещено**.*

## Задание 2
Реализовать некопируемый перемещаемый шаблон класса **stack** для некопируемых перемещаемых типов с методами:
* `push_emplace`, принимающего те же аргументы, что и конструктор для `T`, где `T` - пареметр шаблона;
* `head`, возвращающего ссылку на верхушку стека;
* `pop`, выполняющего снятие элемента с верхушки стека.
```cpp
template <typename T>
class stack
{
public:
  template <typename ... Args>
  void push_emplace(Args&&... value);
  void push(T&& value);
  const T& head() const;
  T pop();
};
```
⚠️ *Стандартными контейнерами пользоваться **запрещено**.*

## Задание 3
Реализовать **unit-test**'ы с использованием фреймворка **Google Test** для реализованных шаблонов в заданиях 1 и 2.

## Рекомендации
💡 Воспользуйтесть [**type traits**](https://en.cppreference.com/w/cpp/types#Type_traits_.28since_C.2B.2B11.29), а именно `is_move_constructible`, `is_move_assignable` и пр.