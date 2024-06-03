// Copyright (c) ZeroC, Inc.

using Demo;

internal class PrinterI : Ice.Blobject
{
    private class ReadValueCallback
    {
        public void Invoke(Ice.Value obj)
        {
            this.Obj = obj;
        }

        internal Ice.Value Obj;
    }

    public override bool ice_invoke(byte[] inParams, out byte[] outParams, Ice.Current current)
    {
        outParams = null;
        bool result = true;

        var communicator = current.adapter.getCommunicator();

        Ice.InputStream inStream = null;
        if (inParams.Length > 0)
        {
            inStream = new Ice.InputStream(communicator, inParams);
            inStream.startEncapsulation();
        }

        if (current.operation.Equals("printString"))
        {
            var message = inStream.readString();
            Console.WriteLine("Printing string `" + message + "'");
        }
        else if (current.operation.Equals("printStringSequence"))
        {
            var seq = StringSeqHelper.read(inStream);
            Console.Write("Printing string sequence {");
            for (int i = 0; i < seq.Length; ++i)
            {
                if (i > 0)
                {
                    Console.Write(", ");
                }
                Console.Write("'" + seq[i] + "'");
            }
            Console.WriteLine("}");
        }
        else if (current.operation.Equals("printDictionary"))
        {
            var dict = StringDictHelper.read(inStream);
            Console.Write("Printing dictionary {");
            bool first = true;
            foreach (var e in dict)
            {
                if (!first)
                {
                    Console.Write(", ");
                }
                first = false;
                Console.Write(e.Key + "=" + e.Value);
            }
            Console.WriteLine("}");
        }
        else if (current.operation.Equals("printEnum"))
        {
            var c = ColorHelper.read(inStream);
            Console.WriteLine("Printing enum " + c);
        }
        else if (current.operation.Equals("printStruct"))
        {
            var s = Structure.ice_read(inStream);
            Console.WriteLine("Printing struct: name=" + s.name + ", value=" + s.value);
        }
        else if (current.operation.Equals("printStructSequence"))
        {
            var seq = StructureSeqHelper.read(inStream);
            Console.Write("Printing struct sequence: {");
            for (int i = 0; i < seq.Length; ++i)
            {
                if (i > 0)
                {
                    Console.Write(", ");
                }
                Console.Write(seq[i].name + "=" + seq[i].value);
            }
            Console.WriteLine("}");
        }
        else if (current.operation.Equals("printClass"))
        {
            var cb = new ReadValueCallback();
            inStream.readValue(cb.Invoke);
            inStream.readPendingValues();
            var c = cb.Obj as C;
            Console.WriteLine("Printing class: s.name=" + c.s.name + ", s.value=" + c.s.value);
        }
        else if (current.operation.Equals("getValues"))
        {
            var c = new C(new Structure("green", Color.green));
            var outStream = new Ice.OutputStream(communicator);
            outStream.startEncapsulation();
            outStream.writeValue(c);
            outStream.writeString("hello");
            outStream.writePendingValues();
            outStream.endEncapsulation();
            outParams = outStream.finished();
        }
        else if (current.operation.Equals("throwPrintFailure"))
        {
            Console.WriteLine("Throwing PrintFailure");
            var ex = new PrintFailure("paper tray empty");
            var outStream = new Ice.OutputStream(communicator);
            outStream.startEncapsulation();
            outStream.writeException(ex);
            outStream.endEncapsulation();
            outParams = outStream.finished();
            result = false;
        }
        else if (current.operation.Equals("shutdown"))
        {
            current.adapter.getCommunicator().shutdown();
        }
        else
        {
            throw new Ice.OperationNotExistException(current.id, current.facet, current.operation);
        }

        // Make sure we read all in parameters
        inStream.endEncapsulation();
        return result;
    }
}
