#include "../../include/service/BankService.h"

BankService::BankService(CustomerRepository& customerRepository, AccountRepository& accountRepository, TransactionRepository& transactionRepository)
    : customerRepository(customerRepository), accountRepository(accountRepository), transactionRepository(transactionRepository), nextTransactionId(1) {}

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

    if (!accountRepository.updateAccount(updatedAccount)) {
        return false;
    }

    Transaction transaction(nextTransactionId++, accountNumber, "DEPOSIT", amount, "Money deposited to account");
    transactionRepository.addTransaction(transaction);

    return true;
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

    if (!accountRepository.updateAccount(updatedAccount)) {
        return false;
    }

    Transaction transaction(nextTransactionId++, accountNumber, "WITHDRAW", amount, "Money withdrawn from account");
    transactionRepository.addTransaction(transaction);

    return true;
}

std::optional<Customer> BankService::findCustomerById(int id) const {
    return customerRepository.findById(id);
}

std::optional<Account> BankService::findAccountByNumber(const std::string& accountNumber) const {
    return accountRepository.findByAccountNumber(accountNumber);
}

std::vector<Transaction> BankService::getTransactionsForAccount(const std::string& accountNumber) const {
    return transactionRepository.findByAccountNumber(accountNumber);
}