# C++ Demos

## Overview

This directory contains C++ sample programs for various Ice components. These
examples are provided to get you started on using a particular Ice feature or
coding technique.

Most of the subdirectories here correspond directly to Ice components, such as
[Freeze](./Freeze), [Glacier2](./Glacier2), and so on. We've also included the
following additional subdirectories:

- [Manual](./Manual) contains complete examples for some of the code snippets
in the [Ice manual][1].

- [Chat](./Chat) contains the C++ server, a command-line client,
and a QT client for the ZeroC [Chat Demo][2].

- [IceTouch](./IceTouch) contains iPhone demos for the Ice Touch SDKs.

## Build Instructions for Linux & OS X

### Prerequisites

The makefiles require GNU make 3.80 or later. If your system does not come
with GNU make, you can find information on how to acquire it at the
following link:

http://www.gnu.org/software/make/

If you've installed Ice in a non-standard location, you'll need to set the
`ICE_HOME` environment variable with the path name of the
installation directory:

    $ export ICE_HOME=~/testing/Ice

### Building the Demos

Review the settings in `make/Make.rules`. For example, set `OPTIMIZE=yes`
to build with optimization.

When you're ready to start the build, run `make`:

    $ make

## Running the Demos

Before running a demo, make sure you've configured your environment to use Ice
as described in the [release notes][3].

If you've installed Ice in a non-standard location, you'll need to set
`LD_LIBRARY_PATH` (Linux) or `DYLD_LIBRARY_PATH` (OS X)
with the library directory of your Ice installation:

        $ export LD_LIBRARY_PATH=$ICE_HOME/lib     (Linux x86)
        $ export LD_LIBRARY_PATH=$ICE_HOME/lib64   (Linux x64)
        $ export DYLD_LIBRARY_PATH=$ICE_HOME/lib   (OS X)

Refer to the README file in each demo directory for usage instructions.

## Building and Using the Demos for Windows

### Prerequisites

The projects for the sample programs require the [Ice Builder for Visual Studio][4].
Add this extension to Visual Studio before opening the solution.

### Building the Demos

Open the solution file `C++ demos.sln` to build the sample programs. This file
was created with Visual Studio 2012 and will be converted if you are using a newer
version of Visual Studio.

Select your target configuration: Debug or Release, Win32 or x64 (on supported x64
platforms). Right click on the desired demo in the Solution Explorer window and
select "Build".

### Running the Demos

Before running a demo, make sure you've configured your environment to use Ice
as described in the [release notes][3].

Refer to the README file in each demo directory for usage instructions.

## Building and Using the Demos for WinRT

### Prerequisites

The projects for the sample programs require the [Ice Builder for Visual Studio][4].
Add this extension to Visual Studio before opening the solution.

WinRT requires Windows 8.1 with Visual Studio 2013.

### Building the Demos

Open the solution file `C++ demos for WinRT.sln` to build the sample programs.

Select your target configuration: Debug or Release, Win32 or x64 (on supported x64
platforms) or ARM. Right click on the desired demo in the Solution Explorer window
and select "Build".

### Running the Demos

To run the demos with a device such as Surface, you will also need to re-create the
server certificate to try out SSL. The Windows RT SSL implementation requires the
certificate's common name to match the IP address or DNS name of the target server.
We provide a `makecerts.py` Python script to re-generate the server certificate
in the `certs` directory at the root of the Ice demo source tree.

To run the script:

    > cd certs
    > makecerts.py 192.168.1.50

In the example above, the server certificate's common name will be set to
192.168.1.50. The certificate is generated in the `certs` directory. You
should specify the IP address of the machine running the SSL server here. You
will need to restart Visual Studio 2013 and rebuild the demo project to ensure
the new root certificate is included with the demo application.

Refer to the README file in each demo directory for usage instructions.

[1]: https://doc.zeroc.com/display/Ice36/Ice+Manual
[2]: https://zeroc.com/chat/index.html
[3]: https://doc.zeroc.com/display/Rel/Ice+3.6.0+Release+Notes
[4]: https://github.com/zeroc-ice/ice-builder-visualstudio
