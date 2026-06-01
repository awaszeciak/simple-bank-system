#include "../../include/repository/AccountRepository.h"

void AccountRepository::addAccount(const Account& account) {
    accounts.push_back(account);
}

std::optional<Account> AccountRepository::findByAccountNumber(const std::string& accountNumber) const {
    for (const Account& account : accounts) {
        if (account.getAccountNumber() == accountNumber) {
            return account;
        }
    }
    return std::nullopt;
}

std::vector<Account> AccountRepository::getAllAccounts() const {
    return accounts;
}

std::vector<Account> AccountRepository::findByOwnerId(int customerId) const {
    std::vector<Account> result;

    for (const Account& account : accounts) {
        if (account.getCustomerId() == customerId) {
            result.push_back(account);
        }
    }
    return result;
}