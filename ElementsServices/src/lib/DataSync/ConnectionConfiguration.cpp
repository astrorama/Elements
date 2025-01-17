/**
 * Copyright (C) 2012-2020 Euclid Science Ground Segment
 *
 * This library is free software; you can redistribute it and/or modify it under
 * the terms of the GNU Lesser General Public License as published by the Free
 * Software Foundation; either version 3.0 of the License, or (at your option)
 * any later version.
 *
 * This library is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more
 * details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this library; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
 */

#include <cstddef>    // for size_t
#include <exception>  // for exception
#include <stdexcept>  // for runtime_error
#include <string>     // for basic_string, allocator, string, operator==, operator+
#include <vector>     // for vector

#include <boost/filesystem/path.hpp>  // for operator/, path
#include <boost/program_options.hpp>  // for value, typed_value, variables_map, options_description_easy_init, variable_value, notify, parse_config_file, store, options_description, program_options

#include "ElementsServices/DataSync/ConnectionConfiguration.h"  // for ConnectionConfiguration, OverwritingPolicy, DataHost, UnknownHost
#include "ElementsServices/DataSync/DataSyncUtils.h"  // for lower, valueIsListed, confFilePath, localWorkspacePrefix, path

namespace Elements {
inline namespace Services {
namespace DataSync {

using std::string;

ConnectionConfiguration::ConnectionConfiguration(const path& filename) {
  parseConfigurationFile(filename);
}

bool ConnectionConfiguration::overwritingAllowed() const {
  return overwritingPolicy == OverwritingPolicy::OVERWRITE;
}

void ConnectionConfiguration::parseConfigurationFile(const path& filename) {
  // @TODO clean function

  namespace po = boost::program_options;

  /* Declare options */
  po::options_description options{};
  options.add_options()("host", po::value<string>(), "Hosting solution: iRODS or WebDAV (case insensitive)")(
      "host-url", po::value<string>()->default_value(""),
      "Host URL if needed")("user", po::value<string>()->default_value(""), "User name if needed")(
      "password", po::value<string>()->default_value(""), "Password if needed")(
      "overwrite", po::value<string>()->default_value("no"), "Allow overwriting local files if they already exist")(
      "distant-workspace", po::value<string>(), "Path to distant repository workspace")(
      "local-workspace", po::value<string>(),
      "Path to local repository workspace")("tries", po::value<int>()->default_value(4), "Number of download tries");

  /* Get config file path */
  const path abs_path = confFilePath(filename);

  /* Read config file */
  po::variables_map vm;
  try {
    po::store(po::parse_config_file<char>(abs_path.c_str(), options), vm);
    po::notify(vm);
  } catch (std::exception& e) {
    throw std::runtime_error("Error while reading configuration file: " + abs_path.string() + " - " + e.what());
  }

  /* Configure object */
  parseHost(vm["host"].as<string>());
  hostUrl  = vm["host-url"].as<string>();
  user     = vm["user"].as<string>();
  password = vm["password"].as<string>();
  parseOverwritingPolicy(vm["overwrite"].as<string>());
  distantRoot = vm["distant-workspace"].as<string>();
  localRoot   = localWorkspacePrefix() / vm["local-workspace"].as<string>();
  tries       = static_cast<std::size_t>(vm["tries"].as<int>());
}

void ConnectionConfiguration::parseHost(const string& name) {
  if (const string uncased = lower(name); uncased == "irods") {
    host = DataHost::IRODS;
  } else if (uncased == "webdav") {
    host = DataHost::WEBDAV;
  } else {
    throw UnknownHost(name);
  }
}

void ConnectionConfiguration::parseOverwritingPolicy(const string& policy) {

  using std::vector;

  const vector<string> overwrite_allowed_options   = {"true", "yes", "y"};
  const vector<string> overwrite_forbidden_options = {"false", "no", "n"};

  if (string uncased = lower(policy); valueIsListed(uncased, overwrite_allowed_options)) {
    overwritingPolicy = OverwritingPolicy::OVERWRITE;
  } else if (valueIsListed(uncased, overwrite_forbidden_options)) {
    overwritingPolicy = OverwritingPolicy::ABORT;
  } else {
    throw std::runtime_error("I don't know this overwriting policy: " + policy);
  }
}

}  // namespace DataSync
}  // namespace Services
}  // namespace Elements
