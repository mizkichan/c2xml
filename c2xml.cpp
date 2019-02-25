// I do not know why this macro has suddenly been necessary here.
#define YY_DECL auto yylex(void)->yy::parser::symbol_type

#include "lexer.hpp"
#include "parser.hpp"
#include <boost/program_options.hpp>
#include <cassert>
#include <cstdarg>
#include <iostream>

extern int yy_flex_debug;

extern auto main(int argc, char **argv) -> int {
  using boost::program_options::command_line_parser;
  using boost::program_options::options_description;
  using boost::program_options::positional_options_description;
  using boost::program_options::value;
  using boost::program_options::variables_map;

  auto visible = options_description();
  // clang-format off
  visible.add_options()
    ("help,h", "Show help")
    ("verbose,v", "Get verbose")
    ("output,o", value<std::string>(), "Output file path");
  // clang-format on

  // clang-format off
  auto hidden = options_description();
  hidden.add_options()
    ("input", value<std::string>(), "Intput file path");
  // clang-format on

  auto options = variables_map();
  try {
    boost::program_options::store(
        command_line_parser(argc, argv)
            .options(options_description().add(visible).add(hidden))
            .positional(positional_options_description().add("input", 1))
            .run(),
        options);
    boost::program_options::notify(options);
  } catch (boost::program_options::error &e) {
    std::cerr << e.what() << std::endl;
    return EXIT_FAILURE;
  }

  if (options.count("help")) {
    std::cout << "Usage: " << argv[0] << " [options] [file]" << std::endl
              << "Options: " << std::endl
              << visible;
    return EXIT_SUCCESS;
  }

  if (options.count("input")) {
    yyin = std::fopen(options["input"].as<std::string>().c_str(), "r");
  }

  if (options.count("output")) {
    yyout = std::fopen(options["output"].as<std::string>().c_str(), "w");
  }

  yy_flex_debug = 0;
  yy::parser yyparse;
  return yyparse();
}

// vim: set ts=2 sw=2 et:
