cmake_minimum_required(VERSION 3.10)

# Имя проекта
project(TuringMachineProject CXX)

# Стандарт C++
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Включаем предупреждения компилятора
if (MSVC)
    add_compile_options(/W4)
else()
    add_compile_options(-Wall -Wextra)
endif()

add_library(TuringMachine_lib STATIC Tape.hpp Tape.cpp Program.hpp Program.cpp TuringMachine.hpp TuringMachine.cpp)

# Создаем исполняемый файл из исходников
add_executable(turingMachine_app main.cpp)
target_link_libraries(turingMachine_app PRIVATE TuringMachine_lib)

enable_testing()

add_executable(run_tests tests.cpp)

target_link_libraries(run_tests PRIVATE TuringMachine_lib UnitTest++)

add_test(NAME TuringMachine_AllTests COMMAND run_tests)