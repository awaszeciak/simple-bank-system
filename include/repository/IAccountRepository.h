#ifndef I_ACCOUNT_REPOSITORY_H
#define I_ACCOUNT_REPOSITORY_H

#include <vector>
#include <optional>
#include <string>
#include "../domain/Account.h"

class IAccountRepository {
    public:
        virtual ~IAccountRepository() = default;
        virtual void addAccount(const Account& account) = 0;
        virtual std::optional<Account> findByAccountNumber(const std::string& accountNumber) const = 0;
        virtual std::vector<Account> getAllAccounts() const = 0;
        virtual std::vector<Account> findByOwnerId(int customerId) const = 0;
        virtual bool updateAccount(const Account& updatedAccount) = 0;
};

#endif