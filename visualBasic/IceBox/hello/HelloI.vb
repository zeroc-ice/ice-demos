' **********************************************************************
'
' Copyright (c) 2003-2017 ZeroC, Inc. All rights reserved.
'
' **********************************************************************

Imports Demo

Public Class HelloI
    Inherits HelloDisp_

    Public Overloads Overrides Sub sayHello(ByVal current As Ice.Current)
        System.Console.Out.WriteLine("Hello World!")
    End Sub
End Class
