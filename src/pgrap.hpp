#ifndef PGRAP_HPP
#define PGRAP_HPP

#include <string>
#include <charconv>
#include <sys/types.h>
#include <vector>
#include <dirent.h>

#include "fmt/format.h"

struct Process {
    pid_t pid;
    const std::string name;

    explicit Process(pid_t arg_pid, const std::string arg_name)
	: pid{arg_pid}, name{arg_name} {}

    Process() = default;
};

// Argument option structure
struct Options {
    bool opt_simple;
    bool opt_mtype;
    bool opt_stand;
    bool opt_pattern;
    bool opt_rtype;
    bool opt_hs;
    bool opt_total;
    bool opt_mlist;
};

#endif

