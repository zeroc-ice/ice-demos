' **********************************************************************
'
' Copyright (c) 2003-2016 ZeroC, Inc. All rights reserved.
'
' **********************************************************************

Module MinimalS

    Public Sub Main(ByVal args() As String)
        Try
            Dim communicator As Ice.Communicator = Ice.Util.initialize(args)
            Dim adapter As Ice.ObjectAdapter = communicator.createObjectAdapterWithEndpoints("Hello", "default -h localhost -p 10000")
            adapter.add(New HelloI, communicator.stringToIdentity("hello"))
            adapter.activate()
            communicator.waitForShutdown()
            communicator.destroy()
        Catch ex As System.Exception
            System.Console.Error.WriteLine(ex)
            System.Environment.Exit(-1)
        End Try
    End Sub

End Module
