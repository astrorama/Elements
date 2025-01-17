/**
 * @file MockDataSynchronizer.cpp
 *
 * @date Jan 16, 2019
 * @author Antoine Basset
 *
 * @copyright 2019
 *
 */

#ifndef ELEMENTSSERVICES_TESTS_SRC_DATASYNC_FIXTURES_MOCKDATASYNCHRONIZER_H_
#define ELEMENTSSERVICES_TESTS_SRC_DATASYNC_FIXTURES_MOCKDATASYNCHRONIZER_H_

#include <map>     // for map
#include <string>  // for string

#include "ElementsServices/DataSync/DataSyncUtils.h"     // for path
#include "ElementsServices/DataSync/DataSynchronizer.h"  // for DataSynchronizer

#include "ConfigFilesFixture.h"  // for theDependencyConfig, theWebdavFrConfig, WorkspaceFixture

struct MockDataSynchronizer : ElementsServices::DataSync::DataSynchronizer, WorkspaceFixture {

  virtual ~MockDataSynchronizer() = default;

  MockDataSynchronizer(const ElementsServices::DataSync::path& connection = theWebdavFrConfig(),
                       const ElementsServices::DataSync::path& dependency = theDependencyConfig());

  std::string createDownloadCommand(ElementsServices::DataSync::path distantFile,
                                    ElementsServices::DataSync::path localFile) const override;

  std::map<ElementsServices::DataSync::path, ElementsServices::DataSync::path> fileMap();
};

#endif  // ELEMENTSSERVICES_TESTS_SRC_DATASYNC_FIXTURES_MOCKDATASYNCHRONIZER_H_
