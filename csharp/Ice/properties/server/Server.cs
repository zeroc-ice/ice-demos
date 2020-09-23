//
//

using Demo;
using System;
using System.Collections.Generic;
using System.Configuration;
using System.Threading;
using ZeroC.Ice;

// using statement - communicator is automatically destroyed at the end of this statement
using var communicator = new Communicator(ref args, ConfigurationManager.AppSettings);

// Destroy the communicator on Ctrl+C or Ctrl+Break
Console.CancelKeyPress += (sender, eventArgs) => communicator.DisposeAsync();

if (args.Length > 0)
{
    throw new ArgumentException("too many arguments");
}

var props = new Properties();

// Retrieve the PropertiesAdmin facet and use props.updated as the update callback.
var admin = communicator.FindAdminFacet("Properties") as IPropertiesAdmin;
admin!.Updated += (sender, changes) => props.Updated(changes);

ObjectAdapter adapter = communicator.CreateObjectAdapter("Properties");
adapter.Add("properties", props);
adapter.Activate();
communicator.WaitForShutdown();

// The servant implements the Slice interface Demo::Props as well as the native callback interface
// Ice.PropertiesAdminUpdateCallback.
class Properties : IProperties
{
    private bool _called;
    private IReadOnlyDictionary<string, string>? _changes;
    private readonly object _mutex = new object();

    public Properties() => _called = false;

    public IReadOnlyDictionary<string, string> GetChanges(Current current)
    {
        lock (_mutex)
        {
            // Make sure that we have received the property updates before we return the results.
            while (!_called)
            {
                Monitor.Wait(this);
            }

            _called = false;
            return _changes!;
        }
    }

    public void Shutdown(Current current) =>
        current.Communicator.DisposeAsync();

    public void Updated(IReadOnlyDictionary<string, string> changes)
    {
        lock (_mutex)
        {
            _changes = changes;
            _called = true;
            Monitor.Pulse(_mutex);
        }
    }
}
