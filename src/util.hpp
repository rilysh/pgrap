#ifndef UTIL_HPP
#define UTIL_HPP

#include <algorithm>
#include <array>
#include <string>
#include <fcntl.h>
#include <string_view>
#include <unistd.h>

#include "fmt/base.h"

// Too expressive, which I currently don't need.
#undef UNUSED
#define UNUSED    [[maybe_unused]]

#undef TRY
#define TRY(x)						\
    try {						\
	x;						\
    } catch (const std::exception &e) {			\
	error::make_error("[exception]: {}", e.what());	\
    }

// Exit types
namespace exit_type {
constexpr inline auto exit_success = 0;
constexpr inline auto exit_failure = 1;

} // namespace exit_type

namespace error {
template <typename... ArgumentsType>
[[noreturn]]
inline void make_error(const fmt::format_string<ArgumentsType...> fmt,
		       ArgumentsType &&...args) noexcept {
    fmt::println(stderr, fmt, static_cast<ArgumentsType&&>(args)...);
    std::exit(exit_type::exit_failure);
}

}; // namespace error

inline bool contains_only_digits(const std::string_view source) noexcept {
    return std::all_of(source.begin(), source.end(), [](auto c) {
        return std::isdigit(c);
    });
}

inline bool contains_only_digits(const std::string &source) noexcept {
    return std::all_of(source.begin(), source.end(), [](auto c) {
        return std::isdigit(c);
    });
}

inline std::string read_to_string(const std::string &path) {
    std::int32_t fd = open(path.data(), O_RDONLY);
    if (fd == -1)
	error::make_error("error: cannot open file {}.", path); 

    ssize_t bytes_read;
    std::array<char, 20> buf{0};
    std::string res;

    while ((bytes_read = read(fd, buf.data(), buf.size())) > 0)
	res.append(buf.data(), bytes_read);

    close(fd);

    auto newline_pos = res.find('\n');
    if (newline_pos != std::string::npos)
	res.erase(newline_pos);

    return res;
}

#endif

