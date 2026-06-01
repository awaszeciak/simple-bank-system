#include <iostream>
#include "../include/domain/Customer.h"
#include "../include/domain/Account.h"
#include "../include/domain/Transaction.h"
#include "../include/repository/CustomerRepository.h"
#include "../include/repository/AccountRepository.h"

int main()
{
    AccountRepository accountRepository;

    Account account1("PL61109010140000071219812874", 1, 500.0);
    Account account2("PL27114020040000300201355387", 1, 1200.0);
    Account account3("PL05102055581111111111111111", 2, 300.0);

    accountRepository.addAccount(account1);
    accountRepository.addAccount(account2);
    accountRepository.addAccount(account3);

    std::cout << "All accounts:" << std::endl;

    for (const Account& account : accountRepository.getAllAccounts()) {
        std::cout << "- Account number: " << account.getAccountNumber()
                  << ", Customer ID: " << account.getCustomerId()
                  << ", Balance: " << account.getBalance() << " PLN"
                  << std::endl;
    }

    std::cout << "\nAccounts owned by customer with ID 1:" << std::endl;

    for (const Account& account : accountRepository.findByOwnerId(1)) {
        std::cout << "- " << account.getAccountNumber()
                  << ", Balance: " << account.getBalance() << " PLN"
                  << std::endl;
    }

    std::cout << "\nSearching for account:" << std::endl;

    std::optional<Account> foundAccount =
        accountRepository.findByAccountNumber("PL61109010140000071219812874");

    if (foundAccount.has_value()) {
        std::cout << "Found account: " << foundAccount->getAccountNumber()
                  << ", Balance: " << foundAccount->getBalance() << " PLN"
                  << std::endl;
    }
    else {
        std::cout << "Account not found." << std::endl;
    }

    return 0;
}