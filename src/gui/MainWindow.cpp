#include "MainWindow.h"

#include <QFormLayout>
#include <QGroupBox>
#include <QLineEdit>
#include <QMessageBox>
#include <QPushButton>
#include <QTextEdit>
#include <QLabel>
#include <QComboBox>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QWidget>
#include <QRegularExpression>

#include <stdexcept>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), bankService(customerRepository, accountRepository, transactionRepository),
    firstNameInput(new QLineEdit()), lastNameInput(new QLineEdit()), initialBalanceInput(new QLineEdit()),
    emailInput(new QLineEdit()), customerComboBox(new QComboBox()),
    sourceAccountComboBox(new QComboBox()), targetAccountComboBox(new QComboBox()),
    transferAmountInput(new QLineEdit()), accountComboBox(new QComboBox()), 
    operationAmountInput(new QLineEdit()), outputArea(new QTextEdit()), 
    statusLabel(new QLabel("Ready.")) {



    setWindowTitle("Simple Bank System");
    resize(1050, 750);

    QWidget *centralWidget = new QWidget(this);
    QVBoxLayout *mainLayout = new QVBoxLayout(centralWidget);

    QLabel *titleLabel = new QLabel("Simple Bank System");
    titleLabel->setStyleSheet("font-size: 24px; font-weight: bold;");

//CREATE CUSTOMER SECTION

    QGroupBox *customerGroup = new QGroupBox("Create customer");
    QFormLayout *customerForm = new QFormLayout(customerGroup);

    firstNameInput->setPlaceholderText("Jan");
    lastNameInput->setPlaceholderText("Kowalski");
    emailInput->setPlaceholderText("jankowalski@example.com");

    QPushButton *createCustomerButton = new QPushButton("Create customer");

    customerForm->addRow("First name:", firstNameInput);
    customerForm->addRow("Last name:", lastNameInput);
    customerForm->addRow("Email:", emailInput);
    customerForm->addRow(createCustomerButton);

//CREATE ACCOUNT SECTION

    QGroupBox *accountGroup = new QGroupBox("Create account");
    QFormLayout *accountForm = new QFormLayout(accountGroup);

    customerComboBox->addItem("No customers available", -1);
    initialBalanceInput->setPlaceholderText("500.00");

    QPushButton *createAccountButton = new QPushButton("Create account");
    
    accountForm->addRow("Customer:", customerComboBox);
    accountForm->addRow("Initial balance:", initialBalanceInput);
    accountForm->addRow(createAccountButton);

//ACCOUNT OPERATIONS SECTION

QGroupBox *operationsGroup = new QGroupBox("Account operations");
QFormLayout *operationsForm = new QFormLayout(operationsGroup);

accountComboBox->addItem("No accounts available", "");

operationAmountInput->setPlaceholderText("100.00");

QPushButton *depositButton = new QPushButton("Deposit");
QPushButton *withdrawButton = new QPushButton("Withdraw");

QHBoxLayout *operationButtonsLayout = new QHBoxLayout();
operationButtonsLayout->addWidget(depositButton);
operationButtonsLayout->addWidget(withdrawButton);

operationsForm->addRow("Account:", accountComboBox);
operationsForm->addRow("Amount:", operationAmountInput);
operationsForm->addRow(operationButtonsLayout);

QGroupBox *transferGroup = new QGroupBox("Transfer");
QFormLayout *transferForm = new QFormLayout(transferGroup);

sourceAccountComboBox->addItem("No accounts available", "");
targetAccountComboBox->addItem("No accounts available", "");

transferAmountInput->setPlaceholderText("100.00");

QPushButton *transferButton = new QPushButton("Transfer");

transferForm->addRow("From account:", sourceAccountComboBox);
transferForm->addRow("To account:", targetAccountComboBox);
transferForm->addRow("Amount:", transferAmountInput);
transferForm->addRow(transferButton);

//OUTPUT SECTION

    outputArea->setReadOnly(true);
    outputArea->setPlaceholderText("Operations will be displayed here...");

    statusLabel->setStyleSheet("font-size: 14px;");

    mainLayout->addWidget(titleLabel);
    mainLayout->addWidget(customerGroup);
    mainLayout->addWidget(accountGroup);
    mainLayout->addWidget(operationsGroup);
    mainLayout->addWidget(transferGroup);
    mainLayout->addWidget(new QLabel("Output:"));
    mainLayout->addWidget(outputArea);
    mainLayout->addWidget(statusLabel);

    setCentralWidget(centralWidget);

    connect(createCustomerButton, &QPushButton::clicked, [this](){
        createCustomer();
    });

    connect(createAccountButton, &QPushButton::clicked, [this]() {
        createAccount();
    });

    connect(depositButton, &QPushButton::clicked, [this]() {
        deposit();
    });

    connect(withdrawButton, &QPushButton::clicked, [this]() {
        withdraw();
    });

    connect(transferButton, &QPushButton::clicked, [this]() {
        transfer();
    });
}

