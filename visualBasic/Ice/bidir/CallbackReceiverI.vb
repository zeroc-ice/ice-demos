' **********************************************************************
'
' Copyright (c) 2003-2015 ZeroC, Inc. All rights reserved.
'
' **********************************************************************

Imports BidirDemo

Public NotInheritable Class CallbackReceiverI
    Inherits CallbackReceiverDisp_

    Public Overloads Overrides Sub callback(ByVal num As Integer, ByVal current As Ice.Current)
        System.Console.Out.WriteLine("received callback #" & num)
    End Sub

End Class
