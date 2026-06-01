#ifndef ACCOUNT_H
#define ACCOUNT_H

#include <string>

class Account {
    private:
        std::string accountNumber;
        int customerId;
        double balance;
    public:
        Account(const std::string& accountNumber, int customerId, double initialBalance = 0.0);

        std::string getAccountNumber() const;
        int getCustomerId() const;
        double getBalance() const;

        bool deposit(double amount);
        bool withdraw(double amount);
};

#endif