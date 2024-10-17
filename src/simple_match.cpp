#include <algorithm>
#include <cctype>

#include "simple_match.hpp"
#include "pgrap.hpp"
#include "util.hpp"

static inline bool has_match_cond(SimpleMatch ctr,
				  const std::string &data) noexcept {
    switch (ctr) {
	// Match all and first characters
    case SimpleMatch::match_all:
	return true;
    case SimpleMatch::match_first_lowercase_only:
	return std::islower(data.front());
    case SimpleMatch::match_first_uppercase_only:
	return std::isupper(data.front());
    case SimpleMatch::match_first_digit_only:
	return std::isdigit(data.front());
    case SimpleMatch::match_first_alpha_only:
	return std::isalpha(data.front());
    case SimpleMatch::match_first_alnum_only:
	return std::isalnum(data.front());

	// Match last character
    case SimpleMatch::match_last_lowercase_only:
	return std::islower(data.back());
    case SimpleMatch::match_last_uppercase_only:
	return std::isupper(data.back());
    case SimpleMatch::match_last_digit_only:
	return std::isdigit(data.back());
    case SimpleMatch::match_last_alpha_only:
	return std::isalpha(data.back());
    case SimpleMatch::match_last_alnum_only:
	return std::isalnum(data.back());

	// Match all characters
    case SimpleMatch::match_all_lowercase_only:
	return std::all_of(data.begin(), data.end(),
			   [](auto c) { return std::islower(c); });
    case SimpleMatch::match_all_uppercase_only:
	return std::all_of(data.begin(), data.end(),
			   [](auto c) { return std::isupper(c); });
    case SimpleMatch::match_all_digit_only:
	return std::all_of(data.begin(), data.end(),
			   [](auto c) { return std::isdigit(c); });
    case SimpleMatch::match_all_alpha_only:
	return std::all_of(data.begin(), data.end(),
			   [](auto c) { return std::isalpha(c); });
    case SimpleMatch::match_all_alnum_only:
	return std::all_of(data.begin(), data.end(),
			   [](auto c) { return std::isalnum(c); });

	// Else string didn't match with anything above 
    default:
	return false;
    }
}

std::pair<std::int64_t, std::vector<Process>> simple_match(SimpleMatch pm) {
    std::vector<Process> list;
    std::int64_t how_many = 0;

    DIR *dir = opendir("/proc");
    if (dir == nullptr)
	error::make_error("error: cannot open /proc. Is it properly mounted?");

    struct dirent *den;

    while ((den = readdir(dir)) != nullptr) {
	const auto file_name = std::string_view(den->d_name);
	if (!contains_only_digits(file_name))
	    continue;

	const auto path = fmt::format("/proc/{}/comm", file_name);
	const auto data = read_to_string(path);
        if (has_match_cond(pm, data)) {
	    pid_t pid = 0;
	    std::from_chars(file_name.data(), file_name.data() + file_name.size(), pid);
	    list.push_back(Process{pid, data});
	    ++how_many;
	}
    }

    return {how_many, list};
}
