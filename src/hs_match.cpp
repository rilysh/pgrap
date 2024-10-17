#include "hs_compile.h"
#include "hs_common.h"
#include "hs_runtime.h"

#include "pgrap.hpp"
#include "hs_match.hpp"
#include "util.hpp"

static std::int32_t on_hs_match(UNUSED unsigned int id, UNUSED unsigned long long from,
				UNUSED unsigned long long to, UNUSED unsigned int flags,
				void *ctx) {
    HSMatch *hm = static_cast<HSMatch *>(ctx);
    hm->found_match = true;
    // how_many was initialized at the beginning.
    ++hm->how_many;

    return HS_SUCCESS;
}

static inline void do_hyperscan_match(hs_database_t *db, hs_scratch_t *scratch,
				      struct HSMatch *hm, const std::string &data) {
    if (hs_scan(db, data.c_str(), data.size(), 0, scratch,
		on_hs_match, hm) != HS_SUCCESS) {
	hs_free_database(db);
	error::make_error("error: cannot perform bytecode scanning.");
    }
}

std::pair<std::int64_t, std::vector<Process>> hs_regex_match(
    std::int32_t mode, const std::string &pattern) {

    std::vector<Process> list;

    hs_database_t *db;        
    hs_compile_error_t *comp_error;
    if (hs_compile(pattern.data(), mode, HS_MODE_BLOCK, nullptr,
		   &db, &comp_error) != HS_SUCCESS) {
        hs_free_compile_error(comp_error);
	error::make_error("error: cannot compile the pattern, "
			  "it's probably invalid.");
    }

    hs_scratch_t *scratch = nullptr;
    if (hs_alloc_scratch(db, &scratch) != HS_SUCCESS) {
	hs_free_database(db);
	error::make_error("error: cannot allocated enough memory "
			  "for the scratch space.");
    }

    auto hm = HSMatch(false, 0);

    DIR *dir = opendir("/proc/");
    if (dir == nullptr)
	error::make_error("error: cannot open /proc. Is it properly mounted?");

    struct dirent *den;
    while ((den = readdir(dir)) != nullptr) {
        const auto file_name = std::string_view{den->d_name};
        if (!contains_only_digits(file_name))
	    continue;

	const auto path = fmt::format("/proc/{}/comm", file_name);
	const auto data = read_to_string(path);

	hm.found_match = false;
	do_hyperscan_match(db, scratch, &hm, data);
	if (hm.found_match) {
	    pid_t pid = 0;
	    std::from_chars(file_name.data(), file_name.data() + file_name.size(), pid);
            list.push_back(Process{pid, data});
	}
    }

    closedir(dir);    
    hs_free_scratch(scratch);
    hs_free_database(db);

    return {hm.how_many, list};
}
