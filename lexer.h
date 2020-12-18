#ifndef LEXER_H
#define LEXER_H
#include <vector>
#include <string>


int findNextChar(std::string str, int i); // Get next char of stack
std::string tokenizeStr(std::string &str, char atChar); // Tokenize given string for stack
bool checkNr(std::string str); // Check if char is numerical
bool checkOP(std::string str); // Check if char is an operator
int OPArithmetic(std::string &tok, std::vector<int> &Stack); // Perform arithmetic using 2 operands and an operator


#endif // LEXER_H
