################################################################################
#
# Generic Makefile to simplify the use of CMake projects
# ------------------------------------------------------
#
# This simple Makefile is meant to provide a simplified entry point for the
# configuration and build of CMake-based projects that use a default toolchain
# (as it is the case for Elements-based projects).
#
# Only a few targets are actually provided: all the main targets are directly
# delegated to the CMake Makefile.
#
# :Author: Hubert Degaudenzi
################################################################################

# Default

.DELETE_ON_ERROR:

# settings
CMAKE := cmake
CTEST := ctest
NINJA := $(shell which ninja-build 2> /dev/null)
ifeq ($(NINJA),)
  NINJA := $(shell which ninja 2> /dev/null)
endif

# Looking for the Custom make library

CUSTOM_MAKE_LIB := Custom.mk

ifneq ($(wildcard $(CURDIR)/make/$(CUSTOM_MAKE_LIB)),)
  CUSTOM_MAKE_LIB_FILE := $(CURDIR)/make/$(CUSTOM_MAKE_LIB)
else
  ifneq ($(CMAKE_PREFIX_PATH),)
    PREFIX_LIST := $(subst :, ,$(CMAKE_PREFIX_PATH))
    CUSTOM_MAKE_LIB_LIST := $(foreach dir,$(PREFIX_LIST),$(wildcard $(dir)/share/Elements/make/$(CUSTOM_MAKE_LIB) $(dir)/../make/$(CUSTOM_MAKE_LIB)))
  endif
  CUSTOM_MAKE_LIB_LIST += /usr/share/Elements/make/$(CUSTOM_MAKE_LIB)
  CUSTOM_MAKE_LIB_FILE := $(firstword $(CUSTOM_MAKE_LIB_LIST))
endif

# Looking for the ToolChain

TOOLCHAIN_NAME := ElementsToolChain.cmake

ifneq ($(wildcard $(CURDIR)/cmake/$(TOOLCHAIN_NAME)),)
  TOOLCHAIN_FILE := $(CURDIR)/cmake/$(TOOLCHAIN_NAME)
else
  ifneq ($(CMAKE_PREFIX_PATH),)
    PREFIX_LIST := $(subst :, ,$(CMAKE_PREFIX_PATH))
    TOOLCHAIN_LIST := $(foreach dir,$(PREFIX_LIST),$(wildcard $(dir)/lib*/cmake/ElementsProject/$(TOOLCHAIN_NAME) $(dir)/$(TOOLCHAIN_NAME)))
    TOOLCHAIN_FILE := $(firstword $(TOOLCHAIN_LIST))
  endif
endif

override ALL_CMAKEFLAGS := --no-warn-unused-cli

ifneq ($(TOOLCHAIN_FILE),)
  # A toolchain has been found. Lets use it.
  override ALL_CMAKEFLAGS += -DCMAKE_TOOLCHAIN_FILE=$(TOOLCHAIN_FILE)
endif


BUILD_PREFIX_NAME := build

override ALL_CMAKEFLAGS += -DUSE_LOCAL_INSTALLAREA=ON -DBUILD_PREFIX_NAME:STRING=$(BUILD_PREFIX_NAME)
override ALL_CMAKEFLAGS += -DUSE_VERSIONED_LIBRARIES=OFF

ifndef BINARY_TAG
  ifdef CMAKECONFIG
    BINARY_TAG := ${CMAKECONFIG}
  else
    ifdef CMTCONFIG
      BINARY_TAG := ${CMTCONFIG}
    endif
  endif
endif

ifdef BINARY_TAG
  BUILD_SUBDIR := $(BUILD_PREFIX_NAME).$(BINARY_TAG)
else
  BUILD_SUBDIR := $(BUILD_PREFIX_NAME)
endif
BUILDDIR := $(CURDIR)/$(BUILD_SUBDIR)

# build tool

ifneq ($(USE_NINJA),)
  # enable Ninja
  override ALL_CMAKEFLAGS += -GNinja
  BUILD_CONF_FILE := build.ninja
  BUILDFLAGS := $(NINJAFLAGS)
  ifneq ($(VERBOSE),)
    BUILDFLAGS := -v $(BUILDFLAGS)
  endif
else
  BUILD_CONF_FILE := Makefile
