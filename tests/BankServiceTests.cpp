#include <cassert>
#include <iostream>

#include "../include/service/BankService.h"
#include "../include/repository/CustomerRepository.h"
#include "../include/repository/AccountRepository.h"
#include "../include/repository/TransactionRepository.h"

void testCreateCustomer() {
    CustomerRepository customerRepository;
    AccountRepository accountRepository;
    TransactionRepository transactionRepository;

    BankService bankService(customerRepository, accountRepository, transactionRepository);

    Customer customer = bankService.createCustomer("Jan", "Kowalski", "jankowalski@example.com");

    assert(customer.getId() == 1);
    assert(customer.getFirstName() == "Jan");
    assert(customer.getLastName() == "Kowalski");
    assert(customer.getEmail() == "jankowalski@example.com");
}

void testCreateAccount() {
    CustomerRepository customerRepository;
    AccountRepository accountRepository;
    TransactionRepository transactionRepository;

    BankService bankService(customerRepository, accountRepository, transactionRepository);

    Customer customer = bankService.createCustomer("Jan", "Kowalski", "jankowalski@example.com");

    std::optional<Account> account = bankService.createAccount(customer.getId(), 500.0);

    assert(account.has_value());
    assert(account->getCustomerId() == customer.getId());
    assert(account->getBalance() == 500.0);
}

void testDeposit() {
    CustomerRepository customerRepository;
    AccountRepository accountRepository;
    TransactionRepository transactionRepository;

    BankService bankService(customerRepository, accountRepository, transactionRepository);

    Customer customer = bankService.createCustomer("Jan", "Kowalski", "jankowalski@example.com");
    std::optional<Account> account = bankService.createAccount(customer.getId(), 100.0);

    bool result = bankService.deposit(account->getAccountNumber(), 50);

    std::optional<Account> updatedAccount = bankService.findAccountByNumber(account->getAccountNumber());
    
    assert(result == true);
    assert(updatedAccount->getCustomerId() == customer.getId());
    assert(updatedAccount->getBalance() == 150.0);
}

void testWithdraw() {
    CustomerRepository customerRepository;
    AccountRepository accountRepository;
    TransactionRepository transactionRepository;

    BankService bankService(customerRepository, accountRepository, transactionRepository);

    Customer customer = bankService.createCustomer("Jan", "Kowalski", "jankowalski@example.com");
    std::optional<Account> account = bankService.createAccount(customer.getId(), 100.0);

    bool result = bankService.withdraw(account->getAccountNumber(), 40.0);

    std::optional<Account> updatedAccount = bankService.findAccountByNumber(account->getAccountNumber());

    assert(result == true);
    assert(updatedAccount.has_value());
    assert(updatedAccount->getBalance() == 60.0);
}

void testWithdrawTooMuchMoney() {
    CustomerRepository customerRepository;
    AccountRepository accountRepository;
    TransactionRepository transactionRepository;

    BankService bankService(customerRepository, accountRepository, transactionRepository);

    Customer customer = bankService.createCustomer("Jan", "Kowalski", "jankowalski@example.com");
    std::optional<Account> account = bankService.createAccount(customer.getId(), 100.0);

    bool result = bankService.withdraw(account->getAccountNumber(), 200.0);

    std::optional<Account> updatedAccount = bankService.findAccountByNumber(account->getAccountNumber());

    assert(result == false);
    assert(updatedAccount.has_value());
    assert(updatedAccount->getBalance() == 100.0);
}

void testDuplicateEmail() {
    CustomerRepository customerRepository;
    AccountRepository accountRepository;
    TransactionRepository transactionRepository;

    BankService bankService(customerRepository, accountRepository, transactionRepository);

    bankService.createCustomer("Jan", "Kowalski", "jankowalski@example.com");

    bool exceptionThrown = false;

    try {
        bankService.createCustomer("Adam", "Nowak", "jankowalski@example.com");
    } catch (const std::invalid_argument&) {
        exceptionThrown = true;
    }

    assert(exceptionThrown);
}

