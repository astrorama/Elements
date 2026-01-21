/**
 * @file Storage.h
 *
 * @date 2014-12-22
 * @author Hubert Degaudenzi
 *
 * @copyright 2012-2023 Euclid Science Ground Segment
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
 */

/**
 * @addtogroup ElementsKernel ElementsKernel
 * @{
 */

#ifndef ELEMENTSKERNEL_ELEMENTSKERNEL_STORAGE_H_
#define ELEMENTSKERNEL_ELEMENTSKERNEL_STORAGE_H_

#include <cstddef>  // for size_t
#include <cstdint>  // for int64_t
#include <map>      // for map
#include <string>   // for string

#include "ElementsKernel/Export.h"

namespace Elements {
inline namespace Kernel {
namespace Units {

/**
 * @enum StorageType
 * @brief Represents various units of digital storage capacity.
 *
 * This enum class provides constants to represent binary and metric-based storage
 * units. Binary units are represented as Byte, KiloByte, MegaByte, etc., while
 * metric units are prefixed with "Metric" such as MetricKiloByte, MetricMegaByte, etc.
 *
 * Binary-based units use powers of 1024, and metric-based units use powers of 1000.
 */
enum class StorageType {
  Byte,
  KiloByte,
  MegaByte,
  GigaByte,
  TeraByte,
  PetaByte,
  MetricKiloByte,
  MetricMegaByte,
  MetricGigaByte,
  MetricTeraByte,
  MetricPetaByte
};

/**
 * @var StorageShortName
 * @brief Maps storage unit types to their respective short name representations.
 *
 * This mapping associates each value of the StorageType enumeration with a
 * standardized short name that corresponds to the unit. The short names follow
 * common conventions for both binary-based (e.g., "KiB", "MiB") and metric-based
 * (e.g., "KB", "MB") units of digital storage. Binary units use IEC standards
 * (kibi-, mebi-, gibi-, etc.), while metric units follow standard SI prefixes.
 */
ELEMENTS_API extern std::map<StorageType, std::string> StorageShortName;

/**
 * @var StorageFactor
 * @brief Maps storage types to their respective scaling factors.
 *
 * This map provides a numerical correspondence between different storage units and
 * their respective scaling factors. Values for binary units (e.g., Byte, KiloByte,
 * MegaByte) are calculated using powers of 2, while values for metric units
 * (e.g., MetricKiloByte, MetricMegaByte) are calculated using powers of 10.
 *
 * The scaling factors are used to convert storage measurements between various
 * units, ensuring accurate calculations and conversions. Binary scaling factors
 * align with powers of 1024, while metric scaling factors align with powers of 1000.
 */
ELEMENTS_API extern std::map<StorageType, std::int64_t> StorageFactor;

/**
 * @brief Rounds a given value to a specified maximum number of digits.
 *
 * This function adjusts the input value to have at most the specified number
 * of significant digits after the decimal point. It supports both positive
 * and negative values.
 *
 * @tparam T The type of the input value. Typically a floating-point type.
 * @param value The numeric value to be rounded.
 * @param max_digits The maximum number of significant digits to retain.
 * @return The value rounded to the specified number of digits.
 */
template <typename T>
ELEMENTS_API T roundToDigits(const T& value, const std::size_t& max_digits);

/// @brief explicit instantiation for double and float:
extern template ELEMENTS_API double roundToDigits<double>(const double& value, const std::size_t& max_digits);
extern template ELEMENTS_API float  roundToDigits<float>(const float& value, const std::size_t& max_digits);

/**
 * @brief Converts a size value from one storage unit to another.
 *
 * This function converts a given size value from a source storage unit to a target storage unit,
 * using the appropriate scaling factor. It supports both binary-based (powers of 1024) and metric-based
 * (powers of 1000) units as defined in the StorageType enumeration.
 *
 * @tparam T The numeric data type of the size value, typically a floating-point type.
 * @param size The size value to be converted.
 * @param source_unit The original storage unit of the size value.
 * @param target_unit The target storage unit to which the size value will be converted.
 * @return The converted size value in the target storage unit.
 */
template <std::size_t max_digits, typename T>
ELEMENTS_API T storageConvert(const T& size, StorageType source_unit, StorageType target_unit);

/**
 * @brief Converts a storage size from one unit to another.
 *
 * This function performs a conversion of a storage size from the specified source unit
 * to the specified target unit. It accounts for both binary and metric-based storage units.
 *
 * @tparam T The type of the size value to be converted, typically a numeric type.
 * @param size The storage size to be converted.
 * @param source_unit The unit of the input storage size, specified as a StorageType.
 * @param target_unit The unit to which the storage size should be converted, specified as a StorageType.
 * @return The converted storage size in the target unit as a value of type T.
 */
template <typename T>
ELEMENTS_API T storageConvert(const T& size, StorageType source_unit, StorageType target_unit);

/// @brief explicit instantiation for double, float, and int64_t:
extern template ELEMENTS_API double storageConvert<double>(const double& size, StorageType source_unit,
                                                           StorageType target_unit);
extern template ELEMENTS_API float  storageConvert<float>(const float& size, StorageType source_unit,
                                                          StorageType target_unit);
extern template ELEMENTS_API        std::int64_t
storageConvert<std::int64_t>(const std::int64_t& size, StorageType source_unit, StorageType target_unit);

}  // namespace Units
}  // namespace Kernel
}  // namespace Elements

#define ELEMENTSKERNEL_ELEMENTSKERNEL_STORAGE_IMPL_
#include "ElementsKernel/_impl/Storage.tpp"  // IWYU pragma: export
#undef ELEMENTSKERNEL_ELEMENTSKERNEL_STORAGE_IMPL_

#endif  // ELEMENTSKERNEL_ELEMENTSKERNEL_STORAGE_H_

/**@}*/
