"""
@file: ElementsKernel/AddScript.py
@author: Hubert Degaudenzi

@date: 03/08/17

This script creates a new Elements module

@copyright: 2012-2020 Euclid Science Ground Segment

This library is free software; you can redistribute it and/or modify it under
the terms of the GNU Lesser General Public License as published by the Free
Software Foundation; either version 3.0 of the License, or (at your option)
any later version.

This library is distributed in the hope that it will be useful, but WITHOUT
ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more
details.

You should have received a copy of the GNU Lesser General Public License
along with this library; if not, write to the Free Software Foundation, Inc.,
51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA

"""

import argparse
import os
import time
import ElementsKernel.ProjectCommonRoutines as epcr
import ElementsKernel.NameCheck as nc
import ElementsKernel.ParseCmakeLists as pcl
import ElementsKernel.Logging as log

logger = log.getLogger('AddScript')

# Define constants
CMAKE_LISTS_FILE = 'CMakeLists.txt'
PROGRAM_TEMPLATE_FILE = 'Script_template'
PROGRAM_TEMPLATE_FILE_IN = 'Script_template.in'

################################################################################

def createDirectories(module_dir, module_name):
    """
    Create directories needed for a python program
    """

    # Create the scripts directory
    scripts_path = os.path.join(module_dir, 'scripts')
    epcr.makeDirectory(scripts_path)

################################################################################

def subStringsInScriptFile(file_path, program_name, module_name):
    """
    Substitute variables in the python template file and rename it
    """
    template_file = os.path.join(file_path, PROGRAM_TEMPLATE_FILE)
    os.rename(os.path.join(file_path, PROGRAM_TEMPLATE_FILE_IN), template_file)

    # Substitute strings in h_template_file
    f = open(template_file, 'r')
    data = f.read()
    # Format all dependent projects
    # We put by default Elements dependency if no one is given
    date_str = time.strftime("%x")
    author_str = epcr.getAuthor()
    # Make some substitutions
    file_name_str = os.path.join('scripts', program_name)
    new_data = data % {"FILE": file_name_str,
                       "DATE": date_str,
                       "AUTHOR": author_str,
                       "PROGRAMNAME": program_name}

    f.close()

    # Save new data
    file_name = template_file.replace(PROGRAM_TEMPLATE_FILE, program_name)
    f = open(file_name, 'w')
    f.write(new_data)
    f.close()
    os.remove(template_file)

################################################################################

def updateCmakeListsFile(module_dir, program_name):
    """
    Update the <CMakeList.txt> file
    """
    logger.info('Updating the <%s> file', CMAKE_LISTS_FILE)
    cmake_filename = os.path.join(module_dir, CMAKE_LISTS_FILE)

    # Backup the file
    epcr.makeACopy(cmake_filename)

    # Cmake file already exist
    if os.path.isfile(cmake_filename):
        f = open(cmake_filename, 'r')
        data = f.read()
        f.close()
        cmake_object = pcl.CMakeLists(data)

        cmake_object.elements_install_scripts = 'elements_install_scripts()'

    # Write new data
    f = open(cmake_filename, 'w')
    f.write(str(cmake_object))
    f.close()

################################################################################

def createScript(current_dir, module_name, program_name):
    """
    Create the python program
    """
    createDirectories(current_dir, module_name)
    program_path = os.path.join(current_dir, 'scripts')
    script_goes_on = epcr.copyAuxFile(program_path, PROGRAM_TEMPLATE_FILE_IN)
    if script_goes_on:
        subStringsInScriptFile(program_path, program_name, module_name)
        updateCmakeListsFile(current_dir, program_name)

    return script_goes_on

################################################################################

def defineSpecificProgramOptions():
    """
    Define program option(s)
    """
    description = """
    This script creates an <Elements> python program at your current directory
    (default), this directory must be an <Elements> module.
           """
    parser = argparse.ArgumentParser(description=description)
    parser.add_argument('program_name', metavar='program-name',
                        type=str,
                        help='Program name')

    return parser

################################################################################

def mainMethod(args):
    """
    Main
    """

    logger.info('#')
    logger.info('#  Logging from the mainMethod() of the AddScript script')
    logger.info('#')

    program_name = args.program_name

    # Default is the current directory
    current_dir = os.getcwd()

    logger.info('# Current directory : %s', current_dir)
    logger.info('')

    # We absolutely need a Elements cmake file
    script_goes_on, module_name = epcr.isElementsModuleExist(current_dir)

    # Module as no version number, '1.0' is just for using the routine
    if script_goes_on:
        script_goes_on = epcr.isNameAndVersionValid(program_name, '1.0')

    # Check aux file exist
    if script_goes_on:
        script_goes_on = epcr.isAuxFileExist(PROGRAM_TEMPLATE_FILE_IN)

    program_file_path = os.path.join(current_dir, 'scripts', program_name)

    # Check name in the Element Naming Database
    if script_goes_on:
        script_goes_on = epcr.checkNameInEuclidNamingDatabase(program_name, nc.TYPES[2])

    # Make sure the program does not already exist
    if script_goes_on:
        script_goes_on = epcr.isFileAlreadyExist(program_file_path, program_name)

    if script_goes_on:
        if os.path.exists(current_dir):
            if createScript(current_dir, module_name, program_name):
                logger.info('< %s > program successfully created in < %s >.',
                            program_name, program_file_path)
                # Remove backup file
                epcr.deleteFile(os.path.join(current_dir, CMAKE_LISTS_FILE) + '~')
                logger.info('Script over.')
        else:
            logger.error('< %s > project directory does not exist!', current_dir)

    if not script_goes_on:
        logger.error('Script aborted!')