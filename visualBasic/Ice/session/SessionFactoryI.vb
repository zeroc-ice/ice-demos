' **********************************************************************
'
' Copyright (c) 2003-2017 ZeroC, Inc. All rights reserved.
'
' **********************************************************************

Imports Demo
Imports System

Public Class SessionFactoryI
    Inherits SessionFactoryDisp_

    Public Overloads Overrides Function create(ByVal name As String, ByVal current As Ice.Current) As SessionPrx

        Dim session As New SessionI(name)
        Dim proxy As SessionPrx = SessionPrxHelper.uncheckedCast(current.adapter.addWithUUID(session))

        '
        ' Remove endpoints to ensure that calls are collocated-only
        ' This way, if we invoke on the proxy during shutdown, the invocation fails immediately
        ' without attempting to establish any connection
        '
        Dim collocProxy As SessionPrx = SessionPrxHelper.uncheckedCast(proxy.ice_endpoints(New Ice.Endpoint() {}))

        '
        ' Never close this connection from the client and turn on heartbeats with a timeout of 30s
        '
        current.con.setACM(30, Ice.ACMClose.CloseOff, Ice.ACMHeartbeat.HeartbeatAlways)
        current.con.setCallback(New CloseCallbackI(collocProxy))

        Return proxy
    End Function

    Public Overloads Overrides Sub shutdown(ByVal current As Ice.Current)
        Console.Out.WriteLine("Shutting down...")
        current.adapter.getCommunicator().shutdown()
    End Sub

    Class CloseCallbackI
        Implements Ice.ConnectionCallback

        Sub New(ByVal session As SessionPrx)
            _session = session
        End Sub

        Public Sub heartbeat(ByVal con As Ice.Connection) Implements Ice.ConnectionCallback.heartbeat
            ' ignored
        End Sub

        Public Sub closed(ByVal con As Ice.Connection) Implements Ice.ConnectionCallback.closed
            Try
                _session.destroy()
                Console.WriteLine("Cleaned up dead client.")
            Catch ex As Ice.LocalException
                ' The client already destroyed this session, or the server is shutting down
            End Try
        End Sub

        Private _session As SessionPrx
    End Class

End Class
