#include <iostream>
#include "../include/domain/Customer.h"
#include "../include/domain/Account.h"
#include "../include/domain/Transaction.h"
#include "../include/repository/CustomerRepository.h"

int main() {
    
    CustomerRepository customerRepository;

    Customer customer1(1, "Anna", "Nowak", "annanowak@example.com");
    Customer customer2(2, "Jan", "Kowalski", "jankowalski@example.com");

    customerRepository.addCustomer(customer1);
    customerRepository.addCustomer(customer2);

    std::optional<Customer> foundCustomer = customerRepository.findById(1);

    if (foundCustomer.has_value()) {
        std::cout << "Found customer: " << foundCustomer->getFullName() << std::endl;
    }
    else {
        std::cout << "Customer not found" << std::endl;
    }

    std::cout << "All customers:" << std::endl;

    for (const Customer& customer : customerRepository.getAllCustomers()) {
        std::cout << " - " << customer.getFullName() << std::endl;
    }

    return 0;
}