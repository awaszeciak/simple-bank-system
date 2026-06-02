#include <cassert>
#include <iostream>

#include "../include/service/BankService.h"
#include "../include/repository/CustomerRepository.h"
#include "../include/repository/AccountRepository.h"
#include "../include/repository/TransactionRepository.h"

struct TestContext {
    CustomerRepository customerRepository;
    AccountRepository accountRepository;
    TransactionRepository transactionRepository;

    BankService bankService;

    TestContext()
        : bankService(customerRepository, accountRepository, transactionRepository) {}
};

void testCreateCustomer() {
    TestContext ctx;

    Customer customer = ctx.bankService.createCustomer("Jan", "Kowalski", "jankowalski@example.com");

    assert(customer.getId() == 1);
    assert(customer.getFirstName() == "Jan");
    assert(customer.getLastName() == "Kowalski");
    assert(customer.getEmail() == "jankowalski@example.com");
}


void testCreateCustomersHaveUniqueIds() {
    TestContext ctx;

    Customer first = ctx.bankService.createCustomer("Jan", "Kowalski", "jankowalski@example.com");
    Customer second = ctx.bankService.createCustomer("Adam", "Nowak", "adamnowak@example.com");

    assert(first.getId() == 1);
    assert(second.getId() == 2);
}

void testDuplicateEmailThrowsException() {
    TestContext ctx;

    ctx.bankService.createCustomer("Jan", "Kowalski", "jankowalski@example.com");

    bool exceptionThrown = false;

    try {
        ctx.bankService.createCustomer("Adam", "Nowak", "jankowalski@example.com");
    } catch (const std::invalid_argument&) {
        exceptionThrown = true;
    }

    assert(exceptionThrown);
}

void testFindCustomerById() {
    TestContext ctx;
    Customer customer = ctx.bankService.createCustomer("Jan", "Kowalski", "jankowalski@example.com");

    std::optional<Customer> foundCustomer = ctx.bankService.findCustomerById(customer.getId());

    assert(foundCustomer.has_value());
    assert(foundCustomer->getEmail() == "jankowalski@example.com");
}

void testCreateAccount() {
    TestContext ctx;

    Customer customer = ctx.bankService.createCustomer("Jan", "Kowalski", "jankowalski@example.com");

    std::optional<Account> account = ctx.bankService.createAccount(customer.getId(), 500.0);

    assert(account.has_value());
    assert(account->getCustomerId() == customer.getId());
    assert(account->getBalance() == 500.0);
    assert(account->getAccountNumber().size() == 28);
    assert(account->getAccountNumber().substr(0, 2) == "PL");
}

void testCreateAccountForMissingCustomerReturnsNullopt() {
    TestContext ctx;

    std::optional<Account> account = ctx.bankService.createAccount(999, 500.0);

    assert(!account.has_value());

}

void testCreateAccountWithNegativeInitialBalanceReturnsNullopt() {
    TestContext ctx;

    Customer customer = ctx.bankService.createCustomer("Jan", "Kowalski", "jankowalski@example.com");

    std::optional<Account> account = ctx.bankService.createAccount(customer.getId(), -100);

    assert(!account.has_value());
}

void testDeposit() {
    TestContext ctx;
    
    Customer customer = ctx.bankService.createCustomer("Jan", "Kowalski", "jankowalski@example.com");
    std::optional<Account> account = ctx.bankService.createAccount(customer.getId(), 100.0);

    bool result = ctx.bankService.deposit(account->getAccountNumber(), 50);

    std::optional<Account> updatedAccount = ctx.bankService.findAccountByNumber(account->getAccountNumber());
    std::vector<Transaction> transactions = ctx.bankService.getTransactionsForAccount(account->getAccountNumber());

    assert(result == true);
    assert(updatedAccount.has_value());
    assert(updatedAccount->getBalance() == 150.0);
    assert(transactions[0].getType() == TransactionType::Deposit);
    assert(transactions[0].getAmount() == 50.0);
}

void testDepositToMissingAccountReturnsFalse() {
    TestContext ctx;

    bool result = ctx.bankService.deposit("PL0000000000000000000000000000", 50.0);

    assert(!result);
}

void testDepositNegativeAmountThrowsException() {
    TestContext ctx;

    Customer customer = ctx.bankService.createCustomer("Jan", "Kowalski", "jankowalski@example.com");
    std::optional<Account> account = ctx.bankService.createAccount(customer.getId(), 100.0);

    bool exceptionThrown = false;

    try {
        ctx.bankService.deposit(account->getAccountNumber(), -50.0);
    } catch (const std::invalid_argument&) {
        exceptionThrown = true;
    }

    assert(exceptionThrown);
}

