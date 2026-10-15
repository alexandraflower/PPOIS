#pragma once
#include <iostream>
#include <map>

/**
 * @file Tape.h
 * @brief Описание структуры ленты машины Тьюринга и перечисления направлений.
 */

/**
 * @enum Direction
 * @brief Направления движения каретки (головки) машины Тьюринга.
 */
enum Direction{
    LEFT, /**< Сдвиг головки влево */
    RIGHT, /**< Сдвиг головки вправо */
    STAY /**< Каретка остается на месте */
};

/**
 * @class Tape
 * @brief Класс, представляющий бесконечную двустороннюю ленту машины Тьюринга.
 * 
 * Реализован на основе разреженного массива `std::map<int, char>`, 
 * что позволяет создать потенциально бесконечную ленту в обе стороны.
 */
class Tape{
private:
    std::map<int,char> cells_; /**< Карта записанных символов (позиция -> символ) */
    int cellNumber_=0; /**< Текущая позиция каретки */
    char blankSymbol_='_'; /**< Символ пустой ячейки по умолчанию */
public:
    /**
     * @brief Конструктор по умолчанию.
     */
    Tape()=default;

    /**
     * @brief Конструктор с указанием пользовательского пустого символа.
     * @param blankSymb Символ, обозначающий пустую ячейку.
     */
    explicit Tape(char blankSymb);

    /**
     * @brief Получить символ в указанной позиции ленты.
     * @param position Индекс ячейки (может быть отрицательным).
     * @return Символ в ячейке или пустой символ, если ячейка не инициализирована.
     */
    char getSymbolAt(int position) const;

    /**
     * @brief Прочитать символ в текущей позиции каретки.
     * @return Текущий символ под кареткой.
     */
    char readCurrentSymbol() const;

    /**
     * @brief Записать новый символ в текущую ячейку.
     * @param newSymbol Записываемый символ.
     */
    void writeNewSymbol(char newSymbol);

    /**
     * @brief Сдвинуть каретку в заданном направлении.
     * @param direction Направление перемещения (@ref Direction).
     */
    void moveToDirection(Direction direction);

    /**
     * @brief Вывести всю занятую область ленты и текущее положение каретки.
     */
    void getFullTape() const;

    /**
     * @brief Перегрузка оператора вывода в поток (выводит окно вокруг каретки).
     * @param os Выходной поток.
     * @param tape Объект ленты.
     * @return Ссылка на выходной поток.
     */
    friend std::ostream& operator<<(std::ostream& os, const Tape& tape);

    /**
     * @brief Перегрузка оператора ввода (считывает строку символов на ленту, начиная с позиции 0).
     * @param is Входной поток.
     * @param tape Объект ленты.
     * @return Ссылка на входной поток.
     */
    friend std::istream& operator>>(std::istream& is, Tape& tape);

    /**
     * @brief Оператор сравнения двух лент на равенство.
     */
    bool operator==(const Tape& other) const;

    /**
     * @brief Оператор проверки неравенства двух лент.
     */
    bool operator!=(const Tape& other) const;
};