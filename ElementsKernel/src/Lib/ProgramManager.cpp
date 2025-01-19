/**
 * @file ProgramManager.cpp
 *
 * @date Jan 7, 2015
 * @author Pierre Dubath
 *
 * @copyright 2012-2020 Euclid Science Ground Segment
 *
 * This library is free software; you can redistribute it and/or modify it under the terms of the GNU Lesser General
 * Public License as published by the Free Software Foundation; either version 3.0 of the License, or (at your option)
 * any later version.
 *
 * This library is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more
 * details.
 *
 * You should have received a copy of the GNU Lesser General Public License along with this library; if not, write to
 * the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
 *
 */

#include "ElementsKernel/ProgramManager.h"

#include <algorithm>  // for transform
#include <cstdint>    // for int64_t
#include <cstdlib>    // for exit, _Exit, abort
#include <exception>  // for exception, current_exception, rethrow_exception, exception_ptr
#include <fstream>    // IWYU pragma: keep
#include <iostream>   // for basic_ostream, operator<<, endl, basic_ostream::operator<<, cerr, cout, ifstream
#include <map>        // for operator!=, map, _Rb_tree_const_iterator
#include <sstream>    // for stringstream
#include <string>     // for basic_string, char_traits, operator<<, operator+, string, operator==
#include <utility>    // for pair, move
#include <vector>     // for vector

#include <boost/algorithm/string.hpp>       // for starts_with
#include <boost/any.hpp>                    // for any
#include <boost/filesystem/operations.hpp>  // for canonical, complete, exists
#include <boost/filesystem/path.hpp>        // for path, operator<<, operator>>, operator!=, operator/
#include <boost/program_options.hpp>  // for variable_value, store, value, options_description_easy_init, typed_value, basic_command_line_parser, collect_unrecognized, notify, operator<<, options_description, parse_config_file, command_line_parser, variables_map, basic_parsed_options, collect_unrecognized_mode, include_positional
#include <boost/smart_ptr.hpp>        // for shared_ptr

#include "ElementsKernel/Configuration.h"  // for getConfigurationPath, getConfigurationLocations
#include "ElementsKernel/Exception.h"      // for Exception
#include "ElementsKernel/Exit.h"           // for ExitCode
#include "ElementsKernel/Logging.h"        // for Logging
#include "ElementsKernel/ModuleInfo.h"     // for getExecutablePath
#include "ElementsKernel/Path.h"           // for Item, joinPath, multiPathAppend, SUFFIXES, VARIABLE, PATH_SEP
#include "ElementsKernel/Program.h"        // for Program
#include "ElementsKernel/System.h"         // for backTrace
#include "ElementsKernel/Unused.h"         // for ELEMENTS_UNUSED

#include "OptionException.h"  // for OptionException

using log4cpp::Priority;
using std::cerr;
using std::endl;
using std::string;
using std::vector;