void testWithdraw() {
    TestContext ctx;
    
    Customer customer = ctx.bankService.createCustomer("Jan", "Kowalski", "jankowalski@example.com");
    std::optional<Account> account = ctx.bankService.createAccount(customer.getId(), 100.0);

    bool result = ctx.bankService.withdraw(account->getAccountNumber(), 40.0);

    std::optional<Account> updatedAccount = ctx.bankService.findAccountByNumber(account->getAccountNumber());
    std::vector<Transaction> transactions = ctx.bankService.getTransactionsForAccount(account->getAccountNumber());

    assert(result == true);
    assert(updatedAccount.has_value());
    assert(updatedAccount->getBalance() == 60.0);
    assert(transactions.size() == 1);
    assert(transactions[0].getType() == TransactionType::Withdrawal);
}

void testWithdrawTooMuchMoney() {
    TestContext ctx;

    Customer customer = ctx.bankService.createCustomer("Jan", "Kowalski", "jankowalski@example.com");
    std::optional<Account> account = ctx.bankService.createAccount(customer.getId(), 100.0);

    bool result = ctx.bankService.withdraw(account->getAccountNumber(), 200.0);

    std::optional<Account> updatedAccount = ctx.bankService.findAccountByNumber(account->getAccountNumber());
    std::vector<Transaction> transactions = ctx.bankService.getTransactionsForAccount(account->getAccountNumber());

    assert(result == false);
    assert(updatedAccount.has_value());
    assert(updatedAccount->getBalance() == 100.0);
    assert(transactions.empty());
}

void testWithdrawFromMissingAccountReturnsFalse() {
    TestContext ctx;

    bool result = ctx.bankService.withdraw("PL00000000000000000000000000", 20.0);

    assert(!result);
}

void testWithdrawNegativeAmountThrowsException() {
    TestContext ctx;

    Customer customer = ctx.bankService.createCustomer("Jan", "Kowalski", "jankowalski@example.com");
    std::optional<Account> account = ctx.bankService.createAccount(customer.getId(), 100);

    bool exceptionThrown = false;

    try {
        ctx.bankService.withdraw(account->getAccountNumber(), -10.0);
    } catch (const std::invalid_argument&) {
        exceptionThrown = true;
    }

    assert(exceptionThrown);
}


void testTransfer() {
    TestContext ctx;

    Customer c1 = ctx.bankService.createCustomer("Jan", "Kowalski", "jankowalski@example.com");
    Customer c2 = ctx.bankService.createCustomer("Adam", "Nowak", "adamnowak@example.com");

    std::optional<Account> ac1 = ctx.bankService.createAccount(c1.getId(), 1000.0);    
    std::optional<Account> ac2 = ctx.bankService.createAccount(c2.getId(), 100.0);   

    bool result = ctx.bankService.transfer(ac1->getAccountNumber(), ac2->getAccountNumber(), 300.0);

    std::optional<Account> updatedAc1 = ctx.bankService.findAccountByNumber(ac1->getAccountNumber());
    std::optional<Account> updatedAc2 = ctx.bankService.findAccountByNumber(ac2->getAccountNumber());
    std::vector<Transaction> ATransactions1 = ctx.bankService.getTransactionsForAccount(ac1->getAccountNumber());
    std::vector<Transaction> ATransactions2 = ctx.bankService.getTransactionsForAccount(ac2->getAccountNumber());
    

    assert(result);
    assert(updatedAc1->getBalance() == 700.0);
    assert(updatedAc2->getBalance() == 400.0);
    assert(ATransactions1.size() == 1);
    assert(ATransactions2.size() == 1);
    assert(ATransactions1[0].getType() == TransactionType::TransferOut);
    assert(ATransactions2[0].getType() == TransactionType::TransferIn);
    

}


void testTransactionHistory() {
    TestContext ctx;

    Customer customer = ctx.bankService.createCustomer("Jan", "Kowalski", "jankowalski@example.com");

    auto account = ctx.bankService.createAccount(customer.getId(), 100.0);

    ctx.bankService.deposit(account->getAccountNumber(), 50.0);
    ctx.bankService.withdraw(account->getAccountNumber(), 20.0);

    std::vector<Transaction> transactions = ctx.bankService.getTransactionsForAccount(account->getAccountNumber());

    assert(transactions.size() == 2);
    assert(transactions[0].getId() == 1);
    assert(transactions[1].getId() == 2);
}

void testTransactionHistoryByType() {
    TestContext ctx;

    Customer customer = ctx.bankService.createCustomer("Jan", "Kowalski", "jankowalski@example.com");
    std::optional<Account> account = ctx.bankService.createAccount(customer.getId(), 100.0);

    ctx.bankService.deposit(account->getAccountNumber(), 50.0);
    ctx.bankService.withdraw(account->getAccountNumber(), 20.0);
    ctx.bankService.deposit(account->getAccountNumber(), 10.0);

    std::vector<Transaction> deposits = ctx.bankService.getTransactionsForAccountByType(account->getAccountNumber(), TransactionType::Deposit);
    std::vector<Transaction> withdrawals = ctx.bankService.getTransactionsForAccountByType(account->getAccountNumber(), TransactionType::Withdrawal);

    assert(deposits.size() == 2);
    assert(withdrawals.size() == 1);
}

void testDepositNegativeAmount() {
    TestContext ctx;

    Customer customer = ctx.bankService.createCustomer("Jan", "Kowalski", "jankowalski@example.com"); 
    auto account = ctx.bankService.createAccount(customer.getId(), 100.0);

    bool exceptionThrown = false;

    try {
        ctx.bankService.deposit(account->getAccountNumber(), -50.0);
    } catch (const std::invalid_argument&) {
        exceptionThrown = true;
    }

    assert(exceptionThrown);
}

void testTransferNotEnoughMoney() {
    TestContext ctx;

    Customer c1 = ctx.bankService.createCustomer("Jan", "Kowalski", "jankowalski@example.com");
    Customer c2 = ctx.bankService.createCustomer("Adam", "Nowak", "adamnowak@example.com");

    auto a1 = ctx.bankService.createAccount(c1.getId(), 100.0);
    auto a2 = ctx.bankService.createAccount(c2.getId(), 50.0);

    bool result = ctx.bankService.transfer(a1->getAccountNumber(), a2->getAccountNumber(), 300.0);

    auto updatedA1 = ctx.bankService.findAccountByNumber(a1->getAccountNumber());
    auto updatedA2 = ctx.bankService.findAccountByNumber(a2->getAccountNumber());

    assert(result == false);
    assert(updatedA1.has_value());
    assert(updatedA2.has_value());
    assert(updatedA1->getBalance() == 100.0);
    assert(updatedA2->getBalance() == 50.0);
}

void testTransferToSameAccount() {
    TestContext ctx;

    Customer customer = ctx.bankService.createCustomer("Jan", "Kowalski", "jankowalski@example.com");
  
    auto account = ctx.bankService.createAccount(customer.getId(), 100.0);

    bool result = ctx.bankService.transfer(account->getAccountNumber(), account->getAccountNumber(), 100.0);

    auto updatedAccount = ctx.bankService.findAccountByNumber(account->getAccountNumber());

    assert(result == false);
    assert(updatedAccount.has_value());
    assert(updatedAccount->getBalance() == 100.0);

}

void testTransferWithMissingTargetAccountReturnsFalse() {
    TestContext ctx;

    Customer customer = ctx.bankService.createCustomer("Jan", "Kowalski", "jankowalski@example.com");
    std::optional<Account> sourceAccount = ctx.bankService.createAccount(customer.getId(), 100.0);

    bool result = ctx.bankService.transfer(sourceAccount->getAccountNumber(), "PL00000000000000000000000000", 50.0);

    std::optional<Account> updatedSourceAccount = ctx.bankService.findAccountByNumber(sourceAccount->getAccountNumber());

    assert(!result);
    assert(updatedSourceAccount->getBalance() == 100.0);
}

void testTransferNegativeAmountThrowsException() {
    TestContext ctx;

    Customer firstCustomer = ctx.bankService.createCustomer("Jan", "Kowalski", "jankowalski@example.com");
    Customer secondCustomer = ctx.bankService.createCustomer("Adam", "Nowak", "adamnowak@example.com");


    std::optional<Account> sourceAccount = ctx.bankService.createAccount(firstCustomer.getId(), 100.0);
    std::optional<Account> targetAccount = ctx.bankService.createAccount(secondCustomer.getId(), 100.0);

    bool exceptionThrown = false;

    try {
        ctx.bankService.transfer(sourceAccount->getAccountNumber(), targetAccount->getAccountNumber(), -10.0);
    } catch (const std::invalid_argument&) {
        exceptionThrown = true;
    }
    assert(exceptionThrown);

}

int main() {
    testCreateCustomer();
    testCreateCustomersHaveUniqueIds();
    testDuplicateEmailThrowsException();
    testFindCustomerById();

    testCreateAccount();
    testCreateAccountForMissingCustomerReturnsNullopt();
    testCreateAccountWithNegativeInitialBalanceReturnsNullopt();

    testDeposit();
    testDepositToMissingAccountReturnsFalse();
    testDepositNegativeAmountThrowsException();

    testWithdraw();
    testWithdrawTooMuchMoney();
    testWithdrawFromMissingAccountReturnsFalse();
    testWithdrawNegativeAmountThrowsException();

    testTransfer();
    testTransferNotEnoughMoney();
    testTransferToSameAccount();
    testTransferWithMissingTargetAccountReturnsFalse();
    testTransferNegativeAmountThrowsException();

    testTransactionHistory();
    testTransactionHistoryByType();


    std::cout << "All BankService tests passed.\n";

    return 0;

    std::cout << "All BankService tests passed.\n";

    return 0;
}