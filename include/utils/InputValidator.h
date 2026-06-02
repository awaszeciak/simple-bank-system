#ifndef INPUT_VALIDATOR_H
#define INPUT_VALIDATOR_H

#include <string>

class InputValidator {
    public:
        static bool isValidAccountNumber(const std::string& accountNumber);
        static bool isPositiveAmount(double amount);
        static bool isValidEmail(const std::string& email);
        static double readNonNegativeNumber(const std::string &prompt);
        static std::string readTextWithoutDigits(const std::string &prompt);
};

#endif