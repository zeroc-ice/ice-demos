#!/bin/sh
#
# Copyright (c) ZeroC, Inc. All rights reserved.
#

if [ -f /usr/local/opt/ice/bin/slice2swift ]; then
    SLICE2SWIFT=/usr/local/bin/slice2swift
    SLICEDIR=/usr/local/ice/slice
else
    SLICE2SWIFT=$ICE_HOME/cpp/bin/slice2swift
    SLICEDIR=$ICE_HOME/slice
fi

$SLICE2SWIFT -I"$SLICEDIR" -I"$INPUT_FILE_DIR" --output-dir "$DERIVED_FILE_DIR" "$INPUT_FILE_PATH"
