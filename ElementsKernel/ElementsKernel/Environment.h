/**
 * @file ElementsKernel/Environment.h
 * @brief Defines a class to handle the Environment
 * @date Jun 17, 2016
 * @author Hubert Degaudenzi
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

/**
 * @addtogroup ElementsKernel ElementsKernel
 * @{
 */

#ifndef ELEMENTSKERNEL_ELEMENTSKERNEL_ENVIRONMENT_H_
#define ELEMENTSKERNEL_ELEMENTSKERNEL_ENVIRONMENT_H_

#include <functional>  // for reference_wrapper
#include <iostream>    // for ostream
#include <map>         // for map
#include <string>      // for string
#include <vector>      // for vector

#include "ElementsKernel/Export.h"  // for ELEMENTS_API

namespace Elements {

/**
 * @brief Python dictionary-like Environment interface
 * @ingroup ElementsKernel
 */
class ELEMENTS_API Environment {
public:
  class Variable;

public:
  /// default constructor
  explicit Environment(bool keep_same = true);
  /**
   * @brief Destructor for the Environment class
   *
   * Releases any resources or performs cleanup tasks for the Environment object.
   * Automatically restores the environment state upon destruction, ensuring
   * consistency with the initial environment setup.
   *
   * @ingroup ElementsKernel
   */
  virtual ~Environment();
  Variable operator[](const std::string&);
  Variable operator[](const std::string& index) const;
  /**
   * @brief Restores the environment to its previous state by unsetting added variables
   *        and resetting modified variables to their original values.
   *
   * This method clears the list of old values and ensures that any changes made
   * to the environment are reverted, maintaining consistency.
   *
   * @return A reference to the current Environment object after the restore operation.
   */
  Environment& restore();
  /**
   * @brief Sets a value in the container associated with the specified key.
   *
   * @param key The key to associate with the specified value.
   * @param value The value to set for the specified key.
   *
   * @return True if the set operation is successful, false otherwise.
   */
  Environment& set(const std::string&, const std::string&);
  /**
   * @brief Unsets a value in the container associated with the specified key.
   *
   * @param key The key to unset in the container.
   *
   * @return True if the unset operation is successful, false otherwise.
   */
  Environment& unSet(const std::string&);
  /**
   * @brief Appends a string value to the existing value of the specified environment variable.
   *
   * This method adds the provided value to the current value associated with the given
   * environment variable, effectively concatenating the new value to the existing one.
   *
   * @param index The name of the environment variable to which the value will be appended.
   * @param value The value to be appended to the specified environment variable.
   * @return A reference to the current Environment object, allowing method chaining.
   */
  Environment& append(const std::string&, const std::string&);
  /**
   * @brief Prepends the specified value to the value of the given environment variable.
   *
   * This method takes the current value of the environment variable identified by the
   * index and prepends the given value to it. If the environment variable does not
   * exist, the function will create it with the specified value as its initial content.
   *
   * @param index The name of the environment variable to be updated.
   * @param value The value to prepend to the current value of the specified environment variable.
   * @return A reference to the updated Environment object.
   */
  Environment& prepend(const std::string&, const std::string&);
  /**
   * @brief Retrieves the value associated with a given index from the environment.
   * If the index does not exist, the provided default value is returned.
   *
   * @param index The key to look up in the environment.
   * @param default_value The value to return if the key does not exist.
   * @return The value corresponding to the specified index, or the default value if the index is not found.
   */
  static std::string get(const std::string& index, const std::string& default_value = "");
  /**
   * @brief Checks whether the specified key exists in the environment.
   *
   * This method verifies if a given key is set or defined in the environment.
   *
   * @param index The key to check for existence in the environment.
   * @return True if the key exists, false otherwise.
   */
  static bool hasKey(const std::string&);
  /**
   * @brief Commits changes to the environment, saving the current state and capturing
   *        any added or modified environment variables.
   * @ingroup ElementsKernel
   */
  void commit();
  /**
   * @brief Enum for representing types of shell environments
   * @ingroup ElementsKernel
   */
  enum ShellType { sh, csh };

  /**
   * @brief Generates a shell script to set or unset environment variables.
   *
   * This method creates a script containing commands to update the environment
   * variables according to the specified shell type. It reflects the changes
   * in variables, such as additions, modifications, or unsets, based on the
   * internal state of the environment.
   *
   * @param type The type of shell for which the script should be generated.
   *             Supported shell types (e.g., sh, csh) determine the script syntax.
   * @return A string containing the generated shell script.
   */
  std::string generateScript(const ShellType) const;

private:
  /**
   * @brief check that the variable is in the environment
   * @ingroup ElementsKernel
   */
  static void checkOutOfRange(const std::string&);

  /// old value for changed variables
  std::map<std::string, std::string> m_old_values;

  bool m_keep_same;

  /// variable added to the environment
  std::vector<std::string> m_added_variables;
};

/**
 *
 */
class Environment::Variable {

public:
  Variable() = delete;
  Variable(Environment& env, std::string index);
  Variable(const Variable& other);
  Variable(Variable&& other);
  ~Variable() = default;
  Variable&          operator=(const Variable& other);
  Variable&          operator=(Variable&& other);
  Variable&          operator=(const std::string&);
  Variable&          set(const std::string&);
  Variable&          unSet();
  Variable&          append(const std::string&);
  Variable&          operator+=(const std::string&);
  Variable&          prepend(const std::string&);
  Variable           operator+(const std::string&) const;
  const std::string& index() const;
  Environment&       env() const;
  std::string        value() const;
  /// to string converter
  operator std::string() const;
  bool empty() const;
  bool exists() const;

private:
  void checkCompatibility(const Variable&) const;

  /// a copiable and movable reference
  std::reference_wrapper<Environment> m_env;

  /// The Name of the variable
  std::string m_index;
};

ELEMENTS_API std::ostream& operator<<(std::ostream&, const Environment::Variable&);

ELEMENTS_API Environment::Variable operator+(const std::string&, const Environment::Variable&);

}  // namespace Elements

#endif  // ELEMENTSKERNEL_ELEMENTSKERNEL_ENVIRONMENT_H_

/**@}*/
