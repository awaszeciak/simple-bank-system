#include "../include/domain/Transaction.h"

Transaction::Transaction(int id, const std::string& accountNumber, const std::string& type, double amount, const std::string& description)
    : id(id), accountNumber(accountNumber), type(type), amount(amount), description(description) {}

int Transaction::getId() const {
    return id;
}

std::string Transaction::getAccountNumber() const {
    return accountNumber;
}

std::string Transaction::getType() const {
    return type;
}

double Transaction::getAmount() const {
    return amount;
}

std::string Transaction::getDescription() const {
    return description;
}

