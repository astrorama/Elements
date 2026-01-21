/**
 * @file ConfigFilesFixture.cpp
 *
 * @date Jan 16, 2019
 * @author Antoine Basset
 *
 * @copyright 2019
 *
 */

#include "ConfigFilesFixture.h"

#include <vector>  // for vector

#include "ElementsServices/DataSync/DataSyncUtils.h"  // for path, localWorkspacePrefix, DEFAULT_WORKDIR_VAR

using ElementsServices::DataSync::DEFAULT_WORKDIR_VAR;
using ElementsServices::DataSync::localWorkspacePrefix;
using ElementsServices::DataSync::path;

WorkspaceFixture::WorkspaceFixture() {
  m_env[DEFAULT_WORKDIR_VAR] = (m_top_dir.path() / "workspace").string();
}

path theDependencyConfig() {
  return {"ElementsServices/testdata/test_file_list.txt"};
}

std::vector<path> theLocalFiles() {
  return std::vector(
      {path("file1.txt"), path("file2.txt"), path("dir/file3.txt"), path("dir/file4.txt"), path("file5.txt")});
}

path theLocalWorkspace() {
  return {"/tmp"};
}

path thePrefixedLocalWorkspace() {
  return localWorkspacePrefix() / theLocalWorkspace();
}

path absPath(const path& relPath) {
  return thePrefixedLocalWorkspace() / relPath;
}

path theInvalidDependencyConfig() {
  return {"ElementsServices/testdata/invalid_test_file_list.txt"};
}

path theIrodsFrConfig() {
  return {"ElementsServices/testdata/sync_fr_irods.conf"};
}

path theWebdavFrConfig() {
  return {"ElementsServices/testdata/sync_fr_webdav.conf"};
}

path theWebdavFrDistantWorkspace() {
  return {"/euclid-fr/ct/mock_test_data"};
}

path theWebdavEsConfig() {
  return {"ElementsServices/testdata/sync_es_webdav.conf"};
}

path theNoOverwriteConfig() {
  return {"ElementsServices/testdata/sync_fr_webdav_nooverwrite.conf"};
}

path aBadConnectionConfig() {
  return {"ElementsServices/testdata/sync_wrong.conf"};
}
