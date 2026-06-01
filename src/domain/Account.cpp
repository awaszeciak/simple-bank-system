#include "../../include/domain/Account.h"

Account::Account(const std::string& accountNumber, int customerId, double initialBalance)
    : accountNumber(accountNumber), customerId(customerId), balance(initialBalance) {}

std::string Account::getAccountNumber() const {
    return accountNumber;
}

int Account::getCustomerId() const {
    return customerId;
}

double Account::getBalance() const {
    return balance;
}

bool Account::deposit(double amount) {
    if (amount <= 0) {
        return false;
    }
    balance += amount;
    return true;
}

bool Account::withdraw(double amount) {
    if (amount <= 0 || amount > balance) {
        return false;
    }
    balance -= amount;
    return true;
}