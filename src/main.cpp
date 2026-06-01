#include <iostream>
#include "../include/domain/Customer.h"

int main() {
    
    std::cout << "Simple Bank System started successfully." << std::endl;

    Customer customer(1, "Anna", "Nowak", "annanowak@gmail.com");
    std::cout << "Customer: " << customer.getFullName() << "\n";
    std::cout << "Email: " << customer.getEmail() << "\n"; 

    return 0;
}