/**
 * @file OpenCV.cpp
 * @date 2015-12-04
 * @author Hubert Degaudenzi
 *
 * @copyright 2012-2025 Euclid Science Ground Segment
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
 */

#include <map>     // for map
#include <string>  // for allocator, string

#include <iostream>
#include <opencv2/opencv.hpp>

#include "ElementsKernel/Exception.h"  // for Exception"
#include "ElementsKernel/Main.h"       // for MAIN_FOR
#include "ElementsKernel/Program.h"    // for Program
#include "ElementsKernel/Unused.h"     // for ELEMENTS_UNUSED

using boost::program_options::value;
using std::map;
using std::string;

namespace Elements::Examples {

class OpenCV final : public Program {

public:
  OptionsDescription defineSpecificProgramOptions() override {

    OptionsDescription config_options{"OpenCV example program options"};
    auto               add = config_options.add_options();

    add("image-path", value<string>(), "A string option without default value");

    return config_options;
  }

  ExitCode mainMethod(ELEMENTS_UNUSED map<string, VariableValue>& args) override {

    const auto log = Logging::getLogger("OpenCV");

    if (args["image-path"].empty()) {
      log.error() << "usage: DisplayImage.out <Image_Path>";
      throw Exception("No image path provided", ExitCode::USAGE);
    }

    cv::Mat image;
    image = imread(args["image-path"].as<string>(), cv::IMREAD_COLOR);

    if (!image.data) {
      log.error() << "No image data";
      throw Exception("No image data", ExitCode::DATAERR);
    }

    namedWindow("Display Image", cv::WINDOW_AUTOSIZE);
    imshow("Display Image", image);

    cv::waitKey(0);

    return ExitCode::OK;
  }
};

}  // namespace Elements::Examples

/**
 * Implementation of a main using a base class macro
 * This must be present in all Elements programs
 */
MAIN_FOR(Elements::Examples::OpenCV)
