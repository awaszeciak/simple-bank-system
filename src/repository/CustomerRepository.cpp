#include "../../include/repository/CustomerRepository.h"

void CustomerRepository::addCustomer(const Customer& customer) {
    customers.push_back(customer);
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