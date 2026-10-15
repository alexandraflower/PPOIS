#pragma once
#include "Tape.hpp"
#include <iostream>
#include <string>
#include <map>
#include <utility>

/**
 * @file Program.h
 * @brief Структура элементарного действия и класс таблицы правил программы.
 */

/**
 * @struct Action
 * @brief Действие, выполняемое машиной Тьюринга при срабатывании правила.
 */
struct Action{
    std::string newState;
    char newSymbol;
    Direction direction; 
};

/**
 * @class Program
 * @brief Хранилище правил перехода (машины Тьюринга).
 * 
 * Правила хранятся в виде отображения пары (текущее_состояние, текущий_символ) в Action.
 */
class Program{
private:
    std::map<std::pair<std::string , char >, Action> rules_;
public:
    /**
     * @brief Конструктор по умолчанию.
     */
    Program()=default;
    
    /**
     * @brief Добавить или обновить правило перехода.
     * @param currentState Текущее состояние машины.
     * @param currentSymbol Текущий считываемый символ.
     * @param action Действие, которое необходимо выполнить.
     */
    void addRule(const std::string& currentState, char currentSymbol, const Action& action);

    /**
     * @brief Поиск правила для текущего состояния и символа.
     * @param currentState Текущее состояние.
     * @param currentSymbol Считанный символ.
     * @return Указатель на структуру Action или nullptr, если правило не найдено.
     */
    const Action* searchForRule(const std::string& currentState, char currentSymbol) const;

    /**
     * @brief Удалить правило перехода.
     * @param currentState Состояние в правиле для удаления.
     * @param currentSymbol Символ в правиле для удаления.
     */
    void deleteRule(const std::string& currentState, char currentSymbol);

    /**
     * @brief Вывод всех правил программы в поток.
     */
    friend std::ostream& operator<<(std::ostream& os, const Program& rule);

    /**
     * @brief Чтение правил программы из потока в формате: `q_cur symb -> q_new symb dir`.
     */
    friend std::istream& operator>>(std::istream& is, Program& rule);
};