#ifndef CUSTOMER_REPOSITORY_H
#define CUSTOMER_REPOSITORY_H

#include <vector>
#include <optional>
#include "../domain/Customer.h"
#include "ICustomerRepository.h"

class CustomerRepository : public ICustomerRepository {
    private:
        std::vector<Customer> customers;
        
    public:
        bool addCustomer(const Customer& customer) override;
        std::optional<Customer> findById(int id) const override;
        std::vector<Customer> getAllCustomers() const override;
        std::optional<Customer> findByEmail(const std::string& email) const override;
};

#endif