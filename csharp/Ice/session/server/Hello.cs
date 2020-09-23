// Copyright (c) ZeroC, Inc. All rights reserved.

using System;
using ZeroC.Ice;

namespace Demo
{
    public class Hello : IHello
    {
        private readonly int _id;
        private readonly string _name;

        public Hello(string name, int id)
        {
            _name = name;
            _id = id;
        }

        public void SayHello(Current c) =>
            Console.WriteLine($"Hello object #{_id} for session `{_name}' says:\nHello {_name}!");
    }

}
