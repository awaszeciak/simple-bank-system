#include <iostream>

#include "../include/repository/CustomerRepository.h"
#include "../include/repository/AccountRepository.h"
#include "../include/repository/TransactionRepository.h"

#include "../include/service/BankService.h"

#include "../include/ui/ConsoleUI.h"

int main() {
    
    CustomerRepository customerRepository;
    AccountRepository accountRepository;
    TransactionRepository transactionRepository;

    BankService bankService(customerRepository, accountRepository, transactionRepository);

    ConsoleUI consoleUI(bankService);
    consoleUI.run();
    
    return 0;
}