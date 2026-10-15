#pragma once
#include <iostream>

/**
 * @file Vector3D.h
 * @brief Определение класса трехмерного геометрического вектора.
 */

/**
 * @class Vector3D
 * @brief Класс, представляющий направленный отрезок (вектор) в трехмерном пространстве.
 * 
 * Вектор задается начальной точкой (x1, y1, z1) и конечной точкой (x2, y2, z2).
 */

class Vector3D{
    private:
    double x1,y1,z1;
    double x2,y2,z2;
    public:

    /**
     * @brief Конструктор по умолчанию.
     * Инициализирует все координаты нулями.
     */
    Vector3D(); 

    /**
     * @brief Конструктор с параметрами.
     * @param startX Координата X начала вектора.
     * @param startY Координата Y начала вектора.
     * @param startZ Координата Z начала вектора.
     * @param endX Координата X конца вектора.
     * @param endY Координата Y конца вектора.
     * @param endZ Координата Z конца вектора.
     */
    Vector3D(double startX, double startY, double startZ, double endX, double endY, double endZ); //объявление вектора с заданными параметрами
    
    /**
     * @brief Получение координаты начала вектора по оси OX.
     * @return Координата X начала вектора.
     */
    double getX1() const;
    /**
     * @brief Получение координаты начала вектора по оси OY.
     * @return Координата Y начала вектора.
     */
    double getY1() const;
    /**
     * @brief Получение координаты начала вектора по оси OZ.
     * @return Координата Z начала вектора.
     */
    double getZ1() const;
    /**
     * @brief Получение координаты конца вектора по оси OX.
     * @return Координата X конца вектора.
     */
    double getX2() const;
    /**
     * @brief Получение координаты конца вектора по оси OY.
     * @return Координата Y конца вектора.
     */
    double getY2() const;
    /**
     * @brief Получение координаты конца вектора по оси OZ.
     * @return Координата Z конца вектора.
     */
    double getZ2() const;

    /**
     * @brief Вычисляет проекцию вектора на ось OX.
     * @return Разность (x2-x1).
     */
    double dx() const;
    /**
     * @brief Вычисляет проекцию вектора на ось OY.
     * @return Разность (y2-y1).
     */
    double dy()const;
    /**
     * @brief Вычисляет проекцию вектора на ось OZ.
     * @return Разность (z2-z1).
     */
    double dz() const;

    /**
     * @brief Вычисляет длину вектора.
     * @return Длина вектора типа double.
     */
    double length() const;

    /**
     * @brief Вычисляет косинус угла между текущим и переданным вектором.
     * @param other Вектор, с которым вычисляется угол.
     * @return Косинус угла между векторами в диапазоне [-1, 1].
     * @throw std::invalid_argument Если один из векторов имеет нулевую длину.
     */
    double operator^(const Vector3D& other) const;

    /**
     * @brief Сложение текущего вектора с другим с присваиванием.
     * @param rhs Прибавляемый вектор.
     * @return Ссылка на текущий объект (*this).
     */
    Vector3D& operator+=(const Vector3D& rhs);

    /**
     * @brief Бинарный оператор сложения двух векторов.
     * @param rhs Второе слагаемое.
     * @return Новый вектор-результат сложения.
     */
    Vector3D operator+(const Vector3D& rhs) const;

    /**
     * @brief Вычитание вектора из текущего с присваиванием.
     * @param rhs Вычитаемый вектор.
     * @return Ссылка на текущий объект (*this).
     */
    Vector3D& operator-=(const Vector3D& rhs);

    /**
     * @brief Бинарный оператор вычитания векторов.
     * @param rhs Вычитаемый вектор.
     * @return Новый вектор-результат вычитания.
     */
    Vector3D operator-(const Vector3D& rhs) const;
    
    /**
     * @brief Скалярное произведение векторов.
     * @param other Второй сомножитель.
     * @return Значение скалярного произведения.
     */
    double operator*(const Vector3D& other) const;

    /**
     * @brief Умножение вектора на число с присваиванием.
     * @param scalar Числовой множитель.
     * @return Ссылка на текущий объект (*this).
     */
    Vector3D& operator*=(double scalar);

    /**
     * @brief Умножение вектора на число справа.
     * @param scalar Числовой множитель.
     * @return Новый вектор, увеличенный в scalar раз.
     */
    Vector3D operator*(double scalar) const;

    /**
     * @brief Покомпонентное деление вектора на другой вектор с присваиванием.
     * @param rhs Вектор-делитель.
     * @return Ссылка на текущий объект (*this).
     * @throw std::invalid_argument Если хотя бы одна из проекций вектора rhs равна нулю.
     */
    Vector3D& operator/=(const Vector3D& rhs);

    /**
     * @brief Бинарное покомпонентное деление векторов.
     * @param rhs Вектор-делитель.
     * @return Новый вектор с разделенными проекциями.
     * @throw std::invalid_argument Если хотя бы одна из проекций вектора rhs равна нулю.
     */
    Vector3D operator/(const Vector3D& rhs) const;

    /**
     * @brief Проверка равенства двух векторов по длине.
     * @param rhs Вектор для сравнения.
     * @return true, если длины равны, иначе false.
     */
    bool operator==(const Vector3D& rhs) const;

    /**
     * @brief Проверка неравенства двух векторов по длине.
     * @param rhs Вектор для сравнения.
     * @return true, если длины не равны, иначе false.
     */
    bool operator!=(const Vector3D& rhs) const;

    /**
     * @brief Проверка отношения "строго меньше" по длине.
     * @param rhs Вектор для сравнения.
     * @return true, если длина текущего вектора меньше длины rhs.
     */
    bool operator<(const Vector3D& rhs) const;

    /**
     * @brief Проверка отношения "меньше или равно" по длине.
     * @param rhs Вектор для сравнения.
     * @return true, если длина текущего вектора меньше либо равна длине rhs.
     */
    bool operator<=(const Vector3D& rhs) const;

    /**
     * @brief Проверка отношения "строго больше" по длине.
     * @param rhs Вектор для сравнения.
     * @return true, если длина текущего вектора больше длины rhs.
     */
    bool operator>(const Vector3D& rhs) const;

    /**
     * @brief Проверка отношения "больше или равно" по длине.
     * @param rhs Вектор для сравнения.
     * @return true, если длина текущего вектора больше либо равна длине rhs.
     */
    bool operator>=(const Vector3D& rhs) const;

    /**
     * @brief Оператор вывода вектора в поток.
     * 
     * Формат вывода: "(x1, y1, z1) (x2, y2, z2)".
     * @param os Выходной поток.
     * @param vector Вектор для вывода.
     * @return Ссылка на выходной поток os.
     */
    friend std::ostream& operator<<(std::ostream& os, const Vector3D& vector);

    /**
     * @brief Оператор ввода вектора из потока.
     * 
     * Считывает последовательно 6 координат: x1 y1 z1 x2 y2 z2.
     * @param is Входной поток.
     * @param vector Вектор, в который записываются считанные данные.
     * @return Ссылка на входной поток is.
     */
    friend std::istream& operator>>(std::istream& is, Vector3D& vector);
};

/**
 * @brief Умножение числа на вектор слева.
 * @param scalar Числовой множитель.
 * @param vector Исходный вектор.
 * @return Новый масштабированный вектор.
 */
Vector3D operator*(double scalar, const Vector3D& vector);