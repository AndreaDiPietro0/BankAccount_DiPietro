cmake_minimum_required(VERSION 3.28)
project(BankAccount)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

set(CMAKE_PREFIX_PATH "/opt/homebrew/opt/qt")

find_package(Qt6 REQUIRED COMPONENTS Core Widgets)

add_executable(BankAccount
        main.cpp
        MainWindow.cpp
        MainWindow.h
        MainWindow.ui
        welcomedialog.cpp
        welcomedialog.h
        welcomedialog.ui
)
target_link_libraries(BankAccount PRIVATE Qt6::Widgets BankCore)

# Scarica GoogleTest
include(FetchContent)
FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/refs/heads/main.zip
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()

# file delle  classi Transaction e BankAccount
add_library(BankCore STATIC
        Transaction.cpp
        Transaction.h
        BankAccount.cpp
        BankAccount.h
)

# eseguibile principale
target_link_libraries(BankAccount PRIVATE BankCore)

#Eseguibile dei Test
add_executable(BankTests
        tests/testBankAccount.cpp
        tests/testTransaction.cpp
)
target_link_libraries(BankTests PRIVATE gtest_main BankCore)

#Integrazione CLion
include(GoogleTest)
gtest_discover_tests(BankTests)