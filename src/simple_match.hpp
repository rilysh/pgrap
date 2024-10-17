#ifndef SIMPLE_MATCH_HPP
#define SIMPLE_MATCH_HPP

#include <array>
#include <string>

#include "pgrap.hpp"

enum class SimpleMatch {
  match_all,
  match_first_lowercase_only,
  match_first_uppercase_only,
  match_first_digit_only,
  match_first_alpha_only,
  match_first_alnum_only,

  match_last_lowercase_only,
  match_last_uppercase_only,
  match_last_digit_only,
  match_last_alpha_only,
  match_last_alnum_only,

  match_all_lowercase_only,
  match_all_uppercase_only,
  match_all_digit_only,
  match_all_alpha_only,
  match_all_alnum_only,
};

struct SimpleMatchType {
    const std::string_view mtype;
    SimpleMatch smtype;

    explicit SimpleMatchType(const std::string_view arg_mtype,
			     SimpleMatch arg_smtype)
	: mtype{arg_mtype},
	  smtype{arg_smtype} {};

    SimpleMatchType() = default;
};

static const std::array<SimpleMatchType, 16> simple_match_types = {
    // Match all
    SimpleMatchType{"all", SimpleMatch::match_all},

    // Only match the first character
    SimpleMatchType{"first-lowercase-only",
		    SimpleMatch::match_first_lowercase_only},
    SimpleMatchType{"first-uppercase-only",
		    SimpleMatch::match_first_uppercase_only},
    SimpleMatchType{"first-digit-only",
		    SimpleMatch::match_first_digit_only},
    SimpleMatchType{"first-alpha-only",
		    SimpleMatch::match_first_alpha_only},
    SimpleMatchType{"first-alnum-only",
		    SimpleMatch::match_first_alnum_only},

    // Only match the last character
    SimpleMatchType{"last-lowercase-only",
		    SimpleMatch::match_last_lowercase_only},
    SimpleMatchType{"last-uppercase-only",
		    SimpleMatch::match_last_uppercase_only},
    SimpleMatchType{"last-digit-only",
		    SimpleMatch::match_last_digit_only},
    SimpleMatchType{"last-alpha-only",
		    SimpleMatch::match_last_alpha_only},
    SimpleMatchType{"last-alnum-only",
		    SimpleMatch::match_last_alnum_only},

    // Match all characters
    SimpleMatchType{"all-lowercase-only",
		    SimpleMatch::match_all_lowercase_only},
    SimpleMatchType{"all-uppercase-only",
		    SimpleMatch::match_all_uppercase_only},
    SimpleMatchType{"all-digit-only",
		    SimpleMatch::match_all_digit_only},
    SimpleMatchType{"all-alpha-only",
		    SimpleMatch::match_all_alpha_only},
    SimpleMatchType{"all-alnum-only",
		    SimpleMatch::match_all_alnum_only},		
};

std::pair<std::int64_t, std::vector<Process>> simple_match(SimpleMatch pm);
    
#endif
