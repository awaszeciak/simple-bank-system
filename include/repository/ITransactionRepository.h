#ifndef I_TRANSACTION_REPOSITORY_H
#define I_TRANSACTION_REPOSITORY_H

#include <vector>
#include <optional>
#include <string>
#include "../domain/Transaction.h"

class ITransactionRepository {
    public:
        virtual ~ITransactionRepository() = default;
        virtual void addTransaction(const Transaction& transaction) = 0;
        virtual std::vector<Transaction> getAllTransactions() const = 0;
        virtual std::vector<Transaction> findByAccountNumber(const std::string& accountNumber) const = 0;
        virtual std::vector<Transaction> findByAccountNumberAndType(const std::string& accountNumber, TransactionType type) const = 0;
};

#endif