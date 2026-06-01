#include "../../include/service/BankService.h"

BankService::BankService(CustomerRepository& customerRepository, AccountRepository& accountRepository, TransactionRepository& transactionRepository)
    : customerRepository(customerRepository), accountRepository(accountRepository), transactionRepository(transactionRepository), nextTransactionId(1), nextCustomerId(1) {}

Customer BankService::createCustomer(const std::string& firstName, const std::string& lastName, const std::string &email) {
    Customer customer(nextCustomerId++, firstName, lastName, email);
    customerRepository.addCustomer(customer);

    return customer;
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

bool BankService::transfer(const std::string& sourceAccountNumber, const std::string& targetAccountNumber, double amount) {
    if (amount <= 0) {
        return false;
    }

    if (sourceAccountNumber == targetAccountNumber) {
        return false;
    }

    std::optional<Account> sourceAccount = accountRepository.findByAccountNumber(sourceAccountNumber);
    std::optional<Account> targetAccount = accountRepository.findByAccountNumber(targetAccountNumber);

    if (!sourceAccount.has_value() || !targetAccount.has_value()) {
        return false;
    }

    Account updatedSourceAccount = sourceAccount.value();
    Account updatedTargetAccount = targetAccount.value();

    if (!updatedSourceAccount.withdraw(amount)) {
        return false;
    }

    if (!updatedTargetAccount.withdraw(amount)) {
        return false;
    }

    if (!accountRepository.updateAccount(updatedSourceAccount)) {
        return false;
    }

    if (!accountRepository.updateAccount(updatedTargetAccount)) {
        return false;
    }

    Transaction outgoingTransaction(nextTransactionId++, sourceAccountNumber, "TRANSFER_OUT", amount, "Money transferred to account " + targetAccountNumber);
    Transaction incomingTransaction(nextTransactionId++, targetAccountNumber, "TRANSFER_IN", amount, "Money received from account " + sourceAccountNumber);
    
    transactionRepository.addTransaction(outgoingTransaction);
    transactionRepository.addTransaction(incomingTransaction);

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