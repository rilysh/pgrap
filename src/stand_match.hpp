#ifndef STAND_MATCH_HPP
#define STAND_MATCH_HPP

#include <array>
#include <cstdint>
#include <regex>

#include "pgrap.hpp"

struct StandRegexType {
    const std::string rtype;
    std::regex_constants::syntax_option_type syntax;

    explicit StandRegexType(const std::string_view arg_rtype,
		   std::regex_constants::syntax_option_type arg_syntax)
	: rtype{arg_rtype}, syntax{arg_syntax} {}

    StandRegexType() = default;
};

// Type of Regex standards supported by C++'s regex standard library.
static const std::array<StandRegexType, 12> stand_rtypes = {
    StandRegexType("ecma", std::regex_constants::ECMAScript),
    StandRegexType("basic", std::regex_constants::basic),
    StandRegexType("extended", std::regex_constants::extended),
    StandRegexType("awk", std::regex_constants::awk),
    StandRegexType("grep", std::regex_constants::grep),
    StandRegexType("egrep", std::regex_constants::egrep),
    StandRegexType("polynomial", std::regex_constants::__polynomial),
    StandRegexType("multiline", std::regex_constants::multiline),
    StandRegexType("icase", std::regex_constants::icase),
    StandRegexType("nosubs", std::regex_constants::nosubs),
    StandRegexType("collate", std::regex_constants::collate),
    StandRegexType("optimize", std::regex_constants::optimize),
};

std::pair<std::int64_t, std::vector<Process>> stand_regex_match(
    const std::string &pattern, std::regex_constants::syntax_option_type);

#endif
