#include <iostream>
#include "../include/domain/Customer.h"
#include "../include/domain/Account.h"


int main() {
    
    std::cout << "Simple Bank System started successfully." << std::endl;

    Customer customer(1, "Anna", "Nowak", "annanowak@gmail.com");
    Account account("PL61109010140000071219812874", customer.getId(), 500.0);

    std::cout << "Customer: " << customer.getFullName() << "\n";
    std::cout << "Account number: " << account.getAccountNumber() << std::endl;
    std::cout << "Email: " << customer.getEmail() << "\n"; 
    std::cout << "Initial balance: " << account.getBalance() << "\n";

    account.deposit(250.0);
    std::cout << "Balance after deposit: " << account.getBalance() << std::endl;

    bool result = account.withdraw(1000.0);

    if (!result) {
        std::cout << "Withdrawal rejected: insufficient funds" << std::endl;
    }

    std::cout << "Final balance: " << account.getBalance() << std::endl;

    return 0;
}