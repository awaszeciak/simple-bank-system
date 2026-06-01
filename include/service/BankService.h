#ifndef BANK_SERVICE_H
#define BANK_SERVICE_H

#include <string>
#include <optional>
#include <vector>

#include "../domain/Account.h"
#include "../domain/Customer.h"
#include "../repository/IAccountRepository.h"
#include "../repository/ICustomerRepository.h"
#include "../repository/ITransactionRepository.h"

class BankService {
    private:
        ICustomerRepository& customerRepository;
        IAccountRepository& accountRepository;
        ITransactionRepository& transactionRepository;
        int nextTransactionId;
        int nextCustomerId;

        std::string generateAccountNumber();

    public:
        BankService(ICustomerRepository& customerRepository, IAccountRepository& accountRepository, ITransactionRepository& transactionRepository);

        Customer createCustomer(const std::string& firstName, const std::string& lastName, const std::string& email);
        std::optional<Account> createAccount(int customerId, double initialBalance);

        std::vector<Transaction> getTransactionsForAccount(const std::string& accountNumber) const;

        bool deposit(const std::string& accountNumber, double amount);
        bool withdraw(const std::string& accountNumber, double amount);

        std::optional<Customer> findCustomerById(int id) const;
        std::optional<Account> findAccountByNumber(const std::string& accountNumber) const;

        bool transfer(const std::string& sourceAccountNumber, const std::string& targetAccountNumber, double amount);

        std::vector<Customer> getAllCustomers() const;
        std::vector<Account> getAllAccounts() const;
        std::vector<Transaction> getTransactionsForAccountByType(const std::string& accountNumber, TransactionType type) const;
};

#endif