void testTransfer() {
    CustomerRepository customerRepository;
    AccountRepository accountRepository;
    TransactionRepository transactionRepository;

    BankService bankService(customerRepository, accountRepository, transactionRepository);

    Customer c1 = bankService.createCustomer("Jan", "Kowalski", "jankowalski@example.com");
    Customer c2 = bankService.createCustomer("Adam", "Nowak", "adamnowak@example.com");

    auto a1 = bankService.createAccount(c1.getId(), 1000.0);
    auto a2 = bankService.createAccount(c2.getId(), 100.0);

    bool result = bankService.transfer(a1->getAccountNumber(), a2->getAccountNumber(), 300.0);

    auto updatedA1 = bankService.findAccountByNumber(a1->getAccountNumber());
    auto updatedA2 = bankService.findAccountByNumber(a2->getAccountNumber());

    assert(result);
    assert(updatedA1->getBalance() == 700.0);
    assert(updatedA2->getBalance() == 400.0);

}


void testTransactionHistory() {
    CustomerRepository customerRepository;
    AccountRepository accountRepository;
    TransactionRepository transactionRepository;

    BankService bankService(customerRepository, accountRepository, transactionRepository);

    Customer customer = bankService.createCustomer("Jan", "Kowalski", "jankowalski@example.com");

    auto account = bankService.createAccount(customer.getId(), 100.0);

    bankService.deposit(account->getAccountNumber(), 50.0);
    bankService.withdraw(account->getAccountNumber(), 20.0);

    auto transactions = bankService.getTransactionsForAccount(account->getAccountNumber());

    assert(transactions.size() == 2);
}

void testDepositNegativeAmount() {
    CustomerRepository customerRepository;
    AccountRepository accountRepository;
    TransactionRepository transactionRepository;

    BankService bankService(customerRepository, accountRepository, transactionRepository);

    Customer customer = bankService.createCustomer("Jan", "Kowalski", "jankowalski@example.com"); 
    auto account = bankService.createAccount(customer.getId(), 100.0);

    bool exceptionThrown = false;

    try {
        bankService.deposit(account->getAccountNumber(), -50.0);
    } catch (const std::invalid_argument&) {
        exceptionThrown = true;
    }

    assert(exceptionThrown);
}

void testTransferNotEnoughMoney() {
    CustomerRepository customerRepository;
    AccountRepository accountRepository;
    TransactionRepository transactionRepository;

    BankService bankService(customerRepository, accountRepository, transactionRepository);

    Customer c1 = bankService.createCustomer("Jan", "Kowalski", "jankowalski@example.com");
    Customer c2 = bankService.createCustomer("Adam", "Nowak", "adamnowak@example.com");

    auto a1 = bankService.createAccount(c1.getId(), 100.0);
    auto a2 = bankService.createAccount(c2.getId(), 50.0);

    bool result = bankService.transfer(a1->getAccountNumber(), a2->getAccountNumber(), 300.0);

    auto updatedA1 = bankService.findAccountByNumber(a1->getAccountNumber());
    auto updatedA2 = bankService.findAccountByNumber(a2->getAccountNumber());

    assert(result == false);
    assert(updatedA1.has_value());
    assert(updatedA2.has_value());

    assert(updatedA1->getBalance() == 100.0);
    assert(updatedA2->getBalance() == 50.0);
}

void testTransferToSameAccount() {
    CustomerRepository customerRepository;
    AccountRepository accountRepository;
    TransactionRepository transactionRepository;

    BankService bankService(customerRepository, accountRepository, transactionRepository);

    Customer customer = bankService.createCustomer("Jan", "Kowalski", "jankowalski@example.com");
  
    auto account = bankService.createAccount(customer.getId(), 100.0);

    bool result = bankService.transfer(account->getAccountNumber(), account->getAccountNumber(), 100.0);

    auto updatedAccount = bankService.findAccountByNumber(account->getAccountNumber());

    assert(result == false);
    assert(updatedAccount.has_value());
    assert(updatedAccount->getBalance() == 100.0);

}

int main() {
    testCreateAccount();
    testCreateCustomer();
    testDeposit();
    testWithdraw();
    testWithdrawTooMuchMoney();
    testDuplicateEmail();
    testTransfer();
    testTransactionHistory();
    testDepositNegativeAmount();
    testTransferNotEnoughMoney();
    testTransferToSameAccount();

    std::cout << "All BankService tests passed.\n";

    return 0;
}