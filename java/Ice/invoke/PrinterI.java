//
// Copyright (c) ZeroC, Inc. All rights reserved.
//

import com.zeroc.demos.Ice.invoke.Demo.*;

public class PrinterI implements com.zeroc.Ice.Blobject
{
    @Override
    public com.zeroc.Ice.Object.Ice_invokeResult ice_invoke(byte[] inParams, com.zeroc.Ice.Current current)
    {
        com.zeroc.Ice.Communicator communicator = current.adapter.getCommunicator();

        com.zeroc.Ice.InputStream in = new com.zeroc.Ice.InputStream(communicator, inParams);
        in.startEncapsulation();

        com.zeroc.Ice.Object.Ice_invokeResult r = new com.zeroc.Ice.Object.Ice_invokeResult();
        r.returnValue = true;

        if(current.operation.equals("printString"))
        {
            String message = in.readString();
            System.out.println("Printing string `" + message + "'");
        }
        else if(current.operation.equals("printStringSequence"))
        {
            String[] seq = in.readStringSeq();
            System.out.print("Printing string sequence {");
            for(int i = 0; i < seq.length; ++i)
            {
                if(i > 0)
                {
                    System.out.print(", ");
                }
                System.out.print("'" + seq[i] + "'");
            }
            System.out.println("}");
        }
        else if(current.operation.equals("printDictionary"))
        {
            java.util.Map<String, String> dict = StringDictHelper.read(in);
            System.out.print("Printing dictionary {");
            boolean first = true;
            for(java.util.Map.Entry<String, String> i : dict.entrySet())
            {
                if(!first)
                {
                    System.out.print(", ");
                }
                first = false;
                System.out.print(i.getKey() + "=" + i.getValue());
            }
            System.out.println("}");
        }
        else if(current.operation.equals("printEnum"))
        {
            Color c = Color.ice_read(in);
            System.out.println("Printing enum " + c);
        }
        else if(current.operation.equals("printStruct"))
        {
            Structure s = Structure.ice_read(in);
            System.out.println("Printing struct: name=" + s.name + ", value=" + s.value);
        }
        else if(current.operation.equals("printStructSequence"))
        {
            Structure[] seq = StructureSeqHelper.read(in);
            System.out.print("Printing struct sequence: {");
            for(int i = 0; i < seq.length; ++i)
            {
                if(i > 0)
                {
                    System.out.print(", ");
                }
                System.out.print(seq[i].name + "=" + seq[i].value);
            }
            System.out.println("}");
        }
        else if(current.operation.equals("printClass"))
        {
            class Holder
            {
                C obj;
            }
            final Holder h = new Holder();
            in.readValue(v -> h.obj = (C)v);
            in.readPendingValues();
            System.out.println("Printing class: s.name=" + h.obj.s.name + ", s.value=" + h.obj.s.value);
        }
        else if(current.operation.equals("getValues"))
        {
            C c = new C();
            c.s = new Structure();
            c.s.name = "green";
            c.s.value = Color.green;
            com.zeroc.Ice.OutputStream out = new com.zeroc.Ice.OutputStream(communicator);
            out.startEncapsulation();
            out.writeValue(c);
            out.writeString("hello");
            out.writePendingValues();
            out.endEncapsulation();
            r.outParams = out.finished();
        }
        else if(current.operation.equals("throwPrintFailure"))
        {
            System.out.println("Throwing PrintFailure");
            PrintFailure ex = new PrintFailure();
            ex.reason = "paper tray empty";
            com.zeroc.Ice.OutputStream out = new com.zeroc.Ice.OutputStream(communicator);
            out.startEncapsulation();
            out.writeException(ex);
            out.endEncapsulation();
            r.outParams = out.finished();
            r.returnValue = false;
        }
        else if(current.operation.equals("shutdown"))
        {
            current.adapter.getCommunicator().shutdown();
        }
        else
        {
            com.zeroc.Ice.OperationNotExistException ex = new com.zeroc.Ice.OperationNotExistException();
            ex.id = current.id;
            ex.facet = current.facet;
            ex.operation = current.operation;
            throw ex;
        }

        //
        // Verify we read all in parameters
        //
        in.endEncapsulation();
        return r;
    }
}
