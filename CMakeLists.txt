cmake_minimum_required(VERSION 3.20)
project(lab6_coroutines VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if (MSVC)
    add_compile_options(/W4 /EHsc /utf-8)
else()
    add_compile_options(-Wall -Wextra -Wpedantic -fexceptions)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")
endif()

include_directories(include)

add_library(lab6_core
    action.cpp
    game_state.cpp
    strategy.cpp
    player_coroutine.cpp
    game_engine.cpp
    input_validator.cpp
)

add_executable(lab6_game
    main.cpp
)
target_link_libraries(lab6_game PRIVATE lab6_core)

enable_testing()

add_executable(lab6_tests
    tests/test_game.cpp
)
target_link_libraries(lab6_tests PRIVATE lab6_core)

add_test(NAME UnitAndIntegrationTests COMMAND lab6_tests)
