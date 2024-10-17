#include <cstdio>
#include <cstdlib>
#include <getopt.h>

#include "fmt/base.h"

#include "hs_match.hpp"
#include "pgrap.hpp"
#include "simple_match.hpp"
#include "stand_match.hpp"
#include "util.hpp"

[[noreturn]]
static void print_usage(UNUSED std::int32_t status) {
    std::FILE *out = status == exit_type::exit_success ?
	stdout : stderr;
    fmt::println(out,
		 "pgrap\n"
		 "* Regex Types\n"
		 " --simple   - Basic search matching each character\n"
		 " --stand    - Use the C++ standard library regex implementation\n"
		 " --hs       - Use Intel hyperscan regex library\n\n"
		 "* Misc\n"
		 " --pattern  - Provide the regex pattern to search\n"
		 " --mtype    - Match type for --simple argument\n"
		 " --rtype    - Special regex cases for --stand and --hs arguments\n"
		 " --total    - Total number of matches\n");

    std::exit(status);
}

[[noreturn]]
static void print_mtype_list() {
    fmt::println(
	"* Match types\n"
	"Note: These are only can used by the '--simple' option\n\n"
	"[01] all\n"
	"[02] first-lowercase-only\n"
	"[03] first-uppercase-only\n"
	"[04] first-digit-only\n"
	"[05] first-alpha-only\n"
	"[06] first-alnum-only\n"
	"[07] last-lowercase-only\n"
	"[08] last-uppercase-only\n"
	"[09] last-digit-only\n"
	"[10] last-alpha-only\n"
	"[11] last-alnum-only\n"
	"[12] all-lowercase-only\n"
	"[13] all-uppercase-only\n"
	"[14] all-digit-only\n"
	"[15] all-alpha-only\n"
	"[16] all-alnum-only");
    std::exit(exit_type::exit_success);
}

int main(int argc, char **argv) {
    static const struct option opt_options[] = {
	{ "simple",    no_argument, nullptr, 1 },
	{ "mtype",     no_argument, nullptr, 2 },
	{ "stand",     no_argument, nullptr, 3 },
	{ "rtype",     no_argument, nullptr, 4 },
	{ "hs",        no_argument, nullptr, 5 },
	{ "pattern",   no_argument, nullptr, 6 },
	{ "total",     no_argument, nullptr, 7 },
	{ "mlist",     no_argument, nullptr, 8 },
	{ "help",      no_argument, nullptr, 9 },
	{ nullptr,     0,           nullptr, 0 },
    };

    // If no argument is provided or just '-' or '--' is provided
    // print the help message, and then exit.
    if (argc == 1 || (argv[1][0] == '-' && argv[1][1] == '\0') ||
	(argv[1][0] == '-' && argv[1][1] == '-' &&
	 argv[1][2] == '\0'))
	print_usage(exit_type::exit_failure);
    
    Options opt = {
	.opt_simple = false,
	.opt_mtype = false,
	.opt_stand = false,
	.opt_pattern = false,
	.opt_rtype = false,
	.opt_hs = false,
	.opt_total = false,
	.opt_mlist = false,
    };

    std::string_view mtype;
    std::string pattern;
    std::string_view rtype;

    std::int32_t c;
    while ((c = getopt_long(argc, argv, "", opt_options, nullptr)) != -1) {
	switch (c) {
	case 1:
	    opt.opt_simple = true;
	    break;
	case 2:
	    opt.opt_mtype = true;
	    if (argv[optind] == nullptr)
		error::make_error("error: match type requires you to specify a type.");
	    mtype = argv[optind];
	    break;	
	case 3:
	    opt.opt_stand = true;
	    break;
	case 4:
	    opt.opt_rtype = true;
	    if (argv[optind] == nullptr)
		error::make_error("error: regex type requires you to specify "
				  "standard regex pattern type.");
	    rtype = argv[optind];
	    break;
	case 5:
	    opt.opt_hs = true;
	    break;
	case 6:
	    opt.opt_pattern = true;
	    if (argv[optind] == nullptr)
		error::make_error("error: pattern requires you to specify a regex pattern.");
	    pattern = argv[optind];
	    break;
	case 7:
	    opt.opt_total = true;
	    break;
	case 8:
	    opt.opt_mlist = true;
	    break;
	case 9:
	    print_usage(exit_type::exit_success);
	default:
	    std::exit(1);
	}
    }

    if (!opt.opt_simple && opt.opt_mtype)
	error::make_error("error: missing '--simple' argument.");
    if (opt.opt_pattern && (!opt.opt_stand && !opt.opt_hs))
	error::make_error("error: '--pattern' requires '--stand' or '--hs' argument.");
    if (opt.opt_total && (!opt.opt_stand && !opt.opt_hs))
	error::make_error("error: '--total' requires '--stand' or '--hs' argument.");

    // Argument: --simple
    if (opt.opt_simple) {
	SimpleMatch sm_type = SimpleMatch::match_all;

        if (opt.opt_mtype) {
	    if (opt.opt_mlist)
		print_mtype_list();

	    bool found_one = false;
	    for (const auto &type : simple_match_types) {
		if (type.mtype == mtype) {
		    sm_type = type.smtype;
		    found_one = true;
		}
	    }

	    if (!found_one)
		error::make_error("error: invalid simple match type.");
	}

	const auto [how_many, entries] = simple_match(sm_type);
	for (const auto &entry : entries)
	    fmt::println("{} ({})", entry.pid, entry.name);

	if (opt.opt_total)
	    fmt::println(
		"{} processes @'{}'.", how_many, mtype);
    }

    // Argument: --stand
    if (opt.opt_stand) {
	if (!opt.opt_pattern)
	    pattern = "";

        auto syntax = std::regex_constants::syntax_option_type{0};
	bool found_one = false;

	if (opt.opt_rtype) {
	    // Split the rtype argument, iterate over regex standards
	    // and pick ones that match each splitted argument with
	    // each standard types.
	    for (const auto word : std::views::split(rtype, ',')) {
		const auto word_view = std::string_view(word.begin(), word.end());
		for (const auto &regex_stand : stand_rtypes) {
		    // e.g. rtype (egrep) == (word_view) egrep
		    if (regex_stand.rtype == word_view) {
			syntax |= regex_stand.syntax;
			found_one = true;
		    }
		}
	    }

	    if (!found_one) {
		fmt::println(stderr, "note: defaulting to ECMA.", rtype);
		syntax = std::regex_constants::ECMAScript;
	    }
	} else {
	    syntax = std::regex_constants::ECMAScript;
	}

	const auto [how_many, entries] = stand_regex_match(pattern, syntax);
	for (const auto &entry : entries)
	    fmt::println("{} ({})", entry.pid, entry.name);

	if (opt.opt_total)
	    fmt::println(
		"{} processes @'{}'.", how_many, pattern.empty() ?
		"(empty)" : pattern);
    }

    // Argument: --hs
    if (opt.opt_hs) {
	if (!opt.opt_pattern)
	    pattern = "";

	std::int32_t mode = opt.opt_rtype ?
	    get_hs_compile_mode(rtype) : HS_FLAG_ALLOWEMPTY;

        const auto [how_many, entries] = hs_regex_match(mode, pattern);
	for (const auto &entry : entries)
	    fmt::println("{} ({})", entry.pid, entry.name);

	if (opt.opt_total)
	    fmt::println(
		"{} processes @'{}'.", how_many, pattern.empty() ?
		"(empty)" : pattern);
    }
}
