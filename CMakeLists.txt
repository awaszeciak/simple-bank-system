cmake_minimum_required(VERSION 3.16)

project(SimpleBankSystem)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


set(CMAKE_EXPORT_COMPILE_COMMANDS ON)


set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

set(CORE_SOURCES
    src/domain/Account.cpp
    src/domain/Customer.cpp
    src/domain/Transaction.cpp

    src/repository/AccountRepository.cpp
    src/repository/CustomerRepository.cpp
    src/repository/TransactionRepository.cpp

    src/service/BankService.cpp

    src/utils/InputValidator.cpp
)

add_executable(SimpleBankSystem
    src/main.cpp
    src/ui/ConsoleUI.cpp
    ${CORE_SOURCES}
)

target_include_directories(SimpleBankSystem PRIVATE
    include
)

enable_testing()

add_executable(BankServiceTests
    tests/BankServiceTests.cpp
    ${CORE_SOURCES}
)

target_include_directories(BankServiceTests PRIVATE
    include
)

add_test(NAME BankServiceTests COMMAND BankServiceTests)

find_package(Qt5 COMPONENTS Widgets REQUIRED)

add_executable(SimpleBankSystemGui
    src/gui/main.cpp
    src/gui/MainWindow.cpp
    src/gui/MainWindow.h
    ${CORE_SOURCES}
)

target_include_directories(SimpleBankSystemGui PRIVATE
    include
)

target_link_libraries(SimpleBankSystemGui PRIVATE
    Qt5::Widgets
)