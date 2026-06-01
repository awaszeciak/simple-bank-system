#include "../../include/domain/Customer.h"

Customer::Customer(int id, const std::string& firstName, const std::string& lastName, const std::string& email)
    : id(id), firstName(firstName), lastName(lastName), email(email) {}

int Customer::getId() const {
    return id;
}

std::string Customer::getFirstName() const {
    return firstName;
}

std::string Customer::getLastName() const {
    return lastName;
}

std::string Customer::getEmail() const {
    return email;
}

std::string Customer::getFullName() const {
    return firstName + " " + lastName;
}