#ifndef TOKENGENERATOR_H
#define TOKENGENERATOR_H

#include <string>

class TokenGenerator {
public:
    TokenGenerator(int length);
    std::string generateToken();

private:
    int m_length;
};

#endif // TOKENGENERATOR_H

