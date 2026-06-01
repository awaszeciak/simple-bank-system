#ifndef I_CUSTOMER_REPOSITORY_H
#define I_CUSTOMER_REPOSITORY_H

#include <vector>
#include <optional>
#include <string>
#include "../domain/Customer.h"

class ICustomerRepository {
    public:
        virtual ~ICustomerRepository() = default;
        virtual bool addCustomer(const Customer& customer) = 0;
        virtual std::optional<Customer> findById(int id) const = 0;
        virtual std::optional<Customer> findByEmail(const std::string& email) const = 0;
        virtual std::vector<Customer> getAllCustomers() const = 0;
};
#endif