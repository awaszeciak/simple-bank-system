#include <iostream>
#include <optional>
#include <string>
#include <vector>

#include "../../include/ui/ConsoleUI.h"

#include "../../include/domain/Customer.h"
#include "../../include/domain/Account.h"
#include "../../include/domain/Transaction.h"

#include "../../include/utils/InputValidator.h"

ConsoleUI::ConsoleUI(BankService& bankService) 
    : bankService(bankService) {}

void ConsoleUI::showMenu() const {
    std::cout << "\n==== Simple Bank System ====\n";
    std::cout << "1. Create customer\n";
    std::cout << "2. Create account\n";
    std::cout << "3. Deposit money\n";
    std::cout << "4. Withdraw money\n";
    std::cout << "5. Transfer money\n";
    std::cout << "6. Show account info\n";
    std::cout << "7. Show transaction history (with filter)\n";
    std::cout << "8. Show all customers\n";
    std::cout << "9. Show all accounts\n";
    std::cout << "0. Exit\n";
    std::cout << "Choose option: ";
}

void ConsoleUI::run() {
    int option;

    do {
        showMenu();
        std::cin >> option;

        switch (option) {
            case 1:
                createCustomer();
                break;
            case 2:
                createAccount();
                break;
            case 3:
                depositMoney();
                break;
            case 4:
                withdrawMoney();
                break;
            case 5:
                transferMoney();
                break;
            case 6:
                showAccountInfo();
                break;
            case 7:
                showFilteredTransactionHistory(); //showtransactionhistory
                break;
            case 8:
                showAllCustomers();
                break;
            case 9:
                showAllAccounts();
                break;
            case 0:
                std::cout << "Exiting application\n";
                break;
            default:
                std::cout << "Invalid option\n";
        }

    } while (option != 0);
}

void ConsoleUI::createCustomer() {
    std::string firstName;
    std::string lastName;
    std::string email;

    std::cout << "First name: ";
    std::cin >> firstName;

    std::cout << "Last name: ";
    std::cin >> lastName;

    std::cout << "Email: ";
    std::cin >> email;

    if (firstName.empty() || lastName.empty()) {
        std::cout << "First name and last name cannot be empty\n";
        return;
    }

    if (!InputValidator::isValidEmail(email)) {
        std::cout << "Invalid email address\n";
        return;
    }

    try {
        Customer customer = bankService.createCustomer(firstName, lastName, email);
        std:: cout << "Customer created successfully. ID: " << customer.getId() << "\n";
    } catch (const std::exception& exception) {
        std::cout << "Error: " << exception.what() << "\n";
    }



}

void ConsoleUI::createAccount() {
    
    int customerId;
    double initialBalance;

    std::cout << "Customer ID: ";
    std::cin >> customerId;

    std::cout << "Initial balance: ";
    std::cin >> initialBalance;

    std::optional<Account> account = bankService.createAccount(customerId, initialBalance);

    if (account.has_value()) {
        std::cout << "Account created successfully\n";
        std::cout << "Assigned account number: " << account->getAccountNumber() << "\n";
        std::cout << "Initial balance: " << account->getBalance() << " PLN\n";
    } else {
        std::cout << "Account creation failed. Customer does not exist or initial balance is invalid\n";
    }
}

void ConsoleUI::depositMoney() {
    std::string accountNumber;
    double amount;

    std::cout << "Account number: ";
    std::cin >> accountNumber;

    std::cout << "Amount: ";
    std::cin >> amount;

    if (!InputValidator::isValidAccountNumber(accountNumber)) {
        std::cout << "Invalid account number format\n";
        return;
    }

    if (!InputValidator::isPositiveAmount(amount)) {
        std::cout << "Amount must be greater than 0\n";
        return;
    }

    if (bankService.deposit(accountNumber, amount)) {
        std::cout << "Deposit completed successfully\n";
    } else {
        std::cout << "Deposit rejected\n";
    }
}

void ConsoleUI::withdrawMoney() {
    std::string accountNumber;
    double amount;

    std::cout << "Account number: ";
    std::cin >> accountNumber;

    std::cout << "Amount: ";
    std::cin >> amount;

    if (!InputValidator::isValidAccountNumber(accountNumber)) {
        std::cout << "Invalid account number format\n";
        return;
    }

    if (!InputValidator::isPositiveAmount(amount)) {
        std::cout << "Amount must be greater than 0\n";
        return;
    }

    if (bankService.withdraw(accountNumber, amount)) {
        std::cout << "Withdrawal completed successfully\n";
    } else {
        std::cout << "Withdrawal rejected\n";
    }
}

