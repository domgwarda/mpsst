// regex_database.h

#pragma once

#include <variant>
#include <hs/hs.h>

using namespace std;

class AbstractRegexHandler;

using RegexDatabase = variant<hs_database_t*>;

void test_handler(AbstractRegexHandler* handler);