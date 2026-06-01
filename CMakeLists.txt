cmake_minimum_required(VERSION 3.10)

project(SimpleBankSystem)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(include)

add_executable(SimpleBankSystem
    src/main.cpp

    src/domain/Account.cpp
    src/domain/Customer.cpp
    src/domain/Transaction.cpp

    src/repository/AccountRepository.cpp
    src/repository/CustomerRepository.cpp
    src/repository/TransactionRepository.cpp

    src/service/BankService.cpp

    src/ui/ConsoleUI.cpp

    src/utils/InputValidator.cpp
)

add_executable(BankServiceTests
    tests/BankServiceTests.cpp

    src/domain/Account.cpp
    src/domain/Customer.cpp
    src/domain/Transaction.cpp

    src/repository/AccountRepository.cpp
    src/repository/CustomerRepository.cpp
    src/repository/TransactionRepository.cpp
    src/service/BankService.cpp
    src/utils/InputValidator.cpp
)