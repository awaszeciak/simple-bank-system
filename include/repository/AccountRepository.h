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
        void addAccount(const Account& account) override;
        std::optional<Account> findByAccountNumber(const std::string& accountNumber) const override;
        std::vector<Account> getAllAccounts() const override;
        std::vector<Account> findByOwnerId(int customerId) const override;
        bool updateAccount(const Account& updatedAccount) override;
};



#endif