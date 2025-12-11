#include "regex_database.h"
#include "abstract_regex_handler.h"
#include <iostream>
#include <variant>
#include <vector>


using namespace std;

void test_handler(AbstractRegexHandler* handler) {
    RegexDatabase db = handler->get_database();

    if (auto hs_db = get_if<hs_database_t*>(&db)) {
    } else {
    cout << "Unknown DB type\n";
}
}