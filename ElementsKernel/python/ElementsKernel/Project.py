"""
@file ElementsKernel/Project.py
@author Nicolas Morisset

@date 01/07/15

This script will create a new Elements project

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
import ELEMENTS_VERSION  # @UnresolvedImport
import ElementsKernel.ProjectCommonRoutines as epcr
import ElementsKernel.NameCheck as nc
import ElementsKernel.Logging as log

logger = log.getLogger('CreateElementsProject')

AUX_CMAKE_LIST_IN = "CMakeLists.txt.in"
AUX_CMAKE_FILE_IN = "Makefile.in"
AUX_PROJ_RST_IN = "doc_project.rst.in"

################################################################################

def setPath():
    """
    Set the installation path either the current directory or
    the directory set by the <User_area> environment variable
    """
    # Check if User_area environment variable exists
    user_area = os.environ.get('User_area')
    if not user_area is None:
        logger.debug('# <$User_area> environment variable defined to : <%s>', user_area)
        destination_path = user_area
    else:
        destination_path = os.getcwd()

    return destination_path

################################################################################

def isDependencyProjectValid(str_list):
    """
    Check if the dependency project name and version list is valid
    """
    valid = True
    for i in range(len(str_list)):
        if not epcr.isNameAndVersionValid(str_list[i][0], str_list[i][1]):
            valid = False
            break

    return valid

################################################################################

def duplicate_elements(duplicate_list):
    """
    Look for duplicate element in a list
    """
    name_list = []
    not_found_duplicate = True
    for elt in duplicate_list:
        if not elt[0] in name_list:
            name_list.append(elt[0])
        else:
            logger.error('# Found twice the following dependency : <%s>, script aborted', elt[0])
            not_found_duplicate = False
            break

    return not_found_duplicate

################################################################################

def getElementsVersion():
    """
    Get the Elements version number
    """
    patch_version = ELEMENTS_VERSION.ELEMENTS_PATCH_VERSION
    version = [str(ELEMENTS_VERSION.ELEMENTS_MAJOR_VERSION),
               str(ELEMENTS_VERSION.ELEMENTS_MINOR_VERSION)]
    version_patch = [str(ELEMENTS_VERSION.ELEMENTS_MAJOR_VERSION),
                     str(ELEMENTS_VERSION.ELEMENTS_MINOR_VERSION),
                     str(patch_version)]

    elt_version = '.'.join(version)
    if patch_version > 0:
        elt_version = '.'.join(version_patch)

    logger.info('# Elements version found : <%s>', elt_version)

    return str(elt_version)

################################################################################

def substituteProjectVariables(project_dir, proj_name, proj_version, dep_projects):
    """
    Substitute variables in <CMakeList.txt.in> file and rename the file to
    <CMakeLists.txt>.
    """
    logger.debug('# Substitute variables in <%s> file', AUX_CMAKE_LIST_IN)
    cmake_list_file = os.path.join(project_dir, AUX_CMAKE_LIST_IN)

    # Substitute
    f = open(cmake_list_file, 'r')
    data = f.read()

    # Format all dependent projects
    # We put by default Elements dependency if no one is given
    str_dep_projects = 'Elements ' + getElementsVersion()
    if not dep_projects is None:
        for dep in dep_projects:
            if not dep[0] in str_dep_projects:
                str_dep_projects += ' ' + dep[0] + ' ' + dep[1]
            else:
                logger.warning('<%s> dependency already exists. It is skipped!', dep[0])
        new_data = data % {"PROJECT_NAME": proj_name,
                           "PROJECT_VERSION": proj_version,
                           "DEPENDANCE_LIST": str_dep_projects}
    else:
        # Add an comment example with the USE keyword

        new_data = data % {"PROJECT_NAME": proj_name,
                           "PROJECT_VERSION": proj_version,
                           "DEPENDANCE_LIST": str_dep_projects}

    f.close()

    # Save new data
    f = open(cmake_list_file, 'w')
    f.write(new_data)
    f.close()
    # Remove '.in'
    os.rename(cmake_list_file, cmake_list_file.replace('.in', ''))

################################################################################

def createProject(project_dir, proj_name, proj_version, dep_projects):
    """
    Create the project structure
    """
    logger.info('# Creating the project')

    # Create project
    os.makedirs(project_dir)

    epcr.copyAuxFile(project_dir, AUX_CMAKE_LIST_IN)
    epcr.copyAuxFile(project_dir, AUX_CMAKE_FILE_IN)
    # Remove '.in'
    cmakefile = os.path.join(project_dir, AUX_CMAKE_FILE_IN)
    os.rename(cmakefile, cmakefile.replace('.in', ''))

    project_doc_dir = os.path.join(project_dir, "doc")
    os.makedirs(project_doc_dir)
    epcr.copyAuxFile(project_doc_dir, AUX_PROJ_RST_IN)
    project_rst_file = os.path.join(project_doc_dir, AUX_PROJ_RST_IN)
    os.rename(project_rst_file, project_rst_file.replace('.in', ''))


    substituteProjectVariables(project_dir, proj_name, proj_version, dep_projects)

################################################################################

def defineSpecificProgramOptions():
    """
    Define program option(s)
    """
    description = """
