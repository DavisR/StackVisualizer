#include "lexer.h"

// Loop through string to find the next valid char
int findNextChar(std::string str, int i) {
    for(; str.at(i) == ' ' && i < str.length(); i++) {}
    return i;
}

// Break up a string at char "atChar"
// Tokenizes a string and returns buffer, this deletes current string up until "atChar"
// Loop function until .length to get all tokens
std::string tokenizeStr(std::string &str, char atChar) {
    std::string buffer;

    int i = 0; // indexing variable
    if(str.at(0) == ' ') // Check for space as first char
        i = findNextChar(str, 0);

    for(; str[i] != atChar && i < str.length(); i++) // Loop through string and append to buffer until "atChar" has been found
        buffer += str.at(i);

    str.erase(0, i+1);    // erase string from position 0 up to length i+1(1='spacebar')
    return buffer;
}

// Check if string/token is a number
bool checkNr(std::string str) {
    int i = 0;
    if(str.at(0) == '-')
        i++; // negative number => continue

    for(; i < str.length(); i++) {
        if(!(static_cast<int>(str.at(i)) > 47 && static_cast<int>(str.at(i)) < 58))
            return false;
    }
    return true;
}

// Check if string/token is an operator
bool checkOP(std::string str) {
    int operators[] = {static_cast<int>('+'), static_cast<int>('-'), static_cast<int>('*'), static_cast<int>('/')}; // Defined ops
    for(int i = 0; i<str.length(); i++) { // Loop through string/token
        for(int g = 0; g < 4; g++) { // 4 = operator array size, look for operator match
            if(static_cast<int>(str.at(i)) == operators[g]) // Return if op has been found
                return true;
        }
    }
    return false;
}

// Perform arithmetic operations based on string/token
int OPArithmetic(std::string &tok, std::vector<int> &Stack) {
    if(Stack.size() < 2) // stack size not sufficient for operation
        return Stack.at(0);
    if(tok == "+")
        return Stack.at(Stack.size()-2) + Stack.at(Stack.size()-1);
    if(tok == "-")
        return Stack.at(Stack.size()-2) - Stack.at(Stack.size()-1);
    if(tok == "*")
        return Stack.at(Stack.size()-2) * Stack.at(Stack.size()-1);
    if(tok == "/")
        return Stack.at(Stack.size()-2) / Stack.at(Stack.size()-1);
}
