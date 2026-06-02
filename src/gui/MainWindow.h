#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "repository/AccountRepository.h"
#include "repository/CustomerRepository.h"
#include "repository/TransactionRepository.h"
#include "service/BankService.h"

class QLabel;
class QLineEdit;
class QTextEdit;
class QComboBox;

class MainWindow : public QMainWindow {
    public:
        explicit MainWindow(QWidget *parent = nullptr);

    private:
        CustomerRepository customerRepository;
        AccountRepository accountRepository;
        TransactionRepository transactionRepository;
        BankService bankService;

        QLineEdit *firstNameInput;
        QLineEdit *lastNameInput;
        QLineEdit *emailInput;
        
        QTextEdit *outputArea;
        QLabel *statusLabel;

        QLineEdit *initialBalanceInput;
        QComboBox *customerComboBox;

        void createAccount();
        void createCustomer();
};

#endif