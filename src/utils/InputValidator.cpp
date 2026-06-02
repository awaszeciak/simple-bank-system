#include "../../include/utils/InputValidator.h"

#include <cctype>
#include <iostream>
#include <string>
#include <stdexcept>

bool InputValidator::isValidAccountNumber(const std::string& accountNumber) {
    if (accountNumber.length() != 28) {
        return false;
    }

    if (accountNumber[0] != 'P' || accountNumber[1] != 'L') {
        return false;
    }

    for (int i = 2; i < 28; i++) {
        if (!std::isdigit(static_cast<unsigned char>(accountNumber[i]))) {
            return false;
        }
    }
    return true;
}

bool InputValidator::isPositiveAmount(double amount) {
    return amount > 0;
}

bool InputValidator::isValidEmail(const std::string& email) {
    return !email.empty() && email.find('@') != std::string::npos;
}

double InputValidator::readNonNegativeNumber(const std::string &prompt) {
    std::string input;

    while (true) {
        std::cout << prompt;
        std::cin >> input;

        try {
            size_t position;
            double amount = std::stod(input, &position);

            if (position == input.length() && amount >= 0) {
                return amount;
            }

            std::cout << "Invalid value. Please enter a positive number.\n";
        } catch (const std::invalid_argument&) {
            std::cout << "Invalid value. Please enter a positive number.\n";
        } catch (const std::out_of_range&) {
            std::cout << "Invalid value. Please enter a positive number.\n";
        }
    }
}

std::string InputValidator::readTextWithoutDigits(const std::string &prompt) {
    std::string input;

    while (true) {
        std::cout << prompt;
        std::cin >> input;

        bool hasDigit = false;

        for (char ch : input) {
            if (std::isdigit(static_cast<unsigned char>(ch))) {
                hasDigit = true;
                break;
            }
        }

        if (!input.empty() && !hasDigit) {
            return input;
        }

        std::cout << "Invalid input. Text cannot contain digits.\n";
    }
}