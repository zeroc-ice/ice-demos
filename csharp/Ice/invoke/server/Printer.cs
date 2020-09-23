// Copyright (c) ZeroC, Inc. All rights reserved.

using System;
using System.Collections.Generic;
using System.Threading.Tasks;
using ZeroC.Ice;

namespace Demo
{
    public class Printer : IObject
    {
        public ValueTask<OutgoingResponseFrame> DispatchAsync(IncomingRequestFrame request, Current current)
        {
            OutgoingResponseFrame? response = null;
            if (current.Operation == "printString")
            {
                string message = request.ReadParamList(current.Communicator, istr => istr.ReadString());
                Console.WriteLine($"Printing string `{message}'");
            }
            else if (current.Operation == "printStringSequence")
            {
                string[] messages = request.ReadParamList(current.Communicator,
                                                          istr => istr.ReadArray(1, istr => istr.ReadString()));
                Console.Write("Printing string sequence {");
                Console.Write(string.Join(", ", messages));
                Console.WriteLine("}");
            }
            else if (current.Operation == "printDictionary")
            {
                Dictionary<string, string> dict = request.ReadParamList(current.Communicator,
                    istr => istr.ReadDictionary(1, 1, istr => istr.ReadString(), istr => istr.ReadString()));
                Console.Write("Printing dictionary {");
                bool first = true;
                foreach ((string key, string value) in dict)
                {
                    if (!first)
                    {
                        Console.Write(", ");
                    }
                    first = false;
                    Console.Write($"{key} = {value}");
                }
                Console.WriteLine("}");
            }
            else if (current.Operation == "printEnum")
            {
                Color color = request.ReadParamList(current.Communicator, ColorHelper.IceReader);
                Console.WriteLine($"Printing enum {color}");
            }
            else if (current.Operation == "printStruct")
            {
                Structure s = request.ReadParamList(current.Communicator, Structure.IceReader);
                Console.WriteLine($"Printing struct: name={s.Name}, value={s.Value}");
            }
            else if (current.Operation == "printStructSequence")
            {
                Structure[] seq = request.ReadParamList(
                    current.Communicator,
                    istr => istr.ReadArray(minElementSize: 2, Structure.IceReader));
                Console.Write("Printing struct sequence: {");
                bool first = true;
                foreach (Structure s in seq)
                {
                    if (!first)
                    {
                        Console.Write(", ");
                    }
                    first = false;
                    Console.Write($"{s.Name} = {s.Value}");
                }
                Console.WriteLine("}");
            }
            else if (current.Operation == "printClass")
            {
                C c = request.ReadParamList(current.Communicator, istr => istr.ReadClass<C>(C.IceTypeId));
                Console.WriteLine($"Printing class: s.name={c.S.Name}, s.value={c.S.Value}");
            }
            else if (current.Operation == "getValues")
            {
                var c = new C(new Structure("green", Color.green));
                response = OutgoingResponseFrame.WithReturnValue<(string, C)>(
                    current,
                    false,
                    FormatType.Compact,
                    ("hello", c),
                    (OutputStream ostr, (string s, C c) value) =>
                    {
                        ostr.WriteString(value.s);
                        ostr.WriteClass(value.c, C.IceTypeId);
                    });
            }
            else if (current.Operation == "throwPrintFailure")
            {
                Console.WriteLine("Throwing PrintFailure");
                var ex = new PrintFailure("paper tray empty");

                response = new OutgoingResponseFrame(request, ex);
            }
            else if (current.Operation == "shutdown")
            {
                current.Adapter.Communicator.DisposeAsync();
            }
            else
            {
                throw new OperationNotExistException(current.Identity, current.Facet, current.Operation);
            }

            return new ValueTask<OutgoingResponseFrame>(
                response ?? OutgoingResponseFrame.WithVoidReturnValue(current));
        }
    }

}
