// **********************************************************************
//
// Copyright (c) 2003-2017 ZeroC, Inc. All rights reserved.
//
// **********************************************************************

#pragma once

module Demo
{
    enum MatrixType { RowMajor, ColumnMajor }
    
    sequence<double> DoubleSeq;

    interface Matrix
    {
        DoubleSeq fetchData(out MatrixType type, out int axisLength);
    }
}
