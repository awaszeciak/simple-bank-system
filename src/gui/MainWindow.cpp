#include "MainWindow.h"

#include <QFormLayout>
#include <QGroupBox>
#include <QLineEdit>
#include <QMessageBox>
#include <QPushButton>
#include <QTextEdit>
#include <QLabel>
#include <QVBoxLayout>
#include <QWidget>

#include <stdexcept>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), bankService(customerRepository, accountRepository, transactionRepository),
    firstNameInput(new QLineEdit()), lastNameInput(new QLineEdit()), emailInput(new QLineEdit()),
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

    outputArea->setReadOnly(true);
    outputArea->setPlaceholderText("Operations will be displayed here...");

    statusLabel->setStyleSheet("font-size: 14px;");

    mainLayout->addWidget(titleLabel);
    mainLayout->addWidget(customerGroup);
    mainLayout->addWidget(new QLabel("Output:"));
    mainLayout->addWidget(outputArea);
    mainLayout->addWidget(statusLabel);

    setCentralWidget(centralWidget);

    connect(createCustomerButton, &QPushButton::clicked, [this](){
        createCustomer();
    });

}

void MainWindow::createCustomer() {
    std::string firstName = firstNameInput->text().toStdString();
    std::string lastName = lastNameInput->text().toStdString();
    std::string email = emailInput->text().toStdString();

    if (firstName.empty() || lastName.empty() || email.empty()) {
        QMessageBox::warning(this, "Validation error", "Please fill in all customer fields.");
        return;
    }

    try {
        Customer customer = bankService.createCustomer(firstName, lastName, email);

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