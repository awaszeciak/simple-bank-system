#include <iostream>
#include "../include/domain/Customer.h"
#include "../include/domain/Account.h"
#include "../include/domain/Transaction.h"

int main() {

    Customer customer(1, "Anna", "Nowak", "annanowak@gmail.com");
    Account account("PL61109010140000071219812874", customer.getId(), 500.0);

    Transaction transaction(1, account.getAccountNumber(), "DEPOSIT", 250.0, "Initial deposit");

    std::cout << "Simple Bank System started successfully." << std::endl;


    std::cout << "Customer: " << customer.getFullName() << std::endl;
    std::cout << "Account number: " << account.getAccountNumber() << std::endl;
    std::cout << "Balance: " << account.getBalance() << " PLN" << std::endl;

    std::cout << "Transaction type: " << transaction.getType() << std::endl;
    std::cout << "Transaction amount: " << transaction.getAmount() << " PLN" << std::endl;
    return 0;
}