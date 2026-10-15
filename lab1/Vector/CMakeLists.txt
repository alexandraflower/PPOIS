cmake_minimum_required(VERSION 3.10)

# Имя проекта
project(Vector3DProject CXX)

# Стандарт C++
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Включаем предупреждения компилятора
if (MSVC)
    add_compile_options(/W4)
else()
    add_compile_options(-Wall -Wextra)
endif()

add_library(Vector3D_lib STATIC Vector3D.cpp Vector3D.hpp)

# Создаем исполняемый файл из исходников
add_executable(vector_app main.cpp)
target_link_libraries(vector_app PRIVATE Vector3D_lib)

enable_testing()

add_executable(run_tests tests.cpp)

target_link_libraries(run_tests PRIVATE Vector3D_lib UnitTest++)

add_test(NAME Vector3D_AllTests COMMAND run_tests)