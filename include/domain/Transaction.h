#ifndef TRANSACTION_H
#define TRANSACTION_H

#include <chrono>
#include <string>

enum class TransactionType {
    Deposit,
    Withdrawal,
    TransferIn,
    TransferOut
};


class Transaction {
    private:
        int id;
        std::string accountNumber;
        TransactionType type;
        double amount;
        std::string description;
        std::chrono::system_clock::time_point timestamp;

    public:
        Transaction(int id, const std::string& accountNumber, TransactionType type, double amount, const std::string& description);

        int getId() const;
        std::string getAccountNumber() const;
        TransactionType getType() const;
        std::string getTypeAsString() const;
        double getAmount() const;
        std::string getDescription() const;
        std::chrono::system_clock::time_point getTimestamp() const;
        std::string getFormattedTimestamp() const;

};

#endif