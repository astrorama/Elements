/**
 * @file PathSearch.cpp
 *
 * @date Dec 4, 2013
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

#include "ElementsKernel/PathSearch.h"

#include <iterator>  // for operator!=
#include <string>    // for allocator, string, basic_string
#include <vector>    // for vector

#include <boost/algorithm/string.hpp>       // for is_any_ofF, is_any_of, split
#include <boost/filesystem/operations.hpp>  // for exists, is_directory
#include <boost/filesystem/path.hpp>        // for operator==, path

#include "ElementsKernel/Environment.h"  // for Environment
#include "ElementsKernel/Logging.h"      // for Logging
#include "ElementsKernel/Path.h"         // for Item

namespace boost::filesystem {
class directory_iterator;
}
namespace boost::filesystem {
class recursive_directory_iterator;
}

using std::string;
using std::vector;

using boost::filesystem::directory_iterator;
using boost::filesystem::recursive_directory_iterator;

namespace Elements {
inline namespace Kernel {

namespace {
auto log = Logging::getLogger("PathSearch");
}

// template instantiations

template vector<string>     pathSearch<string, directory_iterator>(const string& searched_name, string directory);
template vector<Path::Item> pathSearch<Path::Item, directory_iterator>(const string& searched_name,
                                                                       Path::Item    directory);
template vector<string> pathSearch<string, recursive_directory_iterator>(const string& searched_name, string directory);
template vector<Path::Item> pathSearch<Path::Item, recursive_directory_iterator>(const string& searched_name,
                                                                                 Path::Item    directory);

template vector<Path::Item> pathSearch(const string& searched_name, Path::Item directory, SearchType search_type);
template vector<string>     pathSearch(const string& searched_name, string directory, SearchType search_type);

/**
 * Iterate over the different directories included in the path-like environment variable, i.e.,
 *
 * path1:path2:path3 ...
 *
 * and call pathSearch(...) for each of them
 */
vector<Path::Item> pathSearchInEnvVariable(const string& file_name, const string& path_like_env_variable,
                                           const SearchType search_type) {
  // Placeholder for the to-be-returned search result
  vector<Path::Item> search_results{};

  // get the multiple path from the environment variable
  string multiple_path{};

  if (Environment::hasKey(path_like_env_variable)) {
    Environment current_env;
    multiple_path = current_env[path_like_env_variable];
  } else {
    log.warn() << "Environment variable \"" << path_like_env_variable << "\" is not defined !";
  }

  // Tokenize the path elements
  vector<string> path_elements;
  split(path_elements, multiple_path, boost::is_any_of(";:"));

  // Loop over all path elements
  for (const string& path_element : path_elements) {
    // Check if directory exists
    if (boost::filesystem::exists(path_element) && boost::filesystem::is_directory(path_element)) {
      // loop recursively inside directory
      auto single_path_results = pathSearch(file_name, Path::Item{path_element}, search_type);
      search_results.insert(search_results.end(), single_path_results.cbegin(), single_path_results.cend());
    }
  }
  return search_results;
}

}  // namespace Kernel
}  // namespace Elements
