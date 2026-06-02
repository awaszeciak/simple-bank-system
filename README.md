# Simple Bank System

A simple console-based banking system written in C++17.

## Features

- Create customers
- Create bank accounts
- Deposit money
- Withdraw money
- Transfer money between accounts
- Store transaction history
- Filter transaction by type
- Validate business rules, such as:
    - preventing withdrawals when there are insufficient funds
    - preventing transfers to the same account
    - preventing operations on non-existing accounts
- Run service-level tests with CTest

## Technologies

- C++17
- CMake
- CTest
- Standard Template Library
- `std::optional`
- Object-oriented programming
- Repository pattern
- Service layer


## Architecture

The project is divided into several layers:
- **Domain layer**
Contains core business objects such as `Customer`, `Account`, and `Transaction`.

- **Repository layer**
Stores and manages domain objects in memory.

- **Service layer**
Contains banking business logic, such as deposits, withdrawals, transfers, and transaction history.

- **UI layer**
Provides a simple console interface for interacting with the system.

- **Tests**
Contains service-level tests for the most important banking operations.
