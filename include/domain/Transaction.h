#ifndef TRANSACTION_H
#define TRANSACTION_H

#include <string>

class Transaction {
    private:
        int id;
        std::string accountNumber;
        std::string type;
        double amount;
        std::string description;
    public:
        Transaction(int id, const std::string& accountNumber, const std::string& type, double amount, const std::string& description);

        int getId() const;
        std::string getAccountNumber() const;
        std::string getType() const;
        double getAmount() const;
        std::string getDescription() const;

};

#endif