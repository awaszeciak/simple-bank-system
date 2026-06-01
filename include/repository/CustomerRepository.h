#ifndef CUSTOMER_REPOSITORY_H
#define CUSTOMER_REPOSITORY_H

#include <vector>
#include <optional>
#include "../domain/Customer.h"

class CustomerRepository {
    private:
        std::vector<Customer> customers;
        
    public:
        bool addCustomer(const Customer& customer);

        std::optional<Customer> findById(int id) const;

        std::vector<Customer> getAllCustomers() const;
};

#endif