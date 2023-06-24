cmake_minimum_required(VERSION 3.23)
project(questions LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED on)

set(CMAKE_CXX_FLAGS "-Wall -Wextra -Wpedantic")
set(CMAKE_CXX_FLAGS_DEBUG "-fdiagnostics-color=always -fsanitize=undefined,address,float-divide-by-zero,float-cast-overflow -g3 -O0")
set(CMAKE_CXX_FLAGS_RELEASE "-g0 -O2")

find_package(Qt6 REQUIRED
    Core
    Widgets
    Svg
)

qt_standard_project_setup()

add_executable(questions)

target_sources(questions PRIVATE
    main.cpp
    MainWindow.cpp
    Scene.cpp
    Node.cpp
    Arrow.cpp
    NodeDialog.cpp
    ArrowDialog.cpp
)

target_link_libraries(questions PRIVATE
    Qt6::Core
    Qt6::Widgets
    Qt6::Svg
)

qt_add_resources(questions "imgs"
    PREFIX "/"
    BASE "img/"
    FILES
        "img/none.svg"
)
