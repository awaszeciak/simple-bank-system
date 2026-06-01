#ifndef CUSTOMER_H
#define CUSTOMER_H

#include <string>

class Customer {
    private:
        int id;
        std::string firstName;
        std::string lastName;
        std::string email;
    public:
        Customer(int id, const std::string& firstName, const std::string& lastName, const std::string& email);

        int getId() const;
        std::string getFirstName() const;
        std::string getLastName() const;
        std::string getEmail() const;

        std::string getFullName() const;
};


#endif