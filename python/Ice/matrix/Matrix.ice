// **********************************************************************
//
// Copyright (c) 2003-present ZeroC, Inc. All rights reserved.
//
// **********************************************************************

#pragma once

module Demo
{
    enum MatrixType { RowMajor, ColumnMajor }

    sequence<double> DoubleSeq;

    struct MatrixData
    {
        DoubleSeq elements;
        MatrixType type;
        int axisLength;
    }

    interface Matrix
    {
        MatrixData fetchData();
    }
}
