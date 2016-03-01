// **********************************************************************
//
// Copyright (c) 2003-2016 ZeroC, Inc. All rights reserved.
//
// **********************************************************************

#include <Ice/Ice.h>
#include <IcePatch2/IcePatch2.h>

using namespace std;

class PatcherFeedback : public IcePatch2::PatcherFeedback
{
public:

    PatcherFeedback() :
        _filesPatched(0)
    {
    }

    virtual bool
    noFileSummary(const string& /*reason*/)
    {
        return true;
    }

    virtual bool
    checksumStart()
    {
        return true;
    }

    virtual bool
    checksumProgress(const string& /*path*/)
    {
        return true;
    }

    virtual bool
    checksumEnd()
    {
        return true;
    }

    virtual bool
    fileListStart()
    {
        return true;
    }

    virtual bool
    fileListProgress(Ice::Int /*percent*/)
    {
        return true;
    }

    virtual bool
    fileListEnd()
    {
        return true;
    }

    virtual bool
    patchStart(const string& path, Ice::Long /*size*/, Ice::Long /*totalProgress*/, Ice::Long /*totalSize*/)
    {
        cout << "Patching \'" << path << "\' ";
        return true;
    }

    virtual bool
    patchProgress(Ice::Long /*progress*/, Ice::Long /*size*/, Ice::Long /*totalProgress*/, Ice::Long /*totalSize*/)
    {
        cout << ".";
        return true;
    }

    virtual bool
    patchEnd()
    {
        cout << " done" << endl;
        ++_filesPatched;
        return true;
    }

    virtual int
    filesPatched() const
    {
        return _filesPatched;
    }

private:

    int _filesPatched;
};
typedef IceUtil::Handle<PatcherFeedback> PatcherFeedbackPtr;

class SimpleClient : public Ice::Application
{
public:

    SimpleClient();

    virtual int run(int, char*[]);

private:

    void menu();
};

int
main(int argc, char* argv[])
{
    SimpleClient app;
    return app.main(argc, argv, "config.client");
}

SimpleClient::SimpleClient() :
    //
    // Since this is an interactive demo we don't want any signal
    // handling.
    //
    Ice::Application(Ice::NoSignalHandling)
{
}

int
SimpleClient::run(int argc, char* argv[])
{
    if(argc > 1)
    {
        cerr << argv[0] << ": too many arguments" << endl;
        return EXIT_FAILURE;
    }

    menu();

    Ice::PropertiesPtr properties = communicator()->getProperties();
    properties->setProperty("IcePatch2Client.Thorough", "0");
    properties->setProperty("IcePatch2Client.Remove", "0");

    char c = 'x';
    do
    {
        try
        {
            cout << "==> ";
            cin >> c;
            if(c == 'p')
            {
                PatcherFeedbackPtr feedback = new PatcherFeedback();
                IcePatch2::PatcherPtr patcher = IcePatch2::PatcherFactory::create(communicator(), feedback);

                bool aborted = !patcher->prepare();
                if(!aborted)
                {
                    aborted = !patcher->patch("");
                }
                if(!aborted)
                {
                    patcher->finish();
                    cout << "Patched " << feedback->filesPatched() << " files." << endl;
                }
            }
            else if(c == 't')
            {
                int thorough = properties->getPropertyAsInt("IcePatch2Client.Thorough");
                properties->setProperty("IcePatch2Client.Thorough", thorough == 0 ? "1" : "0");
                cout << "Thorough " << (thorough == 0 ? "enabled" : "disabled") << endl;
            }
            else if(c == 'r')
            {
                int remove = properties->getPropertyAsInt("IcePatch2Client.Remove");
                properties->setProperty("IcePatch2Client.Remove", remove == 0 ? "2" : "0");
                cout << "Remove " << (remove == 0 ? "enabled" : "disabled") << endl;
            }
            else if(c == 'x')
            {
                // Nothing to do
            }
            else if(c == '?')
            {
                menu();
            }
            else
            {
                cout << "unknown command `" << c << "'" << endl;
                menu();
            }
        }
        catch(const Ice::Exception& ex)
        {
            cerr << ex << endl;
        }
    }
    while(cin.good() && c != 'x');

    return EXIT_SUCCESS;
}

void
SimpleClient::menu()
{
    cout <<
        "usage:\n"
        "p: Perform patch\n"
        "t: Toggle IcePatch2Client.Thorough (recalulates file checksums)\n"
        "r: Toggle IcePatch2Client.Remove (enables removal of orphaned files)\n"
        "x: exit\n"
        "?: help\n";
}
