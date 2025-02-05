/**
 * @file Environment.cpp
 * @brief Implementation of the Environment functions
 * @date Jun 17, 2016
 * @author Hubert Degaudenzi
 *
 * @copyright 2012-2020 Euclid Science Ground Segment
 *
 * This library is free software; you can redistribute it and/or modify it under the terms of the GNU Lesser General
 * Public Licence as published by the Free Software Foundation; either version 3.0 of the Licence, or (at your option)
 * any later version.
 *
 * This library is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public Licence for more
 * details.
 *
 * You should have received a copy of the GNU Lesser General Public Licence along with this library; if not, write to
 * the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
 *
 */

#include "ElementsKernel/Environment.h"

#include <algorithm>  // for find
#include <map>        // for map
#include <sstream>    // for stringstream
#include <stdexcept>  // for out_of_range
#include <string>     // for string
#include <utility>    // for move

#include <boost/format.hpp>  // for format

#include "ElementsKernel/System.h"  // for getEnv, setEnv, isEnvSet

using std::endl;
using std::ostream;
using std::string;
using std::stringstream;

namespace Elements {

using System::getEnv;
using System::isEnvSet;
using System::setEnv;
using System::unSetEnv;

Environment::Variable::Variable(Environment& env, string index) : m_env{env}, m_index{std::move(index)} {}

Environment::Variable::Variable(const Variable& other) : m_env{other.m_env} {
  checkCompatibility(other);
}

Environment::Variable::Variable(Variable&& other) : m_env{other.m_env} {  // NOLINT(*-noexcept-move-constructor)
  checkCompatibility(other);
}

Environment::Variable& Environment::Variable::operator=(const Variable& other) {
  checkCompatibility(other);

  m_env = other.m_env;

  return *this;
}

Environment::Variable& Environment::Variable::operator=(Variable&& other) {  // NOLINT(*-noexcept-move-constructor)
  checkCompatibility(other);

  m_env = other.m_env;

  return *this;
}

Environment::Variable& Environment::Variable::operator=(const string& arg_value) {

  set(arg_value);

  return *this;
}

Environment::Variable& Environment::Variable::set(const string& arg_value) {

  m_env.get().set(m_index, arg_value);

  return *this;
}

Environment::Variable& Environment::Variable::unSet() {

  m_env.get().unSet(m_index);

  return *this;
}

Environment::Variable& Environment::Variable::append(const string& arg_value) {

  m_env.get().append(m_index, arg_value);

  return *this;
}

Environment::Variable& Environment::Variable::operator+=(const string& arg_value) {

  return append(arg_value);
}

Environment::Variable& Environment::Variable::prepend(const string& arg_value) {

  m_env.get().prepend(m_index, arg_value);

  return *this;
}

Environment::Variable Environment::Variable::operator+(const string& arg_value) const {

  Variable result(m_env, m_index);

  result.append(arg_value);

  return result;
}

const string& Environment::Variable::index() const {
  return m_index;
}

Environment& Environment::Variable::env() const {
  return m_env;
}

// ReSharper disable once CppDFAConstantFunctionResult
string Environment::Variable::value() const {

  return get(m_index, "");
}

// ReSharper disable once CppDFAConstantFunctionResult
Environment::Variable::operator std::string() const {
  return value();
}

bool Environment::Variable::empty() const {
  return value().empty();
}

bool Environment::Variable::exists() const {
  return hasKey(m_index);
}

void Environment::Variable::checkCompatibility(const Variable& other) const {

  if (m_index != other.m_index) {
    stringstream error_buffer;
    error_buffer << "The \"" << other.m_index << "\" environment variable"
                 << " cannot be copied to the \"" << m_index << "\" environment variable." << endl;
    throw std::invalid_argument(error_buffer.str());
  }
}

//----------------------------------------------------------------------------

Environment::Environment(const bool keep_same) : m_keep_same{keep_same} {}

Environment& Environment::restore() {
  for (const auto& v : m_added_variables) {
    unSetEnv(v);
  }

  for (const auto& [key, value] : m_old_values) {
    setEnv(key, value);
  }

  m_old_values = {};

  return *this;
}

Environment::~Environment() {
  restore();
}

Environment::Variable Environment::operator[](const string& index) {
  return {*this, index};
}

Environment::Variable Environment::operator[](const string& index) const {
  return {const_cast<Environment&>(*this), index};
}

Environment& Environment::set(const string& key, const string& value) {

  if (m_old_values.find(key) == m_old_values.end()) {
    if (hasKey(key)) {
      if (not m_keep_same || getEnv(key) != value) {
        m_old_values[key] = getEnv(key);
      }
    } else {
      m_added_variables.emplace_back(key);
    }
  }

  setEnv(key, value);

  return *this;
}

Environment& Environment::unSet(const string& index) {

  checkOutOfRange(index);

  if (m_old_values.find(index) == m_old_values.end()) {
    if (const auto found_index = std::find(m_added_variables.begin(), m_added_variables.end(), index);
        found_index != m_added_variables.end()) {
      m_added_variables.erase(found_index);
    } else {
      m_old_values[index] = getEnv(index);
    }
  }

  unSetEnv(index);

  return *this;
}

Environment& Environment::append(const string& index, const string& value) {

  const string new_value = get(index) + value;

  set(index, new_value);

  return *this;
}

Environment& Environment::prepend(const string& index, const string& value) {

  const string new_value = value + get(index);

  set(index, new_value);

  return *this;
}

string Environment::get(const string& index, const string& default_value) {
  string value{default_value};

  if (hasKey(index)) {
    value = getEnv(index);
  }

  return value;
}

bool Environment::hasKey(const string& index) {

  return isEnvSet(index);
}

void Environment::commit() {

  m_old_values      = {};
  m_added_variables = {};
}

string Environment::generateScript(const ShellType type) const {

  using boost::format;
  using std::map;

  stringstream script_text{};

  map<ShellType, string> set_cmd{{sh, "export %s=%s"}, {csh, "setenv %s %s"}};
  map<ShellType, string> unset_cmd{{sh, "unset %s"}, {csh, "unsetenv %s"}};

  for (const auto& [key, value] : m_old_values) {
    if (hasKey(key)) {
      script_text << format(set_cmd[type]) % key % get(key) << endl;
    } else {
      script_text << format(unset_cmd[type]) % key << endl;
    }
  }

  for (const auto& v : m_added_variables) {
    script_text << format(set_cmd[type]) % v % get(v) << endl;
  }

  return script_text.str();
}

void Environment::checkOutOfRange(const string& index) {

  if (not hasKey(index)) {
    stringstream error_buffer;
    error_buffer << "The environment doesn't contain the " << index << " variable." << endl;
    throw std::out_of_range(error_buffer.str());
  }
}

ostream& operator<<(ostream& stream, const Environment::Variable& v) {

  stream << v.value();

  return stream;
}

Environment::Variable operator+(const string& value, const Environment::Variable& other) {

  Environment::Variable result(other.env(), other.index());

  result.prepend(value);

  return result;
}

}  // namespace Elements
