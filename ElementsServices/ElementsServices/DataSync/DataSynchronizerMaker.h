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

/**
 * @addtogroup ElementsServices ElementsServices
 * @{
 */

#ifndef ELEMENTSSERVICES_ELEMENTSSERVICES_DATASYNC_DATASYNCHRONIZERMAKER_H_
#define ELEMENTSSERVICES_ELEMENTSSERVICES_DATASYNC_DATASYNCHRONIZERMAKER_H_

#include <memory>  // for shared_ptr

#include "ElementsKernel/Export.h"  // for ELEMENTS_API

#include "ElementsServices/DataSync/ConnectionConfiguration.h"  // for ConnectionConfiguration
#include "ElementsServices/DataSync/DataSyncUtils.h"            // for Services
#include "ElementsServices/DataSync/DataSynchronizer.h"         // for DataSynchronizer
#include "ElementsServices/DataSync/DependencyConfiguration.h"  // for DependencyConfiguration

namespace Elements {
inline namespace Services {
namespace DataSync {

ELEMENTS_API std::shared_ptr<DataSynchronizer> createSynchronizer(ConnectionConfiguration connection,
                                                                  DependencyConfiguration dependency);

}  // namespace DataSync
}  // namespace Services
}  // namespace Elements

namespace ElementsServices = Elements::Services;

#endif  // ELEMENTSSERVICES_ELEMENTSSERVICES_DATASYNC_DATASYNCHRONIZERMAKER_H_

/**@}*/
