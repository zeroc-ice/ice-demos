//
// Copyright (c) ZeroC, Inc. All rights reserved.
//

#include "Printer.h"
#include <Ice/Ice.h>
#include <iostream>

using namespace std;
using namespace Demo;

static ostream&
operator<<(ostream& out, Color c)
{
    switch (c)
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

int run(const shared_ptr<Ice::Communicator>&);

int
main(int argc, char* argv[])
{
    int status = 0;

    try
    {
        //
        // CommunicatorHolder's ctor initializes an Ice communicator,
        // and its dtor destroys this communicator.
        //
        const Ice::CommunicatorHolder ich(argc, argv, "config.client");

        //
        // The communicator initialization removes all Ice-related arguments from argc/argv
        //
        if (argc > 1)
        {
            cerr << argv[0] << ": too many arguments" << endl;
            status = 1;
        }
        else
        {
            status = run(ich.communicator());
        }
    }
    catch (const std::exception& ex)
    {
        cerr << argv[0] << ": " << ex.what() << endl;
        status = 1;
    }

    return status;
}

void menu();

int
run(const shared_ptr<Ice::Communicator>& communicator)
{
    auto obj = communicator->propertyToProxy("Printer.Proxy");

    menu();

    char ch = 'x';
    do
    {
        try
        {
            cout << "==> ";
            cin >> ch;
            if (ch == '1')
            {
                //
                // Marshal the in parameter.
                //
                Ice::ByteSeq inParams, outParams;
                Ice::OutputStream out(communicator);
                out.startEncapsulation();
                out.write("The streaming API works!");
                out.endEncapsulation();
                out.finished(inParams);

                //
                // Invoke operation.
                //
                if (!obj->ice_invoke("printString", Ice::OperationMode::Normal, inParams, outParams))
                {
                    cout << "Unknown user exception" << endl;
                }
            }
            else if (ch == '2')
            {
                //
                // Marshal the in parameter.
                //
                Ice::ByteSeq inParams, outParams;
                Ice::OutputStream out(communicator);
                out.startEncapsulation();
                const Demo::StringSeq arr({"The", "streaming", "API", "works!"});
                out.write(arr);
                out.endEncapsulation();
                out.finished(inParams);

                //
                // Invoke operation.
                //
                if (!obj->ice_invoke("printStringSequence", Ice::OperationMode::Normal, inParams, outParams))
                {
                    cout << "Unknown user exception" << endl;
                }
            }
            else if (ch == '3')
            {
                //
                // Marshal the in parameter.
                //
                Ice::ByteSeq inParams, outParams;
                Ice::OutputStream out(communicator);
                out.startEncapsulation();
                const Demo::StringDict dict{{"The", "streaming"}, {"API", "works!"}};
                out.write(dict);
                out.endEncapsulation();
                out.finished(inParams);

                //
                // Invoke operation.
                //
                if (!obj->ice_invoke("printDictionary", Ice::OperationMode::Normal, inParams, outParams))
                {
                    cout << "Unknown user exception" << endl;
                }
            }
            else if (ch == '4')
            {
                //
                // Marshal the in parameter.
                //
                Ice::ByteSeq inParams, outParams;
                Ice::OutputStream out(communicator);
                out.startEncapsulation();
                out.write(Color::green);
                out.endEncapsulation();
                out.finished(inParams);

                //
                // Invoke operation.
                //
                if (!obj->ice_invoke("printEnum", Ice::OperationMode::Normal, inParams, outParams))
                {
                    cout << "Unknown user exception" << endl;
                }
            }
            else if (ch == '5')
            {
                //
                // Marshal the in parameter.
                //
                Ice::ByteSeq inParams, outParams;
                Ice::OutputStream out(communicator);
                out.startEncapsulation();
                Demo::Structure s;
                s.name = "red";
                s.value = Color::red;
                out.write(s);
                out.endEncapsulation();
                out.finished(inParams);

                //
                // Invoke operation.
                //
                if (!obj->ice_invoke("printStruct", Ice::OperationMode::Normal, inParams, outParams))
                {
                    cout << "Unknown user exception" << endl;
                }
            }
            else if (ch == '6')
            {
                //
                // Marshal the in parameter.
                //
                Ice::ByteSeq inParams, outParams;
                Ice::OutputStream out(communicator);
                out.startEncapsulation();
                Demo::StructureSeq arr;
                arr.emplace_back();
                arr.back().name = "red";
                arr.back().value = Color::red;
                arr.emplace_back();
                arr.back().name = "green";
                arr.back().value = Color::green;
                arr.emplace_back();
                arr.back().name = "blue";
                arr.back().value = Color::blue;
                out.write(arr);
                out.endEncapsulation();
                out.finished(inParams);

                //
                // Invoke operation.
                //
                if (!obj->ice_invoke("printStructSequence", Ice::OperationMode::Normal, inParams, outParams))
                {
                    cout << "Unknown user exception" << endl;
                }
            }
            else if (ch == '7')
            {
                //
                // Marshal the in parameter.
                //
                Ice::ByteSeq inParams, outParams;
                Ice::OutputStream out(communicator);
                out.startEncapsulation();
                auto c = make_shared<Demo::C>();
                c->s.name = "blue";
                c->s.value = Color::blue;
                out.write(c);
                out.writePendingValues();
                out.endEncapsulation();
                out.finished(inParams);

                //
                // Invoke operation.
                //
                if (!obj->ice_invoke("printClass", Ice::OperationMode::Normal, inParams, outParams))
                {
                    cout << "Unknown user exception" << endl;
                }
            }
            else if (ch == '8')
            {
                //
                // Invoke operation.
                //
                const Ice::ByteSeq inParams;
                Ice::ByteSeq outParams;
                if (!obj->ice_invoke("getValues", Ice::OperationMode::Normal, inParams, outParams))
                {
                    cout << "Unknown user exception" << endl;
                    continue;
                }

                //
                // Unmarshal the results.
                //
                Ice::InputStream in(communicator, outParams);
                in.startEncapsulation();
                Demo::CPtr c;
                in.read(c);
                string str;
                in.read(str);
                in.readPendingValues();
                in.endEncapsulation();
                cout << "Got string `" << str << "' and class: s.name=" << c->s.name << ", s.value=" << c->s.value
                     << endl;
            }
            else if (ch == '9')
            {
                //
                // Invoke operation.
                //
                const Ice::ByteSeq inParams;
                Ice::ByteSeq outParams;
                if (obj->ice_invoke("throwPrintFailure", Ice::OperationMode::Normal, inParams, outParams))
                {
                    cout << "Expected exception" << endl;
                    continue;
                }

                Ice::InputStream in(communicator, outParams);
                in.startEncapsulation();
                try
                {
                    in.throwException();
                }
                catch (const Demo::PrintFailure&)
                {
                    // Expected.
                }
                catch (const Ice::UserException&)
                {
                    cout << "Unknown user exception" << endl;
                }
                in.endEncapsulation();
            }
            else if (ch == 's')
            {
                const Ice::ByteSeq inParams;
                Ice::ByteSeq outParams;
                obj->ice_invoke("shutdown", Ice::OperationMode::Normal, inParams, outParams);
            }
            else if (ch == 'x')
            {
                // Nothing to do.
            }
            else if (ch == '?')
            {
                menu();
            }
            else
            {
                cout << "unknown command `" << ch << "'" << endl;
                menu();
            }
        }
        catch (const Ice::Exception& ex)
        {
            cerr << ex << endl;
        }
    } while (cin.good() && ch != 'x');

    return 0;
}

void
menu()
{
    cout << "usage:\n"
            "1: print string\n"
            "2: print string sequence\n"
            "3: print dictionary\n"
            "4: print enum\n"
            "5: print struct\n"
            "6: print struct sequence\n"
            "7: print class\n"
            "8: get values\n"
            "9: throw exception\n"
            "s: shutdown server\n"
            "x: exit\n"
            "?: help\n";
}