This script creates an <Elements> project in your current directory(by default)
or at the location defined by the <$User_area> environment variable, if defined.
It means that all the necessary structure (directory structure, makefiles etc...)
will be automatically created for you.

[-d]    Use this option if your project has some dependencies on other project(s).
[-novd] Use this option if you do not want to create a version directory
        e.g.
            > CreateElementsProject test_project 1.0
              This creates the following directories : "test_project/1.0"

            > CreateElementsProject test 1.0 -novd
              This creates the following directory : "test_project"

            The "test_project" project is created at your current directory or
            at the location pointed by the $User_area environment variable

            """

    from argparse import RawTextHelpFormatter

    parser = argparse.ArgumentParser(description=description,
                                     formatter_class=RawTextHelpFormatter)
    parser.add_argument('project_name', metavar='project-name', type=str,
                        help='Project name')
    parser.add_argument('project_version', metavar='project-version',
                        type=str, default='1.0', help='Project version number')
    parser.add_argument('-d', '--dependency', metavar=('project_name', 'version'),
                        nargs=2, action='append', type=str,
                        help='Dependency project name and its version number"\
                         e.g "-d Elements x.x.x"')
    parser.add_argument('-novd', '--no-version-directory', action="store_true",
                        help='Does not create the <project-version> directory only\
                        the <project-name> directory will be created')

    return parser

################################################################################

def mainMethod(args):
    """
    Main
    """
    logger.info('#')
    logger.info('#  Logging from the mainMethod() of the CreateElementsProject script')
    logger.info('#')

    proj_name = args.project_name
    proj_version = args.project_version
    dependant_projects = args.dependency
    destination_path = setPath()
    logger.info('# Installation directory : %s', destination_path)

    # Check project name and version
    script_goes_on = epcr.isNameAndVersionValid(proj_name, proj_version)

    # Check for duplicate dependencies
    if script_goes_on and not args.dependency is None:
        script_goes_on = duplicate_elements(dependant_projects)

    # Check AUX files exist
    if script_goes_on:
        script_goes_on = epcr.isAuxFileExist(AUX_CMAKE_LIST_IN)
    if script_goes_on:
        script_goes_on = epcr.isAuxFileExist(AUX_CMAKE_FILE_IN)
    
    # Check name in the Element Naming Database
    if script_goes_on:
        script_goes_on = epcr.checkNameInEuclidNamingDatabase(proj_name, nc.TYPES[0])

    # Set the project directory
    if args.no_version_directory:
        project_dir = os.path.join(destination_path, proj_name)
    else:
        project_dir = os.path.join(destination_path, proj_name, proj_version)

    # Make sure dependencies name and version are valid
    if script_goes_on and not args.dependency is None:
        script_goes_on = isDependencyProjectValid(dependant_projects)

    if script_goes_on and os.path.exists(project_dir):
        logger.warning('<%s> Project ALREADY exists!!!', project_dir)
        response_key = raw_input('Do you want to replace the existing '\
                                 'project and associated module(s) (Yes/No,'\
                                 ' default: No)?')
        if response_key == "YES" or response_key == "yes" or response_key == "y":
            logger.info('# Replacing the existing project: <%s>', project_dir)
            epcr.eraseDirectory(project_dir)
        else:
            script_goes_on = False
            logger.info('# Script stopped by user!')

    if script_goes_on:
        createProject(project_dir, proj_name, proj_version, dependant_projects)
        logger.info('# <%s> project successfully created.', project_dir)
        logger.info('# Script over.')
    else:
        logger.error('# Script aborted!')
