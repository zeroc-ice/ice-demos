' **********************************************************************
'
' Copyright (c) 2003-2017 ZeroC, Inc. All rights reserved.
'
' **********************************************************************

Imports Demo
Imports System
Imports System.Threading
Imports System.Collections

Module SessionC
    Class Client
        Inherits Ice.Application

        Public Overloads Overrides Function run(ByVal args() As String) As Integer
            If args.Length > 0 Then
                Console.Error.WriteLine(appName() & ": too many arguments")
                Return 1
            End If

            Dim name As String = Nothing
            Do
                Console.Out.Write("Please enter your name ==> ")
                Console.Out.Flush()

                name = Console.In.ReadLine()
                If name Is Nothing Then
                    Return 1
                End If
                name = name.Trim()
            Loop While name.Length = 0

            Dim properties As Ice.Properties = communicator().getProperties()
            Dim proxyProperty As String = "SessionFactory.Proxy"
            Dim proxy As String = properties.getProperty(proxyProperty)
            If proxy.Length = 0 Then
                Console.Error.WriteLine("property `" + proxyProperty + "' not set")
                Return 1
            End If

            Dim basePrx As Ice.ObjectPrx = communicator().propertyToProxy("SessionFactory.Proxy")
            Dim factory As SessionFactoryPrx = SessionFactoryPrxHelper.checkedCast(basePrx)
            If factory Is Nothing Then
                Console.Error.WriteLine("invalid proxy")
                Return 1
            End If

            Dim session As SessionPrx = factory.create(name)

            Dim hellos As New ArrayList

            menu()

            Dim destroy As Boolean = True
            Dim shutdown As Boolean = False
            While True
                Console.Out.Write("==> ")
                Console.Out.Flush()
                Dim line As String = Console.In.ReadLine()
                If line Is Nothing Then
                    Exit While
                End If
                If line.Length > 0 AndAlso Char.IsDigit(line.Chars(0)) Then
                    Dim index As Int32 = Int32.Parse(line)
                    If index < hellos.Count Then
                        Dim hello As Hello = hellos.Item(index)
                        hello.sayHello()
                    Else
                        Console.Out.WriteLine("Index is too high. " & hellos.Count & " hello objects exist so far.")
                        Console.Out.WriteLine("Use `c' to create a new hello object.")
                    End If
                ElseIf line.Equals("c") Then
                    hellos.Add(session.createHello())
                    Console.Out.WriteLine("Created hello object " & (hellos.Count - 1))
                ElseIf line.Equals("s") Then
                    destroy = False
                    shutdown = True
                    Exit While
                ElseIf line.Equals("x") Then
                    Exit While
                ElseIf line.Equals("t") Then
                    destroy = False
                    Exit While
                ElseIf line.Equals("?") Then
                    menu()
                Else
                    Console.Out.WriteLine("Unknown command `" + line + "'.")
                    menu()
                End If
            End While

            If destroy Then
                session.destroy()
            End If
            If shutdown Then
                factory.shutdown()
            End If

            Return 0
        End Function

        Private Sub menu()
            Console.Out.WriteLine("usage:")
            Console.Out.WriteLine("c:     create a new per-client hello object")
            Console.Out.WriteLine("0-9:   send a greeting to a hello object")
            Console.Out.WriteLine("s:     shutdown the server and exit")
            Console.Out.WriteLine("x:     exit")
            Console.Out.WriteLine("t:     exit without destroying the session")
            Console.Out.WriteLine("?:     help")
        End Sub

    End Class

    Public Sub Main(ByVal args() As String)
        Dim app As New Client
        Dim status As Integer = app.main(args, "config.client")
        System.Environment.Exit(status)
    End Sub

End Module
