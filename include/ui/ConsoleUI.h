#ifndef CONSOLE_UI_H
#define CONSOLE_UI_H

#include "../service/BankService.h"

class ConsoleUI {
    private:
        BankService& bankService;

        void showMenu() const;

        void createCustomer();
        void createAccount();
        void depositMoney();
        void withdrawMoney();
        void transferMoney();
        void showAccountInfo() const;
        void showTransactionHistory() const;
        void showAllCustomers() const;
        void showAllAccounts() const;

    public:
        explicit ConsoleUI(BankService& bankService);

        void run();
};

#endif