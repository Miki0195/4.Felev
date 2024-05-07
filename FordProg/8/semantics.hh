#ifndef IMPLEMENTATION_HH
#define IMPLEMENTATION_HH

#include <map>
#include <string>

void semantic_error(int line, std::string text);

enum type {boolean, integer};

extern std::map<std::string, type> symbol_table;

#endif // IMPLEMENTATION_HH

