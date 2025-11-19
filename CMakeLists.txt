cmake_minimum_required(VERSION 3.28)
project(BankAccount)

set(CMAKE_CXX_STANDARD 17)

add_executable(BankAccount main.cpp
        Transaction.cpp
        Transaction.h
        BankAccount.cpp
        BankAccount.h
        tests/testBankAccount.cpp
        tests/testTransaction.cpp)

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