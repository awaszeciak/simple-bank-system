#include "../../include/repository/CustomerRepository.h"

bool CustomerRepository::addCustomer(const Customer& customer) {
    if (findById(customer.getId()).has_value()) {
        return false;
    }

    customers.push_back(customer);
    return true;
}

std::optional<Customer> CustomerRepository::findById(int id) const {
    for (const Customer& customer : customers) {
        if (customer.getId() == id) {
            return customer;
        }
    }
    return std::nullopt;
}

std::vector<Customer> CustomerRepository::getAllCustomers() const {
    return customers;
}