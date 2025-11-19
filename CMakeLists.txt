cmake_minimum_required(VERSION 3.28)
project(BankAccount)

set(CMAKE_CXX_STANDARD 17)

add_executable(BankAccount main.cpp
        Transaction.cpp
        Transaction.h
        BankAccount.cpp
        BankAccount.h)
