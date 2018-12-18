// **********************************************************************
//
// Copyright (c) 2003-present ZeroC, Inc. All rights reserved.
//
// **********************************************************************

#include <Ice/Ice.h>
#include <Nrvo.h>
#include <MyStringSeq.h>
#include <iomanip>

using namespace std;
using namespace Demo;

int run(const Ice::CommunicatorPtr&);

int
main(int argc, char* argv[])
{
#ifdef ICE_STATIC_LIBS
    Ice::registerIceSSL();
#endif

    int status = 0;

    try
    {
        //
        // CommunicatorHolder's ctor initializes an Ice communicator,
        // and its dtor destroys this communicator.
        //
        Ice::CommunicatorHolder ich(argc, argv, "config.client");

        //
        // The communicator initialization removes all Ice-related arguments from argc/argv
        //
        if(argc > 1)
        {
            cerr << argv[0] << ": too many arguments" << endl;
            status = 1;
        }
        else
        {
            status = run(ich.communicator());
        }
    }
    catch(const std::exception& ex)
    {
        cerr << argv[0] << ": " << ex.what() << endl;
        status = 1;
    }

    return status;
}

void
menu()
{
    cout <<
        "usage:\n"
        "\n"
        "Operation to call:\n"
        "1: return a string sequence\n"
        "2: return a string sequence that is a data member of the servant\n"
        "3: return a string sequence from an operation with multiple return paths\n"
        "4: return a string sequence from an operation with multiple return paths, using RVO\n"
        "s: shutdown server\n"
        "x: exit\n"
        "?: show this menu\n";
}

int
run(const Ice::CommunicatorPtr& communicator)
{
    NrvoPrx nrvo = NrvoPrx::checkedCast(communicator->propertyToProxy("Nrvo.Proxy"));
    if(!nrvo)
    {
        cerr << "invalid proxy" << endl;
        return 1;
    }

    menu();

    char c = 'x';
    do
    {
        try
        {
            cout << "==> ";
            cin >> c;

            switch(c)
            {
                case '1':
                {
                    cout << "calling op1" << endl;
                    MyStringSeq seq = nrvo->op1();
                    break;
                }

                case '2':
                {
                    cout << "calling op2" << endl;
                    MyStringSeq seq = nrvo->op2();
                    break;
                }

                case '3':
                {
                    cout << "calling op3(10)" << endl;
                    MyStringSeq seq = nrvo->op3(10);
                    break;
                }

                case '4':
                {
                    cout << "calling op3(0)" << endl;
                    MyStringSeq seq = nrvo->op3(0);
                    break;
                }

                case 's':
                {
                    nrvo->shutdown();
                    break;
                }

                case '?':
                {
                    menu();
                    break;
                }

                case 'x':
                {
                    break;
                }
                default:
                {
                    cout << "unknown command `" << c << "'" << endl;
                    menu();
                    break;
                }
            }
        }
        catch(const Ice::Exception& ex)
        {
            cerr << ex << endl;
        }
    }
    while(cin.good() && c != 'x');

    return 0;
}
