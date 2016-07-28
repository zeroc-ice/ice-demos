// **********************************************************************
//
// Copyright (c) 2003-2016 ZeroC, Inc. All rights reserved.
//
// **********************************************************************

using Demo;
using System;

public class PrinterI : Ice.Blobject
{
    private class ReadValueCallback
    {
        public void invoke(Ice.Value obj)
        {
            this.obj = obj;
        }

        internal Ice.Value obj;
    }

    public override bool ice_invoke(byte[] inParams, out byte[] outParams, Ice.Current current)
    {
        outParams = null;

        var communicator = current.adapter.getCommunicator();

        Ice.InputStream inStream = null;
        if(inParams.Length > 0)
        {
            inStream = new Ice.InputStream(communicator, inParams);
            inStream.startEncapsulation();
        }

        if(current.operation.Equals("printString"))
        {
            var message = inStream.readString();
            inStream.endEncapsulation();
            Console.WriteLine("Printing string `" + message + "'");
            return true;
        }
        else if(current.operation.Equals("printStringSequence"))
        {
            var seq = StringSeqHelper.read(inStream);
            inStream.endEncapsulation();
            Console.Write("Printing string sequence {");
            for(int i = 0; i < seq.Length; ++i)
            {
                if(i > 0)
                {
                    Console.Write(", ");
                }
                Console.Write("'" + seq[i] + "'");
            }
            Console.WriteLine("}");
            return true;
        }
        else if(current.operation.Equals("printDictionary"))
        {
            var dict = StringDictHelper.read(inStream);
            inStream.endEncapsulation();
            Console.Write("Printing dictionary {");
            bool first = true;
            foreach(var e in dict)
            {
                if(!first)
                {
                    Console.Write(", ");
                }
                first = false;
                Console.Write(e.Key + "=" + e.Value);
            }
            Console.WriteLine("}");
            return true;
        }
        else if(current.operation.Equals("printEnum"))
        {
            var c = ColorHelper.read(inStream);
            inStream.endEncapsulation();
            Console.WriteLine("Printing enum " + c);
            return true;
        }
        else if(current.operation.Equals("printStruct"))
        {
            var s = Structure.read(inStream);
            inStream.endEncapsulation();
            Console.WriteLine("Printing struct: name=" + s.name + ", value=" + s.value);
            return true;
        }
        else if(current.operation.Equals("printStructSequence"))
        {
            var seq = StructureSeqHelper.read(inStream);
            inStream.endEncapsulation();
            Console.Write("Printing struct sequence: {");
            for(int i = 0; i < seq.Length; ++i)
            {
                if(i > 0)
                {
                    Console.Write(", ");
                }
                Console.Write(seq[i].name + "=" + seq[i].value);
            }
            Console.WriteLine("}");
            return true;
        }
        else if(current.operation.Equals("printClass"))
        {
            var cb = new ReadValueCallback();
            inStream.readValue(cb.invoke);
            inStream.readPendingValues();
            inStream.endEncapsulation();
            var c = cb.obj as C;
            Console.WriteLine("Printing class: s.name=" + c.s.name + ", s.value=" + c.s.value);
            return true;
        }
        else if(current.operation.Equals("getValues"))
        {
            var c = new C(new Structure("green", Color.green));
            var outStream = new Ice.OutputStream(communicator);
            outStream.startEncapsulation();
            outStream.writeValue(c);
            outStream.writeString("hello");
            outStream.writePendingValues();
            outStream.endEncapsulation();
            outParams = outStream.finished();
            return true;
        }
        else if(current.operation.Equals("throwPrintFailure"))
        {
            Console.WriteLine("Throwing PrintFailure");
            var ex = new PrintFailure("paper tray empty");
            var outStream = new Ice.OutputStream(communicator);
            outStream.startEncapsulation();
            outStream.writeException(ex);
            outStream.endEncapsulation();
            outParams = outStream.finished();
            return false;
        }
        else if(current.operation.Equals("shutdown"))
        {
            current.adapter.getCommunicator().shutdown();
            return true;
        }
        else
        {
            throw new Ice.OperationNotExistException(current.id, current.facet, current.operation);
        }
    }
}
