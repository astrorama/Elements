/**
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
 * @details All the compilation system dependent parts (\#if, \#ifdef etc)
 *   should be located in this file. This should clear the need of these
 *   entities in all other C++ files.
 */

#ifndef _ELEMENTSSERVICES_DATASYNC_DEPENDENCYCONFIGURATIONPUBLIC_H
#define _ELEMENTSSERVICES_DATASYNC_DEPENDENCYCONFIGURATIONPUBLIC_H

#include "ElementsServices/DataSync/DataSyncUtils.h"            // for path
#include "ElementsServices/DataSync/DependencyConfiguration.h"  // for DependencyConfiguration

class DependencyConfigurationPublic : public ElementsServices::DataSync::DependencyConfiguration {
public:
  virtual ~DependencyConfigurationPublic() = default;
  DependencyConfigurationPublic(const ElementsServices::DataSync::path& distant_root,
                                const ElementsServices::DataSync::path& local_root,
                                const ElementsServices::DataSync::path& config_file);
  using ElementsServices::DataSync::DependencyConfiguration::aliasSeparator;
  using ElementsServices::DataSync::DependencyConfiguration::lineHasAlias;
  using ElementsServices::DataSync::DependencyConfiguration::parseLineWithAlias;
  using ElementsServices::DataSync::DependencyConfiguration::parseLineWithoutAlias;
};

#endif
