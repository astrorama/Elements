# Copyright (C) 2012-2024, UNIGE and contributors (for the Euclid Science Ground Segment)
# This file is part of Elements <https://gitlab.euclid-sgs.uk/ST-TOOLS/Elements>
# SPDX-License-Identifier: LGPL-3.0-or-later

'''
@file ElementsKernel.File
@brief Provide basic utilities for file handling
@date 2024-02-13
@author Hubert Degaudenzi
'''

__updated__ = "2024-02-13"

import sys

DEFAULT_FILESYSTEM_ENCODING = sys.getfilesystemencoding()


def nativeOpen(*args, **kwargs):
    """ write to the open command to provide the
        call without having to specify the encoding. It
        uses the default filesystem encoding, like the
        original implementation of the open command.
    """

    binary_mode = False
    explicit_encoding = False

    open_mode = "r"

    if len(args) >= 2:
        open_mode = args[1]

    if "mode" in kwargs:
        open_mode = kwargs["mode"]

    if "b" in open_mode:
        binary_mode = True

    if len(args) >= 4 or "encoding" in kwargs:
        explicit_encoding = True

    if not binary_mode and not explicit_encoding:
        kwargs["encoding"] = DEFAULT_FILESYSTEM_ENCODING

    return open(*args, **kwargs)  # pylint: disable=unspecified-encoding