endif
BUILD_CMD := $(CMAKE) --build $(BUILD_SUBDIR) --target


# Use environment variable for extra flags

# Replace the ":" from eclipse variable list to spaces
ifneq ($(EXPAND_FLAGS),)
  CMAKEFLAGS := $(subst :-, -,$(CMAKEFLAGS))
endif

ifneq ($(CMAKEFLAGS),)
  override ALL_CMAKEFLAGS += $(CMAKEFLAGS)
endif

# default target
all: ## (default) build everything

# deep clean
purge: ## deep clean of the build, including InstallArea (requires re-configuration)
	$(RM) -r $(BUILDDIR) $(CURDIR)/InstallArea/$(BINARY_TAG)
	find $(CURDIR) "(" -name "InstallArea" -prune -o -name "*.pyc" -o -name "*.pyo" ")" -a -type f -exec $(RM) -v \{} \;
	find $(CURDIR) -depth -type d -name "__pycache__" -exec $(RM) -rv \{} \;

# Remove all the possible directories and the whole InstallArea as well
mrproper: ## Remove all build directories (requires re-configuration)
	$(RM) -r $(CURDIR)/build $(CURDIR)/build.* $(CURDIR)/InstallArea
	find $(CURDIR) "(" -name "*.pyc" -o -name "*.pyo" ")" -a -type f -exec $(RM) -v \{} \;
	find $(CURDIR) -depth -type d -name "__pycache__" -exec $(RM) -rv \{} \;

# delegate any target to the build directory (except 'purge')
ifneq ($(MAKECMDGOALS),purge)
ifneq ($(MAKECMDGOALS),mrproper)
%: $(BUILDDIR)/$(BUILD_CONF_FILE) FORCE
	+$(BUILD_CMD) $* -- $(BUILDFLAGS)
endif
endif

# aliases
.PHONY: configure tests FORCE
ifneq ($(wildcard $(BUILDDIR)/$(BUILD_CONF_FILE)),)
configure: rebuild_cache ## Alias to CMake 'rebuild_cache' target
else
configure: $(BUILDDIR)/$(BUILD_CONF_FILE)
endif
	@ # do not delegate further

ifndef CTEST_ARGS
  CTEST_ARGS = -T test --output-junit $(BUILDDIR)/Testing/JUnitTestReport.xml $(CTEST_EXTRA_ARGS)
endif

# This wrapping around the test target is used to ensure the generation of
# the XML output from ctest.
test: $(BUILDDIR)/$(BUILD_CONF_FILE) ## Run the tests
	$(RM) -r $(BUILDDIR)/Testing $(BUILDDIR)/html
	-cd $(BUILDDIR) && $(CTEST) $(CTEST_ARGS) $(ARGS)
	+$(BUILD_CMD) JUnitSummary


# This target ensures that the "all" target is called before
# running the tests (unlike the "test" default target of CMake)
tests: all ## build all and run the tests
	$(RM) -r $(BUILDDIR)/Testing $(BUILDDIR)/html
	-cd $(BUILDDIR) && $(CTEST) $(CTEST_ARGS) $(ARGS)
	+$(BUILD_CMD) JUnitSummary

ifeq ($(VERBOSE),)
# less verbose install
# (emulate the default CMake install target)
install: all ## Populate the InstallArea with the products of the build
	cd $(BUILDDIR) && $(CMAKE) -P cmake_install.cmake | grep -v "^-- Up-to-date:"
endif

# import the library to look for a custom Makefile
-include $(CUSTOM_MAKE_LIB_FILE)

# ensure that the target are always passed to the CMake Makefile
FORCE: ;

# Makefiles are used as implicit targets in make, but we should not consider
# them for delegation.
$(MAKEFILE_LIST): ;


# trigger CMake configuration
$(BUILDDIR)/$(BUILD_CONF_FILE): | $(BUILDDIR) ## Run CMake
	cd $(BUILDDIR) && $(CMAKE) $(ALL_CMAKEFLAGS) $(CURDIR)

$(BUILDDIR): ## Create the build directory
	mkdir -p $(BUILDDIR)

help: ## This help
	@awk 'BEGIN {FS = ":.*?## "} /^[a-zA-Z_-]+:.*?## / {printf "\033[36m%-30s\033[0m %s\n", $$1, $$2}' $(MAKEFILE_LIST)
