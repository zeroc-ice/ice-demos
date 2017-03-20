' **********************************************************************
'
' Copyright (c) 2003-2017 ZeroC, Inc. All rights reserved.
'
' **********************************************************************

Imports Demo
Imports System
Imports System.Collections.Generic
Imports System.Diagnostics

Public Class SessionFactoryI
    Inherits SessionFactoryDisp_

    Public Overloads Overrides Function create(ByVal name As String, ByVal current As Ice.Current) As SessionPrx

        Dim session As New SessionI(name)
        Dim proxy As SessionPrx = SessionPrxHelper.uncheckedCast(current.adapter.addWithUUID(session))

        SyncLock Me
            '
            ' With this demo, the connection cannot have an old session associated with it
            '
            Debug.Assert(Not _connectionMap.ContainsKey(current.con))
            _connectionMap(current.con) = proxy
        End SyncLock

        '
        ' Never close this connection from the client and turn on heartbeats with a timeout of 30s
        '
        current.con.setACM(30, Ice.ACMClose.CloseOff, Ice.ACMHeartbeat.HeartbeatAlways)
        current.con.setCallback(New ClosedCallbackI(Me))

        Return proxy
    End Function

    Public Overloads Overrides Sub shutdown(ByVal current As Ice.Current)
        Console.Out.WriteLine("Shutting down...")
        current.adapter.getCommunicator().shutdown()
    End Sub

    Sub deadClient(ByVal con as Ice.Connection)
        Dim session As SessionPrx = Nothing
        SyncLock Me
            If(_connectionMap.ContainsKey(con)) Then
                session = _connectionMap(con)
                _connectionMap.Remove(con)
            End If
        End SyncLock
        If Not session Is Nothing Then
            Try
                session.destroy()
                Console.WriteLine("Cleaned up dead client.")
            Catch ex as Ice.ObjectNotExistException
                ' The client already destroyed this session
            Catch ex as Ice.ConnectionRefusedException
                ' Server is shutting down
            End Try
        End If
    End Sub

    Class ClosedCallbackI
        Implements Ice.ConnectionCallback

        Sub New(ByVal sessionFactory as SessionFactoryI)
            _sessionFactory = sessionFactory
        End Sub

        Public Sub heartbeat(Byval con As Ice.Connection) Implements Ice.ConnectionCallback.heartbeat
            ' ignored
        End Sub

        Public Sub closed(ByVal con As Ice.Connection) Implements Ice.ConnectionCallback.closed
            _sessionFactory.deadClient(con)
        End Sub

        Private _sessionFactory As SessionFactoryI
    End Class

    Private _connectionMap As New Dictionary(Of Ice.Connection, SessionPrx)

End Class
