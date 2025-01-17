/**
 * @file DependencyConfigurationPublic.cpp
 *
 * @date Jan 16, 2019
 * @author Antoine Basset
 *
 * @copyright 2019
 *
 */

#include "ElementsServices/DataSync/DependencyConfiguration.h"

#include "DependencyConfigurationPublic.h"

namespace DataSync = ElementsServices::DataSync;
using DataSync::path;

DependencyConfigurationPublic::DependencyConfigurationPublic(const path& distant_root, const path& local_root,
                                                             const path& config_file)
    : DataSync::DependencyConfiguration(distant_root, local_root, config_file) {}
