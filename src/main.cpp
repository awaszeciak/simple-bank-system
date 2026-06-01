#include <iostream>
#include "../include/domain/Customer.h"
#include "../include/domain/Account.h"
#include "../include/domain/Transaction.h"

#include "../include/repository/CustomerRepository.h"
#include "../include/repository/AccountRepository.h"
#include "../include/repository/TransactionRepository.h"

#include "../include/service/BankService.h"

int main() {
    
    CustomerRepository customerRepository;
    AccountRepository accountRepository;
    TransactionRepository transactionRepository;

    BankService bankService(customerRepository, accountRepository, transactionRepository);

    Customer customer1(1, "Anna", "Nowak", "annanowak@example.com");
    Customer customer2(2, "Jan", "Kowalski", "jankowalski@example.com");
    Customer customer3(1, "Marek", "Wiśniewski", "marekwisniewski@example.com");

    std::cout << "Creating customers:" << std::endl;

    if (bankService.createCustomer(customer1)) {
        std::cout << "Customer created: " << customer1.getFullName() << std::endl;
    } else {
        std::cout << "Could not create customer: " << customer1.getFullName() << std::endl;
    }

    if (bankService.createCustomer(customer2)) {
        std::cout << "Customer created: " << customer2.getFullName() << std::endl;
    } else {
        std::cout << "Could not create customer: " << customer2.getFullName() << std::endl;
    }

    if (bankService.createCustomer(customer3)) {
        std::cout << "Customer created: " << customer3.getFullName() << std::endl;
    } else {
        std::cout << "Could not create customer: " << customer3.getFullName() << std::endl;
    }

    std::cout << "\nCreating accounts:" << std::endl;

    Account account1("PL61109010140000071219812874", 1, 500.0);
    Account account2("PL27114020040000300201355387", 1, 1200.0);
    Account account3("PL05102055581111111111111111", 99, 300.0);

    if (bankService.createAccount(account1)) {
        std::cout << "Account created: " << account1.getAccountNumber() << std::endl;
    } else {
        std::cout << "Could not create account: " << account1.getAccountNumber() << std::endl;
    }

    if (bankService.createAccount(account2)) {
        std::cout << "Account created: " << account2.getAccountNumber() << std::endl;
    } else {
        std::cout << "Could not create account: " << account2.getAccountNumber() << std::endl;
    }

    if (bankService.createAccount(account3)) {
        std::cout << "Account created: " << account3.getAccountNumber() << std::endl;
    } else {
        std::cout << "Could not create account: " << account3.getAccountNumber() << std::endl;
    }

    std::cout << "\nTesting deposit:" << std::endl;

    if (bankService.deposit("PL61109010140000071219812874", 250)) {
        std::cout << "Deposit accepted" << std::endl;
    } else {
        std::cout << "Deposit rejected" << std::endl;
    }

    if (bankService.deposit("PL61109010140000071219812874", -100.0)) {
        std::cout << "Deposit accepted" << std::endl;
    } else {
        std::cout << "Deposit rejected" << std::endl;
    }

    std::cout << "\nTesting withdrawal:" << std::endl;

    if (bankService.withdraw("PL61109010140000071219812874", 200.0)) {
        std::cout << "Withdrawal accepted" << std::endl;
    } else {
        std::cout << "Withdrawal rejected" << std::endl;
    }

    if (bankService.withdraw("PL61109010140000071219812874", 10000.0)) {
        std::cout << "Withdrawal accepted" << std::endl;
    } else {
        std::cout << "Withdrawal rejected" << std::endl;
    }

    std::cout << "\nFinal account state:" << std::endl;

    std::optional<Account> foundAccount = bankService.findAccountByNumber("PL61109010140000071219812874");

    if (foundAccount.has_value()) {
        std::cout << "Account number: " << foundAccount->getAccountNumber() << std::endl;
        std::cout << "Owner ID: " << foundAccount->getCustomerId() << std::endl;
        std::cout << "Balance: " << foundAccount->getBalance() << " PLN" << std::endl;
    }
    else {
        std::cout << "Account not found." << std::endl;
    }

    std::cout << "\nTransaction history for account:" << std::endl;

    std::vector<Transaction> transactions = bankService.getTransactionsForAccount("PL61109010140000071219812874");

    if (transactions.empty()) {
        std::cout << "No transactions found" << std::endl;
    } else {
        for (const Transaction& transaction : transactions) {
            std::cout << "ID: " << transaction.getId() << ", Type: " << transaction.getType() << ", Amount: " << transaction.getAmount() << " PLN" << ", Description: " << transaction.getDescription() << std::endl;
        }
    }

    return 0;
}