/**
 * @file Path_test.cpp
 *
 * Created on: Dec 4, 2013
 *     Author: Pierre Dubath
 */

#include "ElementsKernel/Path.h"          // header to test

#include <string>                         // for std::string
#include <vector>                         // for std::vector
#include <algorithm>                      // for for_each, transform

#include <boost/test/unit_test.hpp>       // for boost unit test macros
#include <boost/filesystem.hpp>           // for boost::filesystem
#include <boost/filesystem/fstream.hpp>   // for ofstream

#include <ElementsKernel/Temporary.h>     // for TempDir

using std::string;
using std::vector;

using boost::filesystem::path;
using boost::filesystem::exists;
using boost::filesystem::is_regular;

//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
//
// Begin of the Boost tests
//
//-----------------------------------------------------------------------------

struct Path_Fixture {

  Elements::TempDir m_top_dir;
  vector<path> m_item_list;
  vector<path> m_file_list;

  static void create_test_file(const path& p) {

    boost::filesystem::ofstream f{p.string()};
    f << "test text" << std::endl;
    f.close();

  }

  Path_Fixture(): m_top_dir{ "Path_test-%%%%%%%" } {

    using std::for_each;
    using boost::filesystem::create_directory;

    m_item_list.push_back(m_top_dir.path() / "test1");
    m_item_list.push_back(m_top_dir.path() / "test1" / "foo");
    m_item_list.push_back(m_top_dir.path() / "test2");
    m_item_list.push_back(m_top_dir.path() / "test3");
    m_item_list.push_back(m_top_dir.path() / "test4");
    m_item_list.push_back(m_top_dir.path() / "test5");
    m_item_list.push_back(m_top_dir.path() / "test6");

    for_each(m_item_list.cbegin(), m_item_list.cend(),
        [](path p) {
        create_directory(p);
    });


    m_file_list.push_back(m_top_dir.path() / "test1" / "foo" / "e1e2");
    m_file_list.push_back(m_top_dir.path() / "test3" / "e1e2");
    m_file_list.push_back(m_top_dir.path() / "test4" / "e1e2");

    create_directory(m_top_dir.path() / "test1" / "sub");
    create_directory(m_top_dir.path() / "test5" / "sub");

    m_file_list.push_back(m_top_dir.path() / "test1" / "sub" / "d1d2");
    m_file_list.push_back(m_top_dir.path() / "test5" / "sub" / "d1d2");
    m_file_list.push_back(m_top_dir.path() / "test6" / "d1d2");

    for_each(m_file_list.cbegin(), m_file_list.cend(),
        [](path p) {
        create_test_file(p);
    });

  }

  ~Path_Fixture() {

  }

};

BOOST_AUTO_TEST_SUITE(Path_test)

//-----------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE(PathConstructor_test) {

  path test_path {"toto/titi"};

  BOOST_CHECK(test_path.is_relative());
  BOOST_CHECK(test_path.filename() == "titi");
  BOOST_CHECK(test_path.parent_path() == "toto");

  string test_str {"toto/tutu"};
  path test_path2 { test_str };

  BOOST_CHECK(not test_path2.empty());

  BOOST_CHECK(test_path2.is_relative());
  BOOST_CHECK(test_path2.string() == test_path2);

  path test_path3 {};
  BOOST_CHECK(test_path3.empty());

  BOOST_CHECK(test_path3.string() == "");
  BOOST_CHECK(test_path3.filename() == "");
  BOOST_CHECK(test_path3.parent_path() == "");

}

BOOST_FIXTURE_TEST_CASE(getFromLocations_test, Path_Fixture) {

  using Elements::Path::getPathFromLocations;

  path that_file = getPathFromLocations("Bla", m_item_list);

  BOOST_CHECK(that_file.filename() == "");

  path that_dir = getPathFromLocations("foo", m_item_list);
  BOOST_CHECK(that_dir.filename() != "");
  BOOST_CHECK(exists(that_dir));

  path f_path {m_top_dir.path() / "test2" / "bar"};

  create_test_file(f_path);

  path that_file2 = getPathFromLocations("bar", m_item_list);
  BOOST_CHECK(that_file2.filename() != "");
  BOOST_CHECK(exists(that_file2));
  BOOST_CHECK(is_regular_file(that_file2));

}

