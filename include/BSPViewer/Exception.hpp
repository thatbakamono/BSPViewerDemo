#pragma once

#include <exception>
#include <string>

class Exception : public std::exception {
private:
    std::string m_message;
public:
    explicit Exception(std::string message) : m_message(std::move(message)) {}

    [[nodiscard]] const char* what() const noexcept override {
        return m_message.c_str();
    }
};
