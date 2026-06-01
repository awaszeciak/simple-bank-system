#ifndef ACCOUNT_REPOSITORY_H
#define ACCOUNT_REPOSITORY_H

#include <vector>
#include <optional>
#include <string>

#include "../domain/Account.h"
#include "IAccountRepository.h"

class AccountRepository : public IAccountRepository {
    private:
        std::vector<Account> accounts;

    public:
        void addAccount(const Account& account);

        std::optional<Account> findByAccountNumber(const std::string& accountNumber) const;

        std::vector<Account> getAllAccounts() const;

        std::vector<Account> findByOwnerId(int customerId) const;

        bool updateAccount(const Account& updatedAccount);
};



#endif