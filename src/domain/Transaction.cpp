#include "../include/domain/Transaction.h"
#include <ctime>

Transaction::Transaction(int id, const std::string& accountNumber, TransactionType type, double amount, const std::string& description)
    : id(id), accountNumber(accountNumber), type(type), amount(amount), description(description), timestamp(std::chrono::system_clock::now()) {}

int Transaction::getId() const {
    return id;
}

std::string Transaction::getAccountNumber() const {
    return accountNumber;
}

TransactionType Transaction::getType() const {
    return type;
}

double Transaction::getAmount() const {
    return amount;
}

std::string Transaction::getDescription() const {
    return description;
}

std::string Transaction::getFormattedTimestamp() const {
    auto t = std::chrono::system_clock::to_time_t(timestamp);
    std::string ts = std::ctime(&t);
    ts.pop_back();
    return ts;
}

std::chrono::system_clock::time_point Transaction::getTimestamp() const {
    return timestamp;
}

std::string Transaction::getTypeAsString() const {
    switch (type) {
        case TransactionType::Deposit:
            return "Deposit";
        
        case TransactionType::Withdrawal:
            return "Withdrawal";

        case TransactionType::TransferIn:
            return "Transfer In";

        case TransactionType::TransferOut:
            return "Transfer Out";

        default:
            return "Unknown";
    }
}