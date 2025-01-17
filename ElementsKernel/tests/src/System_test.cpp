/**
 * @file BackTrace_test.cpp
 *
 * @date Aug 27, 2015
 * @author hubert
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

#include "ElementsKernel/System.h"

#include <sys/utsname.h>  // for uname, utsname

#include <boost/test/unit_test.hpp>  // for BOOST_TEST

#include <string>  // for string, operator!=

#include "ElementsKernel/Environment.h"  // for Environment

using std::string;

namespace Elements {

template <class T>
struct X {};

BOOST_AUTO_TEST_SUITE(System_test)

//-----------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE(HostName_test) {

  using std::string;

  const string DELIMITER{"."};

  if (Environment current; current["HOSTNAME"].exists()) {
    string        full_var_hostname = current["HOSTNAME"];
    const string  var_hostname      = full_var_hostname.substr(0, full_var_hostname.find(DELIMITER));
    const string& full_sys_hostname = System::hostName();
    const string  sys_hostname      = full_sys_hostname.substr(0, full_sys_hostname.find(DELIMITER));
    BOOST_CHECK_EQUAL(var_hostname, sys_hostname);
  }
}

BOOST_AUTO_TEST_CASE(osName_test) {

  string  osname{"UNKNOWN"};
  utsname ut{};
  if (::uname(&ut) == 0) {
    osname = ut.sysname;
  }

  BOOST_CHECK_EQUAL(System::osName(), osname);
}

BOOST_AUTO_TEST_CASE(osVersion_test) {

  string  os_version{"UNKNOWN"};
  utsname ut{};
  if (uname(&ut) == 0) {
    os_version = ut.release;
  }

  BOOST_CHECK_EQUAL(System::osVersion(), os_version);
}

BOOST_AUTO_TEST_CASE(typenameInfo_test) {

  BOOST_CHECK_EQUAL(System::typeinfoName(typeid(void).name()), "void");
  BOOST_CHECK_EQUAL(System::typeinfoName(typeid(void)), "void");

  BOOST_CHECK_EQUAL(System::typeinfoName(typeid(wchar_t).name()), "wchar_t");
  BOOST_CHECK_EQUAL(System::typeinfoName(typeid(wchar_t)), "wchar_t");

  BOOST_CHECK_EQUAL(System::typeinfoName(typeid(bool).name()), "bool");
  BOOST_CHECK_EQUAL(System::typeinfoName(typeid(bool)), "bool");

  BOOST_CHECK_EQUAL(System::typeinfoName(typeid(char).name()), "char");
  BOOST_CHECK_EQUAL(System::typeinfoName(typeid(char)), "char");

  BOOST_CHECK_EQUAL(System::typeinfoName(typeid(signed char).name()), "signed char");
  BOOST_CHECK_EQUAL(System::typeinfoName(typeid(signed char)), "signed char");

  BOOST_CHECK_EQUAL(System::typeinfoName(typeid(unsigned char).name()), "unsigned char");
  BOOST_CHECK_EQUAL(System::typeinfoName(typeid(unsigned char)), "unsigned char");

  BOOST_CHECK_EQUAL(System::typeinfoName(typeid(short).name()), "short");
  BOOST_CHECK_EQUAL(System::typeinfoName(typeid(short)), "short");

  BOOST_CHECK_EQUAL(System::typeinfoName(typeid(unsigned short).name()), "unsigned short");
  BOOST_CHECK_EQUAL(System::typeinfoName(typeid(unsigned short)), "unsigned short");

  BOOST_CHECK_EQUAL(System::typeinfoName(typeid(int).name()), "int");
  BOOST_CHECK_EQUAL(System::typeinfoName(typeid(int)), "int");

  BOOST_CHECK_EQUAL(System::typeinfoName(typeid(unsigned int).name()), "unsigned int");
  BOOST_CHECK_EQUAL(System::typeinfoName(typeid(unsigned int)), "unsigned int");

  BOOST_CHECK_EQUAL(System::typeinfoName(typeid(long).name()), "long");
  BOOST_CHECK_EQUAL(System::typeinfoName(typeid(long)), "long");

  BOOST_CHECK_EQUAL(System::typeinfoName(typeid(unsigned long).name()), "unsigned long");
  BOOST_CHECK_EQUAL(System::typeinfoName(typeid(unsigned long)), "unsigned long");

  BOOST_CHECK_EQUAL(System::typeinfoName(typeid(long long).name()), "long long");
  BOOST_CHECK_EQUAL(System::typeinfoName(typeid(long long)), "long long");

  BOOST_CHECK_EQUAL(System::typeinfoName(typeid(unsigned long long).name()), "unsigned long long");
  BOOST_CHECK_EQUAL(System::typeinfoName(typeid(unsigned long long)), "unsigned long long");

  BOOST_CHECK_EQUAL(System::typeinfoName(typeid(float).name()), "float");
  BOOST_CHECK_EQUAL(System::typeinfoName(typeid(float)), "float");

  BOOST_CHECK_EQUAL(System::typeinfoName(typeid(double).name()), "double");
  BOOST_CHECK_EQUAL(System::typeinfoName(typeid(double)), "double");

  BOOST_CHECK_EQUAL(System::typeinfoName(typeid(long double).name()), "long double");
  BOOST_CHECK_EQUAL(System::typeinfoName(typeid(long double)), "long double");

  BOOST_CHECK_EQUAL(System::typeinfoName(typeid(__float128).name()), "__float128");
  BOOST_CHECK_EQUAL(System::typeinfoName(typeid(__float128)), "__float128");

  BOOST_CHECK_EQUAL(System::typeinfoName(typeid(X<int>).name()), "Elements::X<int>");
  BOOST_CHECK_EQUAL(System::typeinfoName(typeid(X<int>)), "Elements::X<int>");
}

//-----------------------------------------------------------------------------

BOOST_AUTO_TEST_SUITE_END()

}  // namespace Elements