namespace Elements {

namespace {
auto log = Logging::getLogger("ElementsProgram");
}

using System::getExecutablePath;
using VariablesMap = Program::VariablesMap;

ProgramManager::ProgramManager(std::unique_ptr<Program> program_ptr, const string& parent_project_version,
                               const string& parent_project_name, const string& parent_project_vcs_version,
                               const string& parent_module_version, const string& parent_module_name,
                               const vector<string>& search_dirs, const Priority::Value& elements_loglevel,
                               const bool no_config_file, const bool no_default_conf)
    : m_program_ptr(std::move(program_ptr))
    , m_parent_project_version(parent_project_version)
    , m_parent_project_name(parent_project_name)
    , m_parent_project_vcs_version(parent_project_vcs_version)
    , m_parent_module_version(parent_module_version)
    , m_parent_module_name(parent_module_name)
    , m_search_dirs(search_dirs)
    , m_elements_loglevel(elements_loglevel)
    , m_no_config_file(no_config_file)
    , m_no_default_conf(no_default_conf) {}

const Path::Item& ProgramManager::getProgramPath() const {
  return m_program_path;
}

const Path::Item& ProgramManager::getProgramName() const {
  return m_program_name;
}

/**
 * @brief Get default config file
 * @todo check whether priotities are correct if more than one
 * config file is found in pathSearchInEnvVariable
 * */
Path::Item ProgramManager::getDefaultConfigFile(const Path::Item& program_name, const string& module_name) {
  Path::Item default_config_file{};

  // .conf is the standard extension for configuration file
  Path::Item conf_name(program_name);
  conf_name.replace_extension("conf");

  // Construct and return the full path
  default_config_file = getConfigurationPath(conf_name.string(), false);
  if (default_config_file.empty()) {
    log.warn() << "The " << conf_name << " default configuration file cannot be found in:";
    for (auto loc : getConfigurationLocations()) {
      log.warn() << " " << loc;
    }
    if (not module_name.empty()) {
      conf_name = Path::Item{module_name} / conf_name;
      log.warn() << "Trying " << conf_name << ".";
      default_config_file = getConfigurationPath(conf_name.string(), false);
    }
  }

  if (default_config_file.empty()) {
    log.debug() << "Couldn't find " << conf_name << " default configuration file.";
  } else {
    log.debug() << "Found " << conf_name << " default configuration file at " << default_config_file;
  }

  return default_config_file;
}

Path::Item ProgramManager::setProgramName(ELEMENTS_UNUSED char* arg0) {
  const Path::Item full_path = getExecutablePath();

  return full_path.filename();
}

Path::Item ProgramManager::setProgramPath(ELEMENTS_UNUSED char* arg0) {
  const Path::Item full_path = getExecutablePath();

  return full_path.parent_path();
}

/*
 * Get program options
 */
VariablesMap ProgramManager::getProgramOptions(int argc, char* argv[]) {
  using std::cout;
  using std::exit;
  using OptionsDescription = Program::OptionsDescription;
  using boost::program_options::collect_unrecognized;
  using boost::program_options::command_line_parser;
  using boost::program_options::include_positional;
  using boost::program_options::notify;
  using boost::program_options::parse_config_file;
  using boost::program_options::store;
  using boost::program_options::value;

  VariablesMap var_map{};
  Path::Item   config_file;

  // default value for default_log_level option
  string default_log_level = "INFO";

  // Define the options which can be given only at the command line
  OptionsDescription cmd_only_generic_options{};
  cmd_only_generic_options.add_options()("version", "Print version string")("help", "Produce help message");

  if (not m_no_config_file) {
    // Get defaults

    Path::Item default_config_file{};

    if (not m_no_default_conf) {
      default_config_file = getDefaultConfigFile(getProgramName(), m_parent_module_name);
    }

    cmd_only_generic_options.add_options()("config-file", value<Path::Item>()->default_value(default_config_file),
                                           "Name of a configuration file");
  }

  // Define the options which can be given both at command line and conf file
  OptionsDescription cmd_and_file_generic_options{};
  cmd_and_file_generic_options.add_options()("log-level", value<string>()->default_value(default_log_level),
                                             "Log level: FATAL, ERROR, WARN, INFO (default), DEBUG")(
      "log-file", value<Path::Item>(), "Name of a log file");

  // Group all the generic options, for help output. Note that we add the
  // options one by one to avoid having empty lines between the groups
  OptionsDescription all_generic_options{"Generic options"};
  for (const auto& o : cmd_only_generic_options.options()) {
    all_generic_options.add(o);
  }
  for (const auto& o : cmd_and_file_generic_options.options()) {
    all_generic_options.add(o);
  }

  // Get the definition of the specific options and arguments (positional
  // options) from the derived class
  auto specific_options                                    = m_program_ptr->defineSpecificProgramOptions();
  auto [option_description, positional_option_description] = m_program_ptr->defineProgramArguments();
  OptionsDescription all_specific_options{};
  all_specific_options.add(specific_options).add(option_description);

  // Put together all the options to parse from the cmd line and the file
  OptionsDescription all_cmd_and_file_options{};
  all_cmd_and_file_options.add(cmd_and_file_generic_options).add(all_specific_options);

  // Put together all the options to use for the help message
  OptionsDescription help_options{};
  help_options.add(all_generic_options).add(all_specific_options);

  // Perform a first parsing of the command line, to handle the cmd only options
  auto cmd_parsed_options =
      command_line_parser(argc, argv).options(cmd_only_generic_options).allow_unregistered().run();

  checkCommandLineOptions(cmd_parsed_options);

  store(cmd_parsed_options, var_map);

  // Deal with the "help" option
  if (var_map.count("help") > 0) {
    cout << help_options << endl;
    exit(static_cast<int>(ExitCode::OK));
  }

  // Deal with the "version" option
  if (var_map.count("version") > 0) {
    cout << getVersion() << endl;
    exit(static_cast<int>(ExitCode::OK));
  }

  if (not m_no_config_file) {
    // Get the configuration file. It is guaranteed to exist, because it has
    // default value
    config_file = var_map.at("config-file").as<Path::Item>();
  }

  // Parse from the command line the rest of the options. Here we also handle
  // the positional arguments.
  auto leftover_cmd_options = collect_unrecognized(cmd_parsed_options.options, include_positional);

  try {

    auto parsed_cmdline_options = command_line_parser(leftover_cmd_options)
                                      .options(all_cmd_and_file_options)
                                      .positional(positional_option_description)
                                      .run();

    store(parsed_cmdline_options, var_map);

    if (not m_no_config_file) {
      // Parse from the configuration file if it exists
      if (not config_file.empty() and exists(config_file)) {
        if (std::ifstream ifs{config_file.string()}) {
          auto parsed_cfgfile_options = parse_config_file(ifs, all_cmd_and_file_options);
          store(parsed_cfgfile_options, var_map);
        }
      }
    }

  } catch (const std::exception& e) {
    if (boost::starts_with(e.what(), "unrecognised option") or
        boost::starts_with(e.what(), "too many positional options")) {
      throw OptionException(e.what());
    } else {
      throw;
    }
  }
  // After parsing both the command line and the conf file notify the variables
  // map, so we can get any messages for missing parameters
  notify(var_map);

  // return the var_map loaded with all options
  return var_map;
}

void ProgramManager::logHeader(const string& program_name) const {
  log.log(m_elements_loglevel, "##########################################################");
  log.log(m_elements_loglevel, "##########################################################");
  log.log(m_elements_loglevel, "#");
  log.log(m_elements_loglevel, "#  C++ program:  " + program_name + " starts ");
  log.log(m_elements_loglevel, "#");
  log.debug("# The Program Name: " + m_program_name.string());
  log.debug("# The Program Path: " + m_program_path.string());
}

void ProgramManager::logFooter(const string& program_name) const {
  log.log(m_elements_loglevel, "##########################################################");
  log.log(m_elements_loglevel, "#");
  log.log(m_elements_loglevel, "#  C++ program:  " + program_name + " stops ");
  log.log(m_elements_loglevel, "#");
  log.log(m_elements_loglevel, "##########################################################");
  log.log(m_elements_loglevel, "##########################################################");
}

// Log all options with a header
void ProgramManager::logAllOptions() const {
  using std::int64_t;
  using std::stringstream;

  log.log(m_elements_loglevel, "##########################################################");
  log.log(m_elements_loglevel, "#");
  log.log(m_elements_loglevel, "# List of all program options");
  log.log(m_elements_loglevel, "# ---------------------------");
  log.log(m_elements_loglevel, "#");

  // Build a log message
  stringstream log_message{};

  // Loop over all options included in the variable_map
  for (const auto& [name, content] : m_variables_map) {
    // string option
    if (content.value().type() == typeid(string)) {
      log_message << name << " = " << content.as<string>();
      // double option
    } else if (content.value().type() == typeid(double)) {
      log_message << name << " = " << content.as<double>();
      // int64_t option
    } else if (content.value().type() == typeid(int64_t)) {
      log_message << name << " = " << content.as<int64_t>();
      // int option
    } else if (content.value().type() == typeid(int)) {
      log_message << name << " = " << content.as<int>();
      // bool option
    } else if (content.value().type() == typeid(bool)) {
      log_message << name << " = " << content.as<bool>();
      // path option
    } else if (content.value().type() == typeid(Path::Item)) {
      log_message << name << " = " << content.as<Path::Item>();
      // int vector option
    } else if (content.value().type() == typeid(vector<int>)) {
      auto         intVec = content.as<vector<int>>();
      stringstream vecContent{};
      for (const auto& i : intVec) {
        vecContent << " " << i;
      }
      log_message << name << " = {" << vecContent.str() << " }";
      // double vector option
    } else if (content.value().type() == typeid(vector<double>)) {
      auto         intVec = content.as<vector<double>>();
      stringstream vecContent{};
      for (const auto& i : intVec) {
        vecContent << " " << i;
      }
      log_message << name << " = {" << vecContent.str() << " }";
      // string vector option
    } else if (content.value().type() == typeid(vector<string>)) {
      auto         intVec = content.as<vector<string>>();
      stringstream vecContent{};
      for (const auto& i : intVec) {
        vecContent << " " << i;
      }
      log_message << name << " = {" << vecContent.str() << " }";
      // if nothing else
    } else {
      log_message << "Option " << name << " of type " << content.value().type().name() << " not supported in logging !"
                  << endl;
    }
    // write the log message
    log.log(m_elements_loglevel, log_message.str());
    log_message.str("");
  }
  log.log(m_elements_loglevel, "#");
}

// Log all options with a header
void ProgramManager::logTheEnvironment() const {
  log.debug() << "##########################################################";
  log.debug() << "#";
  log.debug() << "# Environment of the Run";
  log.debug() << "# ---------------------------";
  log.debug() << "#";

  for (const auto& [type, name] : Path::VARIABLE) {
    log.debug() << name << ": " << m_env[name];
  }

  log.debug() << "#";
}

void ProgramManager::bootstrapEnvironment(char* arg0) {
  m_program_name = setProgramName(arg0);
  m_program_path = setProgramPath(arg0);

  vector<Path::Item> local_search_paths(m_search_dirs.size());

  std::transform(m_search_dirs.cbegin(), m_search_dirs.cend(), local_search_paths.begin(), [](const string& s) {
    return boost::filesystem::absolute(s);
  });

  // insert local parent dir if it is not already
  // the first one of the list
  const Path::Item this_parent_path = canonical(m_program_path.parent_path());
  if (local_search_paths[0] != this_parent_path) {
    const auto b = local_search_paths.begin();
    local_search_paths.insert(b, this_parent_path);
  }

  using Path::joinPath;
  using Path::multiPathAppend;

  for (const auto& [type, name] : Path::VARIABLE) {
    if (m_env[name].exists()) {
      m_env[name] += Path::PATH_SEP + joinPath(multiPathAppend(local_search_paths, Path::SUFFIXES.at(type)));
    } else {
      m_env[name] = joinPath(multiPathAppend(local_search_paths, Path::SUFFIXES.at(type)));
    }
  }
}

// Get the program options and setup logging
void ProgramManager::setup(const int argc, char* argv[]) {
  // store the program name and path in class variable
  // and retrieve the local environment
  bootstrapEnvironment(argv[0]);

  // get all program options into the varaiable_map
  try {
    m_variables_map = getProgramOptions(argc, argv);
  } catch (const OptionException& e) {
    auto exit_code = e.exitCode();
    log.fatal() << "# Elements Exception : " << e.what();
    std::_Exit(static_cast<int>(exit_code));
  }

  // get the program options related to the logging
  string logging_level;
  if (m_variables_map.count("log-level")) {
    logging_level = m_variables_map["log-level"].as<string>();
  } else {
    throw Exception("Required option log-level is not provided!", ExitCode::CONFIG);
  }

  if (m_variables_map.count("log-file")) {
    const Path::Item log_file_name = m_variables_map["log-file"].as<Path::Item>();
    Logging::setLogFile(log_file_name);
  }

  // setup the logging
  Logging::setLevel(logging_level);

  logHeader(m_program_name.string());
  // log all program options
  logAllOptions();
  logTheEnvironment();
}

void ProgramManager::tearDown(const ExitCode& c) const {
  log.debug() << "# Exit Code: " << static_cast<int>(c);

  logFooter(m_program_name.string());
}

// This is the method call from the main which does everything
ExitCode ProgramManager::run(const int argc, char* argv[]) {
  setup(argc, argv);

  const ExitCode exit_code = m_program_ptr->mainMethod(m_variables_map);

  tearDown(exit_code);

  return exit_code;
}

string ProgramManager::getVersion() const {
  string version = m_parent_project_name + " " + m_parent_project_vcs_version;

  return version;
}

ProgramManager::~ProgramManager() = default;

void ProgramManager::onTerminate() noexcept {
  auto exit_code{ExitCode::NOT_OK};

  if (const auto exc = std::current_exception()) {

    log.fatal() << "Crash detected";
    log.fatal() << "This is the back trace:";
    for (const auto& level : System::backTrace(21, 4)) {
      log.fatal() << level;
    }

    // we have an exception
    try {
      std::rethrow_exception(exc);  // throw to recognize the type
    } catch (const Exception& exc1) {
      log.fatal() << "# ";
      log.fatal() << "# Elements Exception : " << exc1.what();
      log.fatal() << "# ";
    } catch (const std::exception& exc2) {
      log.fatal() << "# ";
      log.fatal() << "# Standard Exception : " << exc2.what();
      log.fatal() << "# ";
    } catch (...) {
      log.fatal() << "# ";
      log.fatal() << "# An exception of unknown type occurred, "
                  << "i.e., an exception not deriving from std::exception ";
      log.fatal() << "# ";
    }

    abort();
  }

  std::_Exit(static_cast<int>(exit_code));
}

}  // namespace Elements