void ConsoleUI::transferMoney() {
    std::string sourceAccountNumber;
    std::string targetAccountNumber;
    double amount;

    std::cout << "Source account number: ";
    std::cin >> sourceAccountNumber;

    std::cout << "Target account number: ";
    std::cin >> targetAccountNumber;

    std::cout << "Amount: ";
    std::cin >> amount;

    if (!InputValidator::isValidAccountNumber(sourceAccountNumber)) {
        std::cout << "Invalid source account number format\n";
        return;
    }

    if (!InputValidator::isValidAccountNumber(targetAccountNumber)) {
        std::cout << "Invalid target account number format\n";
        return;
    }

    if (!InputValidator::isPositiveAmount(amount)) {
        std::cout << "Amount must be greater than 0\n";
        return;
    }

    if (bankService.transfer(sourceAccountNumber, targetAccountNumber, amount)) {
        std::cout << "Transfer completed successfully\n";
    } else {
        std::cout << "Transfer rejected\n";
    }
}

void ConsoleUI::showAccountInfo() const {
    std::string accountNumber;

    std::cout << "Account number: ";
    std::cin >> accountNumber;

    if (!InputValidator::isValidAccountNumber(accountNumber)) {
        std::cout << "Invalid account number format\n";
        return;
    }

    std::optional<Account> account = bankService.findAccountByNumber(accountNumber);

    if (!account.has_value()) {
        std::cout << "Account not found\n";
        return;
    }
    
    std::cout << "\nAccount number: " << account->getAccountNumber() << "\n";
    std::cout << "Customer ID: " << account->getCustomerId() << "\n";
    std::cout << "Balance: " << account->getBalance() << " PLN\n";
}

void ConsoleUI::showAllCustomers() const {
    std::vector<Customer> customers = bankService.getAllCustomers();

    if (customers.empty()) {
        std::cout << "No customers found\n";
        return;
    }

    std::cout << "\nCustomers:\n";

    for (const Customer& customer: customers) {
        std::cout << " - ID: " << customer.getId() << ", Name: " << customer.getFullName() << ", Email: " << customer.getEmail() << "\n";
    }
}

void ConsoleUI::showAllAccounts() const {
    std::vector<Account> accounts = bankService.getAllAccounts();

    if (accounts.empty()) {
        std::cout << "No accounts found\n";
        return;
    }

    std::cout << "\nAccounts:\n";

    for (const Account& account : accounts) {
        std::optional<Customer> owner = bankService.findCustomerById(account.getCustomerId());

        std::cout << " - Account number: " << account.getAccountNumber() << ", Balance: " << account.getBalance() << " PLN";

        if (owner.has_value()) {
            std::cout << ", Owner: " << owner->getFullName();
        } else {
            std::cout << ", Owner ID: " << account.getCustomerId();
        }

        std::cout << "\n";
    }
}

void ConsoleUI::showFilteredTransactionHistory() const {
    std::string accountNumber;
    std::cout << "Account number: ";
    std::cin >> accountNumber;

    if (!InputValidator::isValidAccountNumber(accountNumber)) {
        std::cout << "Invalid account number format\n";
        return;
    }

    std::cout << "Filter by type (DEPOSIT / WITHDRAW / TRANSFER_IN / TRANSFER_OUT / ALL) ";
    std::string type;
    std::cin >> type;

    std::vector<Transaction> transactions;

    if (type == "ALL") {
        transactions = bankService.getTransactionsForAccount(accountNumber);
    } else if (type == "DEPOSIT") {
        transactions = bankService.getTransactionsForAccountByType(accountNumber, TransactionType::Deposit);
    } else if (type == "WITHDRAW") {
        transactions = bankService.getTransactionsForAccountByType(accountNumber, TransactionType::Withdrawal);
    } else if (type == "TRANSFER_IN") {
        transactions = bankService.getTransactionsForAccountByType(accountNumber, TransactionType::TransferIn);
    } else if (type == "TRANSFER_OUT") {
        transactions = bankService.getTransactionsForAccountByType(accountNumber, TransactionType::TransferOut);
    } else {
        std::cout << "Invalid transaction type.\n";
        return;
    }

    if (transactions.empty()) {
        std::cout << "No transactions fount\n";
        return;
    }

    std::cout << "\nTransaction history:\n";
    for (const Transaction& transaction : transactions) {
        std::cout << " - ID: " << transaction.getId() << ", Type: " << transaction.getTypeAsString() << ", Amount: " << transaction.getAmount() << ", Description: " << transaction.getDescription() << ", Time: " << transaction.getFormattedTimestamp() << "\n";
    }
}