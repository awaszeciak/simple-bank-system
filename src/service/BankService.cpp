#include "../../include/service/BankService.h"
#include <random>

BankService::BankService(ICustomerRepository& customerRepository, IAccountRepository& accountRepository, ITransactionRepository& transactionRepository)
    : customerRepository(customerRepository), accountRepository(accountRepository), transactionRepository(transactionRepository), nextTransactionId(1), nextCustomerId(1) {}

Customer BankService::createCustomer(const std::string& firstName, const std::string& lastName, const std::string &email) {
    Customer customer(nextCustomerId++, firstName, lastName, email);
    customerRepository.addCustomer(customer);

    return customer;
}

std::string BankService::generateAccountNumber() {
    static std::random_device randomDevice;
    static std::mt19937 generator(randomDevice());
    static std::uniform_int_distribution<int> digitDistribution(0, 9);

    std::string accountNumber = "PL";

    for (int i = 0; i < 26; i++) {
        accountNumber += std::to_string(digitDistribution(generator));
    }

    return accountNumber;
}

std::vector<Customer> BankService::getAllCustomers() const {
    return customerRepository.getAllCustomers();
}

std::vector<Account> BankService::getAllAccounts() const {
    return accountRepository.getAllAccounts();
}

std::optional<Account> BankService::createAccount(int customerId, double initialBalance) {
    if (!customerRepository.findById(customerId).has_value()) {
        return std::nullopt;
    }
    if (initialBalance < 0) {
        return std::nullopt;
    }
    std::string accountNumber = generateAccountNumber();

    while (accountRepository.findByAccountNumber(accountNumber).has_value()) {
        accountNumber = generateAccountNumber();
    }

    Account account(accountNumber, customerId, initialBalance);
    accountRepository.addAccount(account);
    
    return account;
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

    Transaction transaction(nextTransactionId++, accountNumber, TransactionType::Deposit, amount, "Money deposited to account");
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

    Transaction transaction(nextTransactionId++, accountNumber, TransactionType::Withdrawal, amount, "Money withdrawn from account");
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

    if (!updatedTargetAccount.deposit(amount)) {
        return false;
    }

    if (!accountRepository.updateAccount(updatedSourceAccount)) {
        return false;
    }

    if (!accountRepository.updateAccount(updatedTargetAccount)) {
        return false;
    }

    Transaction outgoingTransaction(nextTransactionId++, sourceAccountNumber, TransactionType::TransferOut, amount, "Money transferred to account " + targetAccountNumber);
    Transaction incomingTransaction(nextTransactionId++, targetAccountNumber, TransactionType::TransferIn, amount, "Money received from account " + sourceAccountNumber);
    
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

std::vector<Transaction> BankService::getTransactionsForAccountByType(const std::string& accountNumber, TransactionType type) const {
    return transactionRepository.findByAccountNumberAndType(accountNumber, type);
}

