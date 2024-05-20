//
// Copyright (c) ZeroC, Inc. All rights reserved.
//

#include <Ice/Ice.h>
#include <PrinterI.h>

using namespace std;
using namespace Demo;

static ostream&
operator<<(ostream& out, Color c)
{
    switch(c)
    {
    case Color::red:
        out << "red";
        break;
    case Color::green:
        out << "green";
        break;
    case Color::blue:
        out << "blue";
        break;
    }
    return out;
}

bool
PrinterI::ice_invoke(vector<Ice::Byte> inParams, vector<Ice::Byte>& outParams, const Ice::Current& current)
{
    auto communicator = current.adapter->getCommunicator();

    bool result = true; // success as opposed to user exception

    Ice::InputStream in(communicator, inParams);
    in.startEncapsulation();

    if(current.operation == "printString")
    {
        string message;
        in.read(message);
        cout << "Printing string `" << message << "'" << endl;
    }
    else if(current.operation == "printStringSequence")
    {
        Demo::StringSeq seq;
        in.read(seq);
        cout << "Printing string sequence {";
        for(auto p = seq.begin(); p != seq.end(); ++p)
        {
            if(p != seq.begin())
            {
                cout << ", ";
            }
            cout << "'" << *p << "'";
        }
        cout << "}" << endl;
    }
    else if(current.operation == "printDictionary")
    {
        Demo::StringDict dict;
        in.read(dict);
        cout << "Printing dictionary {";
        for(auto p = dict.begin(); p != dict.end(); ++p)
        {
            if(p != dict.begin())
            {
                cout << ", ";
            }
            cout << p->first << "=" << p->second;
        }
        cout << "}" << endl;
    }
    else if(current.operation == "printEnum")
    {
        Demo::Color c;
        in.read(c);
        cout << "Printing enum " << c << endl;
    }
    else if(current.operation == "printStruct")
    {
        Demo::Structure s;
        in.read(s);
        cout << "Printing struct: name=" << s.name << ", value=" << s.value << endl;
    }
    else if(current.operation == "printStructSequence")
    {
        Demo::StructureSeq seq;
        in.read(seq);
        cout << "Printing struct sequence: {";
        for(auto p = seq.begin(); p != seq.end(); ++p)
        {
            if(p != seq.begin())
            {
                cout << ", ";
            }
            cout << p->name << "=" << p->value;
        }
        cout << "}" << endl;
    }
    else if(current.operation == "printClass")
    {
        shared_ptr<Demo::C> c;
        in.read(c);
        cout << "Printing class: s.name=" << c->s.name << ", s.value=" << c->s.value << endl;
    }
    else if(current.operation == "getValues")
    {
        auto c = make_shared<Demo::C>();
        c->s.name = "green";
        c->s.value = Color::green;
        Ice::OutputStream out(communicator);
        out.startEncapsulation();
        out.write(c);
        out.write("hello");
        out.endEncapsulation();
        out.finished(outParams);
    }
    else if(current.operation == "throwPrintFailure")
    {
        cout << "Throwing PrintFailure" << endl;
        Demo::PrintFailure ex;
        ex.reason = "paper tray empty";
        Ice::OutputStream out(communicator);
        out.startEncapsulation();
        out.write(ex);
        out.endEncapsulation();
        out.finished(outParams);
        result = false;
    }
    else if(current.operation == "shutdown")
    {
        current.adapter->getCommunicator()->shutdown();
    }
    else
    {
        throw Ice::OperationNotExistException(__FILE__, __LINE__, current.id, current.facet, current.operation);
    }

    //
    // Call endEncapsulation to make sure we read all in parameters
    //
    in.endEncapsulation();
    return result;
}