void MainWindow::createCustomer() {
    QString firstNameText = firstNameInput->text().trimmed();
    QString lastNameText = lastNameInput->text().trimmed();
    QString emailText = emailInput->text().trimmed();

    if (firstNameText.isEmpty() || lastNameText.isEmpty() || emailText.isEmpty()) {
        QMessageBox::warning(this, "Validation error", "Please fill in all customer fields.");
        statusLabel->setText("Missing customer data.");
        return;
    }

    QRegularExpression nameRegex("^[A-Za-zĄĆĘŁŃÓŚŹŻąćęłńóśźż@. -]+$");

    if (!nameRegex.match(firstNameText).hasMatch()) {
        QMessageBox::warning(this, "Validation error", "First name can contain only letters.");
        statusLabel->setText("Invalid first name.");
        return;
    }

    if (!nameRegex.match(lastNameText).hasMatch()) {
        QMessageBox::warning(this, "Validation error", "Last name can contain only letters.");
        statusLabel->setText("Invalid last name.");
        return;
    }

    if (!nameRegex.match(emailText).hasMatch()) {
        QMessageBox::warning(this, "Validation error", "Please enter a valid email address.");
        statusLabel->setText("Invalid email.");
        return;
    }

    std::string firstName = firstNameText.toStdString();
    std::string lastName = lastNameText.toStdString();
    std::string email = emailText.toStdString();

    try {
        Customer customer = bankService.createCustomer(firstName, lastName, email);

        if (customerComboBox->count() == 1 && customerComboBox->itemData(0).toInt() == -1) {
            customerComboBox->clear();
        }

        QString customerText = QString("%1 %2 (ID: %3)").arg(QString::fromStdString(customer.getFirstName())).arg(QString::fromStdString(customer.getLastName()))
        .arg(customer.getId());

        customerComboBox->addItem(customerText, customer.getId());

        
        QString message = QString("Created customer: ID=%1, name=%2 %3, email=%4").arg(customer.getId())
        .arg(QString::fromStdString(customer.getFirstName())).arg(QString::fromStdString(customer.getLastName()))
        .arg(QString::fromStdString(customer.getEmail()));

        outputArea->append(message);
        statusLabel->setText("Customer created successfully.");

        firstNameInput->clear();
        lastNameInput->clear();
        emailInput->clear();

    } catch (const std::invalid_argument& error) {
        QMessageBox::warning(this, "Error", error.what());
        statusLabel->setText("Could not create customer.");
    }
}

