#ifndef BANK_SERVICE_H
#define BANK_SERVICE_H

#include <string>
#include <optional>

#include "../domain/Account.h"
#include "../domain/Customer.h"
#include "../repository/AccountRepository.h"
#include "../repository/CustomerRepository.h"

class BankService {
    private:
        CustomerRepository& customerRepository;
        AccountRepository& accountRepository;

    public:
        BankService(CustomerRepository& customerRepository, AccountRepository& accountRepository);

        bool createCustomer(const Customer& customer);
        bool createAccount(const Account& account);

        bool deposit(const std::string& accountNumber, double amount);
        bool withdraw(const std::string& accountNumber, double amount);

        std::optional<Customer> findCustomerById(int id) const;
        std::optional<Account> findAccountByNumber(const std::string& accountNumber) const;
};

#endif