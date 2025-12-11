// regex_database.h

#pragma once

#include <variant>
#include <hs/hs.h>
#include <vector>
#define PCRE2_CODE_UNIT_WIDTH 8
#include <pcre2.h>

using namespace std;

class AbstractRegexHandler;

using RegexDatabase = variant<hs_database_t*, vector<pcre2_code*>>;

void test_handler(AbstractRegexHandler* handler);