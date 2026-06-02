#include "MainWindow.h"

#include <QFormLayout>
#include <QGroupBox>
#include <QLineEdit>
#include <QMessageBox>
#include <QPushButton>
#include <QTextEdit>
#include <QLabel>
#include <QComboBox>
#include <QVBoxLayout>
#include <QWidget>

#include <stdexcept>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), bankService(customerRepository, accountRepository, transactionRepository),
    firstNameInput(new QLineEdit()), lastNameInput(new QLineEdit()), initialBalanceInput(new QLineEdit()),
    emailInput(new QLineEdit()), customerComboBox(new QComboBox()),
    outputArea(new QTextEdit()), statusLabel(new QLabel("Ready.")) {

    setWindowTitle("Simple Bank System");
    resize(900, 600);

    QWidget *centralWidget = new QWidget(this);
    QVBoxLayout *mainLayout = new QVBoxLayout(centralWidget);

    QLabel *titleLabel = new QLabel("Simple Bank System");
    titleLabel->setStyleSheet("font-size: 24px; font-weight: bold;");

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

    QGroupBox *accountGroup = new QGroupBox("Create account");
    QFormLayout *accountForm = new QFormLayout(accountGroup);

    customerComboBox->addItem("No customers available", -1);
    initialBalanceInput->setPlaceholderText("500.00");

    QPushButton *createAccountButton = new QPushButton("Create account");
    
    accountForm->addRow("Customer:", customerComboBox);
    accountForm->addRow("Initial balance:", initialBalanceInput);
    accountForm->addRow(createAccountButton);

    outputArea->setReadOnly(true);
    outputArea->setPlaceholderText("Operations will be displayed here...");

    statusLabel->setStyleSheet("font-size: 14px;");

    mainLayout->addWidget(titleLabel);
    mainLayout->addWidget(customerGroup);
    mainLayout->addWidget(accountGroup);
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

}

void MainWindow::createCustomer() {
    std::string firstName = firstNameInput->text().toStdString();
    std::string lastName = lastNameInput->text().toStdString();
    std::string email = emailInput->text().toStdString();

    if (firstName.empty() || lastName.empty() || email.empty()) {
        QMessageBox::warning(this, "Validation error", "Please fill in all customer fields.");
        statusLabel->setText("Missing customer data.");
        return;
    }

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

    if (!balanceOk) {
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

    QString message = QString("Created account: number=%1, customer ID=%2, balance=%3").arg(QString::fromStdString(account->getAccountNumber()))
    .arg(account->getCustomerId()).arg(account->getBalance());

    outputArea->append(message);
    statusLabel->setText("Account created successfully.");

    initialBalanceInput->clear();
}