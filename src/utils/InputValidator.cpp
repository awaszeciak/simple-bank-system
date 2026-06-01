#include "../../include/utils/InputValidator.h"

#include <cctype>

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