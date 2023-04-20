#include "TokenGenerator.h"
#include <random>

TokenGenerator::TokenGenerator(int length)
    : m_length(length)
{}

std::string TokenGenerator::generateToken() {
    std::string token;
    const std::string chars = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, chars.size() - 1);
    for (int i = 0; i < m_length; ++i) {
        token += chars[dis(gen)];
    }
    return token;
}