void MainWindow::createAccount() {
    int customerId = customerComboBox->currentData().toInt();

    if (customerId == -1) {
        QMessageBox::warning(this, "Validation error", "Create a customer before creating an account.");
        statusLabel->setText("No customer selected.");
        return;
    }

    bool balanceOk = false;
    double initialBalance = initialBalanceInput->text().toDouble(&balanceOk);

    if (!balanceOk || initialBalance < 0) {
        QMessageBox::warning(this, "Validation error", "Initial balance must be a valid number.");
        statusLabel->setText("Invalid account data.");
        return;
    }

    std::optional<Account> account = bankService.createAccount(customerId, initialBalance);

    if (!account.has_value()) {
        QMessageBox::warning(this, "Error", "Could not create account. Chceck selected customer or initial balance.");
        statusLabel->setText("Could not create account.");
        return;
    }

    addAccountToAllComboBoxes(*account);


    QString message = QString("Created account: number=%1, customer ID=%2, balance=%3 PLN").arg(QString::fromStdString(account->getAccountNumber()))
    .arg(account->getCustomerId()).arg(account->getBalance());

    outputArea->append(message);
    statusLabel->setText("Account created successfully.");

    initialBalanceInput->clear();
}


void MainWindow::deposit() {
    QString selectedAccountNumber = accountComboBox->currentData().toString();

    if (selectedAccountNumber.isEmpty()) {
        QMessageBox::warning(this, "Validation error", "Create an account before making operations.");
        statusLabel->setText("No account selected.");
        return;
    }

    bool amountOk = false;
    double amount = operationAmountInput->text().toDouble(&amountOk);

    if (!amountOk || amount <= 0) {
        QMessageBox::warning(this, "Validation error", "Amount must be a valid number");
        statusLabel->setText("Invalid amount.");
        return;
    }

    try {
        bool result = bankService.deposit(selectedAccountNumber.toStdString(), amount);

        if (!result) {
            QMessageBox::warning(this, "Error", "Deposit failed. Check selected account.");
            statusLabel->setText("Deposit failed.");
            return;
        }

        std::optional<Account> updatedAccount = bankService.findAccountByNumber(selectedAccountNumber.toStdString());

        if (updatedAccount.has_value()) {
            updateAccountInAllComboBoxes(*updatedAccount);
        }


        QString message = QString("Deposit: account=%1, amount=%2, new balance=%3 PLN").arg(selectedAccountNumber)
        .arg(amount).arg(updatedAccount->getBalance());

        outputArea->append(message);
        statusLabel->setText("Deposit completed successfully.");

        operationAmountInput->clear();

    } catch (const std::invalid_argument &error) {
        QMessageBox::warning(this, "Error", error.what());
        statusLabel->setText("Deposit failed.");
    }
}


void MainWindow::withdraw() {
    QString selectedAccountNumber = accountComboBox->currentData().toString();

    if (selectedAccountNumber.isEmpty()) {
        QMessageBox::warning(this, "Validation error", "Create an account before making operations.");
        statusLabel->setText("No account selected.");
        return;
    }

    bool amountOk = false;
    double amount = operationAmountInput->text().toDouble(&amountOk);

    if (!amountOk || amount <= 0) {
        QMessageBox::warning(this, "Validation error", "Amount must be a valid number.");
        statusLabel->setText("Invalid amount");
        return;
    }

    try {
        bool result = bankService.withdraw(selectedAccountNumber.toStdString(), amount);

        if (!result) { 
            QMessageBox::warning(this, "Error", "Withdrawal failed. Check balance or selected account");
            statusLabel->setText("Withdrawal failed.");
            return;
        }

        std::optional<Account> updatedAccount = bankService.findAccountByNumber(selectedAccountNumber.toStdString());

        if (updatedAccount.has_value()) {
            updateAccountInAllComboBoxes(*updatedAccount);
        }


        QString message = QString("Withdrawal: account=%1, amount=%2, new balance=%3 PLN").arg(selectedAccountNumber)
        .arg(amount).arg(updatedAccount->getBalance());

        outputArea->append(message);
        statusLabel->setText("Withdrawal completed successfully.");

        operationAmountInput->clear();
    } catch (const std::invalid_argument& error) {
        QMessageBox::warning(this, "Error", error.what());
        statusLabel->setText("Withdrawal failed.");
    }
}

