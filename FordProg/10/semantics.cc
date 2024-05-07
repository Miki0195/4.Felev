#include "semantics.hh"
#include "while.tab.hh"
#include <iostream>
#include <sstream>

void semantic_error(int line, std::string text) {
    std::cerr << "Line " << line << ": " << text << std::endl;
    exit(1);
}

long id = 0;

std::string next_label() {
    std::stringstream ss;
    ss << "label" << id++;
    return ss.str();
}

symbol_data::symbol_data() {}

symbol_data::symbol_data(type t) : typ(t), label(next_label()) {}

std::map<std::string, symbol_data> symbol_table;

expression::expression() : typ(integer), code("") {}

expression::expression(type t, std::string c) : typ(t), code(c) {}  