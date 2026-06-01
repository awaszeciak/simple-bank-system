#include "../../include/service/BankService.h"

BankService::BankService(CustomerRepository& customerRepository, AccountRepository& accountRepository) 
    : customerRepository(customerRepository), accountRepository(accountRepository) {}

bool BankService::createCustomer(const Customer& customer) {
    return customerRepository.addCustomer(customer);
}

bool BankService::createAccount(const Account& account) {
    if (!customerRepository.findById(account.getCustomerId()).has_value()) {
        return false;
    }
    if (accountRepository.findByAccountNumber(account.getAccountNumber()).has_value()) {
        return false;
    }
    accountRepository.addAccount(account);
    return true;
}

bool BankService::deposit(const std::string& accountNumber, double amount) {
    std::optional<Account> account = accountRepository.findByAccountNumber(accountNumber);

    if (!account.has_value()) {
        return false;
    }

    Account updatedAccount = account.value();

    if (!updatedAccount.deposit(amount)) {
        return false;
    }

    return accountRepository.updateAccount(updatedAccount);
}


bool BankService::withdraw(const std::string& accountNumber, double amount) {
    std::optional<Account> account = accountRepository.findByAccountNumber(accountNumber);

    if (!account.has_value()) {
        return false;
    }

    Account updatedAccount = account.value();

    if (!updatedAccount.withdraw(amount)) {
        return false;
    }

    return accountRepository.updateAccount(updatedAccount);
}

std::optional<Customer> BankService::findCustomerById(int id) const {
    return customerRepository.findById(id);
}

std::optional<Account> BankService::findAccountByNumber(const std::string& accountNumber) const {
    return accountRepository.findByAccountNumber(accountNumber);
}