BOOST_FIXTURE_TEST_CASE(getAllFromLocations_test, Path_Fixture) {

  using Elements::Path::getAllPathFromLocations;

  vector<path> file_list = getAllPathFromLocations("e1e2", m_item_list);

  BOOST_CHECK(file_list.size() == 3);

  vector<path> ref_file_list;
  ref_file_list.push_back(m_top_dir.path() / "test1" / "foo" / "e1e2");
  ref_file_list.push_back(m_top_dir.path() / "test3" / "e1e2");
  ref_file_list.push_back(m_top_dir.path() / "test4" / "e1e2");

  BOOST_CHECK_EQUAL_COLLECTIONS(file_list.cbegin(), file_list.cend(),
                                ref_file_list.cbegin(), ref_file_list.cend());


  vector<path> file_list2 = getAllPathFromLocations("sub/d1d2", m_item_list);

  BOOST_CHECK(file_list2.size() == 2);

  vector<path> ref_file_list2;
  ref_file_list2.push_back(m_top_dir.path() / "test1" / "sub" / "d1d2");
  ref_file_list2.push_back(m_top_dir.path() / "test5" / "sub" / "d1d2");

  BOOST_CHECK_EQUAL_COLLECTIONS(file_list2.cbegin(), file_list2.cend(),
                                ref_file_list2.cbegin(), ref_file_list2.cend());

}


BOOST_FIXTURE_TEST_CASE(getFromStringLocations_test, Path_Fixture) {

  using Elements::Path::getPathFromLocations;
  using std::transform;

  vector<string> str_item_list(m_item_list.size());

  transform(m_item_list.cbegin(), m_item_list.cend(),
      str_item_list.begin(),
      [](path p){
      return p.string();
  });

  path that_file = getPathFromLocations("Bla", str_item_list);

  BOOST_CHECK(that_file.filename() == "");

  path that_dir = getPathFromLocations("foo", str_item_list);
  BOOST_CHECK(that_dir.filename() != "");
  BOOST_CHECK(exists(that_dir));

  path f_path {m_top_dir.path() / "test2" / "bar"};

  create_test_file(f_path);

  path that_file2 = getPathFromLocations("bar", str_item_list);
  BOOST_CHECK(that_file2.filename() != "");
  BOOST_CHECK(exists(that_file2));
  BOOST_CHECK(is_regular_file(that_file2));

}


BOOST_AUTO_TEST_CASE(JoinPath_test) {

  using Elements::Path::joinPath;

  const vector<string> path_list {"/toto", "titi", "./tutu"};

  BOOST_CHECK(joinPath(path_list) == "/toto:titi:./tutu");

  const vector<string> path_list2 {"", "/toto", "titi", "./tutu"};

  BOOST_CHECK(joinPath(path_list2) == ":/toto:titi:./tutu");

  const vector<string> path_list3 {"/toto", "titi", "./tutu", ""};

  BOOST_CHECK(joinPath(path_list3) == "/toto:titi:./tutu:");

}

BOOST_AUTO_TEST_CASE(MultiPathAppend_test) {

  using Elements::Path::multiPathAppend;

  const vector<string> locations {"loc1","/loc2","./loc3"};
  const vector<string> suffixes {"bin", "scripts"};

  const vector<string> ref_paths {"loc1/bin", "loc1/scripts",
                                   "/loc2/bin", "/loc2/scripts",
                                   "./loc3/bin", "./loc3/scripts"};

  const vector<path> full_paths = multiPathAppend(locations, suffixes);

  BOOST_CHECK(full_paths.size() == 6);

  vector<string> full_path_strings(full_paths.size());

  std::transform(full_paths.cbegin(), full_paths.cend(),
                 full_path_strings.begin(),
                 [](path p){
                   return p.string();
                 });

  BOOST_CHECK(ref_paths == full_path_strings);

}


BOOST_AUTO_TEST_SUITE_END()

//-----------------------------------------------------------------------------
//
// End of the Boost tests
//
//-----------------------------------------------------------------------------
