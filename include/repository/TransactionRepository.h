#ifndef TRANSACTION_REPOSITORY_H
#define TRANSACTION_REPOSITORY_H

#include <vector>
#include <string>

#include "../domain/Transaction.h"

class TransactionRepository {
    private:
        std::vector<Transaction> transactions;

    public:
        void addTransaction(const Transaction& transaction);

        std::vector<Transaction> getAllTransactions() const;

        std::vector<Transaction> findByAccountNumber(const std::string& accountNumber) const;
};

#endif