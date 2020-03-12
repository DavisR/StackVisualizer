#ifndef LEXER_H
#define LEXER_H
#include <vector>
#include <string>


int findNextChar(std::string str, int i);
std::string tokenizeStr(std::string &str, char atChar);
bool checkNr(std::string str);
bool checkOP(std::string str);
int OPArithmetic(std::string &tok, std::vector<int> &Stack);


#endif // LEXER_H
