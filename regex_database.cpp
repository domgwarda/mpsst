#include "regex_database.h"
#include "abstract_regex_handler.h"
#include <iostream>
#include <variant>

using namespace std;

void test_handler(AbstractRegexHandler* handler) {
    RegexDatabase db = handler->get_database();

    if (auto hs_db = std::get_if<hs_database_t*>(&db)) {
    } else {
    std::cout << "Unknown DB type\n";
}
}