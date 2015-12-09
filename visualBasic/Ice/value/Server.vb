' **********************************************************************
'
' Copyright (c) 2003-2015 ZeroC, Inc. All rights reserved.
'
' **********************************************************************

Module ValueS
    Class Server
        Inherits Ice.Application

        Public Overloads Overrides Function run(ByVal args() As String) As Integer
            If args.Length > 0 Then
                Console.Error.WriteLine(appName() & ": too many arguments")
                Return 1
            End If

            Dim factory As Ice.ObjectFactory = New ObjectFactory
            communicator().addValueFactory(factory, Demo.Printer.ice_staticId())

            Dim adapter As Ice.ObjectAdapter = communicator().createObjectAdapter("Value")
            adapter.add(New InitialI(adapter), communicator().stringToIdentity("initial"))
            adapter.activate()
            communicator().waitForShutdown()
            Return 0
        End Function

    End Class

    Public Sub Main(ByVal args() As String)
        Dim app As Server = New Server
        Dim status As Integer = app.main(args, "config.server")
        System.Environment.Exit(status)
    End Sub

End Module
