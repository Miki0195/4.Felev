#include "semantics.hh"
#include "while.tab.hh"
#include <iostream>

void semantic_error(int line, std::string text) {
    std::cerr << "Line " << line << ": " << text << std::endl;
    exit(1);
}

std::map<std::string, type> symbol_table;