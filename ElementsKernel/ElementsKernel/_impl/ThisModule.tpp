/**
 * @file ElementsKernel/_impl/ThisModule.tpp
 * @brief implementation of the templates declared in ElementsKernel/ThisModule.h
 * @date 2023-12-13
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
 */

// IWYU pragma: private, include "ElementsKernel/ThisModule.h"
#pragma once

#ifndef ELEMENTSKERNEL_ELEMENTSKERNEL_THIS_MODULE_IMPL_
#error "This file should not be included directly! Use ElementsKernel/ThisModule.h instead"
#else

#include "ElementsKernel/FuncPtrCast.h"  // for FuncPtrCast
#include "ElementsKernel/ModuleInfo.h"   // for ModuleInfo

namespace Elements::System {

inline const ModuleInfo& getThisModuleInfo() {
  static ModuleInfo this_module;
  if (this_module.isEmpty()) {
    this_module = ModuleInfo(FuncPtrCast<void*>(getThisModuleInfo));
  }

  return this_module;
}

}  // namespace Elements::System

#endif  // ELEMENTSKERNEL_ELEMENTSKERNEL_THIS_MODULE_IMPL_
