#include "../../include/repository/TransactionRepository.h"

void TransactionRepository::addTransaction(const Transaction& transaction) {
    transactions.push_back(transaction);
}

std::vector<Transaction> TransactionRepository::getAllTransactions() const {
    return transactions;
}

std::vector<Transaction> TransactionRepository::findByAccountNumber(const std::string& accountNumber) const {
    std::vector<Transaction> result;

    for (const Transaction& transaction : transactions) {
        if (transaction.getAccountNumber() == accountNumber) {
            result.push_back(transaction);
        }
    }
    return result;
}