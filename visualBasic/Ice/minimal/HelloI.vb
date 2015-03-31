' **********************************************************************
'
' Copyright (c) 2003-2014 ZeroC, Inc. All rights reserved.
'
' **********************************************************************

Imports Demo

Public NotInheritable Class HelloI
    Inherits HelloDisp_

    Public Overloads Overrides sub sayHello(ByVal current As Ice.Current)
        System.Console.Out.WriteLine("Hello World!")
    End Sub
End Class
