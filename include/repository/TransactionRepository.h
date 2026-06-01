#ifndef TRANSACTION_REPOSITORY_H
#define TRANSACTION_REPOSITORY_H

#include <vector>
#include <string>

#include "../domain/Transaction.h"
#include "ITransactionRepository.h"

class TransactionRepository : public ITransactionRepository {
    private:
        std::vector<Transaction> transactions;

    public:
        void addTransaction(const Transaction& transaction);
        std::vector<Transaction> getAllTransactions() const;
        std::vector<Transaction> findByAccountNumber(const std::string& accountNumber) const;
        std::vector<Transaction> findByAccountNumberAndType(const std::string& accountNumber, TransactionType type) const override;
};

#endif