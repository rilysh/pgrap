#include "pgrap.hpp"
#include "util.hpp"
#include "stand_match.hpp"

static inline bool has_regex_matched(const std::string &content,
                                     const std::regex &regex) {
    const auto begin =
	std::regex_iterator(content.begin(), content.end(), regex);
    const auto end = std::sregex_iterator();
    return begin != end;
}

std::pair<std::int64_t, std::vector<Process>> stand_regex_match(
    const std::string &pattern, std::regex_constants::syntax_option_type syntax) {

    std::regex self_regex;

    TRY(self_regex = std::regex(pattern, syntax));
    std::vector<Process> list;
    std::int64_t how_many = 0;

    DIR *dir = opendir("/proc/");
    if (dir == nullptr)
	error::make_error("error: cannot open /proc. Is it properly mounted?");

    struct dirent *den;
    while ((den = readdir(dir)) != nullptr) {
	const auto file_name = std::string_view(den->d_name);
	if (!contains_only_digits(file_name))
	    continue;

	const auto path = fmt::format("/proc/{}/comm", file_name);
	const auto data = read_to_string(path);
	if (has_regex_matched(data, self_regex)) {
	    pid_t pid = 0;
	    std::from_chars(file_name.data(), file_name.data() + file_name.size(), pid);
            list.push_back(Process{pid, data});
	    ++how_many;
	}
    }

    closedir(dir);
    return {how_many, list};
}
