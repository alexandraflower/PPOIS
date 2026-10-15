#pragma once
#include "Tape.hpp"
#include "Program.hpp"
#include <string>

/**
 * @file TuringMachine.h
 * @brief Класс исполнителя машины Тьюринга.
 */

/**
 * @class TuringMachine
 * @brief Управляет выполнением алгоритма на ленте в соответствии с заданной программой.
 */
class TuringMachine{
private:
    Tape& tape_; /**< Ссылка на исполняемую ленту */
    Program& program_; /**< Ссылка на набор правил (программу) */
    std::string currentState_; /**< Текущее состояние машины */
    bool isHalted_; /**< Флаг остановки машины */
public:
    /**
     * @brief Конструктор машины Тьюринга.
     * @param tape Ссылка на используемую ленту.
     * @param program Ссылка на используемую программу переходов.
     * @param currentState Начальное состояние (например, "q0").
     */
    TuringMachine(Tape& tape, Program& program, std::string currentState);

    /**
     * @brief Выполнить один шаг вычисления машины Тьюринга.
     * @return Ссылка на текущий объект машины.
     */
    TuringMachine& operator++();

    /**
     * @brief Запустить непрерывный цикл вычислений до остановки или достижения лимита.
     * @param needLog Выводить ли текущую конфигурацию ленты на каждом шаге.
     * @param maxSteps Максимальное количество шагов до принудительной остановки (защита от зацикливания).
     */
    void run(bool needLog, size_t maxSteps=100000);

    /**
     * @brief Получить текущее состояние машины.
     * @return Название текущего состояния.
     */
    const std::string getCurrentState() const;

    /**
     * @brief Проверить, завершила ли машина свою работу.
     * @return true, если достигнуто состояние "halt" или отсутствует подходящее правило.
     */
    bool isHalted() const;
};