void MainWindow::transfer() {
    QString sourceAccountNumber = sourceAccountComboBox->currentData().toString();
    QString targetAccountNumber = targetAccountComboBox->currentData().toString();

    if (sourceAccountNumber.isEmpty() || targetAccountNumber.isEmpty()) {
        QMessageBox::warning(this, "Validation error", "Create accounts before making a transfer.");
        statusLabel->setText("No account selected.");
        return;
    }

    if (sourceAccountNumber == targetAccountNumber) {
        QMessageBox::warning(this, "Validation error", "Source and target account must be different.");
        statusLabel->setText("Invalid transfer accounts.");
        return;
    }

    bool amountOk = false;
    double amount = transferAmountInput->text().toDouble(&amountOk);

    if (!amountOk || amount <= 0) {
        QMessageBox::warning(this, "Validation error", "Transfer amount must be a positive number.");
        statusLabel->setText("Invalid transfer amount.");
        return;
    }

    try {
        bool result = bankService.transfer(sourceAccountNumber.toStdString(),
    targetAccountNumber.toStdString(), amount);

    
    if (!result) {
        QMessageBox::warning(this, "Error", "Transfer failed. Check balance or selected accounts.");
        statusLabel->setText("Transfer failed");
        return;
    }

    std::optional<Account> updatedSourceAccount = bankService.findAccountByNumber(sourceAccountNumber.toStdString());

    std::optional<Account> updatedTargetAccount = bankService.findAccountByNumber(targetAccountNumber.toStdString());

    if (!updatedSourceAccount.has_value() || !updatedTargetAccount.has_value()) {
        QMessageBox::warning(this, "Error", "Could not refresh accounts after transfer");
        statusLabel->setText("Transfer completed, but accounts could not be refreshed.");
        return;
    }

    updateAccountInAllComboBoxes(*updatedSourceAccount);
    updateAccountInAllComboBoxes(*updatedTargetAccount);

    QString message = QString("Transfer: from=%1, to=%2, amount=%3 PLN, source balance=%4 PLN, target balance=%5 PLN").arg(sourceAccountNumber)
    .arg(targetAccountNumber).arg(amount).arg(updatedSourceAccount->getBalance()).arg(updatedTargetAccount->getBalance());

    outputArea->append(message);
    statusLabel->setText("Transfer completed successfully.");

    transferAmountInput->clear();


    } catch (const std::invalid_argument& error) {
        QMessageBox::warning(this, "Error", error.what());
        statusLabel->setText("Transfer failed.");
    }
}

void MainWindow::addAccountToAllComboBoxes(const Account& account) {
    QString accountNumber = QString::fromStdString(account.getAccountNumber());

    QString accountText = QString("%1 | customer ID: %2 | balance: %3 PLN")
    .arg(accountNumber).arg(account.getCustomerId()).arg(account.getBalance());

    if (accountComboBox->count() == 1 && accountComboBox->itemData(0).toString().isEmpty()) {
        accountComboBox->clear();
    }

    if (sourceAccountComboBox->count() == 1 && sourceAccountComboBox->itemData(0).toString().isEmpty()) {
        sourceAccountComboBox->clear();
    }

    if (targetAccountComboBox->count() == 1 && targetAccountComboBox->itemData(0).toString().isEmpty()) {
        targetAccountComboBox->clear();
    }

    accountComboBox->addItem(accountText, accountNumber);
    sourceAccountComboBox->addItem(accountText, accountNumber);
    targetAccountComboBox->addItem(accountText, accountNumber);
}

void MainWindow::updateAccountInAllComboBoxes(const Account& account) {
    QString accountNumber = QString::fromStdString(account.getAccountNumber());

    QString accountText = QString("%1 | customer ID: %2 | balance: %3 PLN").arg(accountNumber)
    .arg(account.getCustomerId()).arg(account.getBalance());

    QList<QComboBox*> comboBoxes = {
        accountComboBox,
        sourceAccountComboBox,
        targetAccountComboBox
    };

    for (QComboBox *comboBox : comboBoxes) {
        for (int i = 0; i < comboBox->count(); i++) {
            if (comboBox->itemData(i).toString() == accountNumber) {
                comboBox->setItemText(i, accountText);
                comboBox->setItemData(i, accountNumber);
            }
        }
    }
}