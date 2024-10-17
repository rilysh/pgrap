#ifndef HS_MATCH_HPP
#define HS_MATCH_HPP

#include <array>
#include <cstdint>
#include <ranges>

#include "hs_compile.h"

#include "pgrap.hpp"

struct HSMatch {
    bool found_match;
    std::int64_t how_many;

    explicit HSMatch(bool arg_found_match,
		    std::int64_t arg_how_many)
	: found_match{arg_found_match}, how_many{arg_how_many} {}

    HSMatch() = default;
};

struct HSMode {
    std::string_view mode;
    std::int32_t flag;

    explicit HSMode(const std::string_view arg_mode,
		    std::int32_t arg_flags)
	: mode{arg_mode}, flag{arg_flags} {}

    HSMode() = default;
};

static const std::array<HSMode, 11> list_flags = {
    HSMode("caseless", HS_FLAG_CASELESS),
    HSMode("dotall", HS_FLAG_DOTALL),
    HSMode("multiline", HS_FLAG_MULTILINE),
    HSMode("singlematch", HS_FLAG_SINGLEMATCH),
    HSMode("allowempty", HS_FLAG_ALLOWEMPTY),
    HSMode("utf8", HS_FLAG_UTF8),
    HSMode("ucp", HS_FLAG_UCP),
    HSMode("prefilter", HS_FLAG_PREFILTER),
    HSMode("somleftmost", HS_FLAG_SOM_LEFTMOST),
    HSMode("combination", HS_FLAG_COMBINATION),
    HSMode("quiet", HS_FLAG_QUIET),
};

static inline std::int32_t get_hs_compile_mode(const std::string_view rtype) {
    std::int32_t flags = 0;
    for (const auto &elem : std::views::split(rtype, ',')) {
	const auto elem_view = std::string_view(elem.begin(), elem.end());
	if (elem_view.empty())
	    continue;
	for (const auto &body : list_flags) {
	    if (body.mode == elem_view)
		flags |= body.flag;
	}
    }
    
    return flags;
}

std::pair<std::int64_t, std::vector<Process>> hs_regex_match(
    std::int32_t mode, const std::string &pattern);

#endif
