# PHP Demos

## Overview

This directory contains PHP sample programs for various Ice components. These
examples are provided to get you started on using a particular Ice feature or coding
technique.

The following subdirectories contain sample programs:

- [Chat](./Chat) contains a GUI client for the ZeroC [Chat Demo][1].

- [Glacier2](./Glacier2) has an example that maintains a Glacier2 session.

- [Ice](./Ice) has several examples that illustrate fundamental Ice concepts.

- [IceDiscovery](./IceDiscovery) demonstrates the use of our multicast service
discovery plug-in.

Refer to the [C++11 demos](../cpp11) for more examples that use Ice services
such as IceGrid and IceStorm.

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

Review the settings in `make/Make.rules`. When you're ready to start the build,
run `make`:

    $ make

## Build Instructions for Windows

### Prerequisites

The projects for the sample programs require the [Ice Builder for Visual Studio][3].
Add this extension to Visual Studio before building the demos.

### Building the Demos

When you're ready to start the build, run `MSBuild`:

    > MSBuild msbuild\ice.proj /p:IceHome=C:\Ice-3.7.0

## Running the Demos

Refer to the README file in each demo directory for usage instructions.

[1]: https://zeroc.com/chat/index.html
[2]: http://www.microsoft.com/downloads/details.aspx?FamilyId=E6E1C3DF-A74F-4207-8586-711EBE331CDC&displaylang=en
[3]: https://github.com/zeroc-ice/ice-builder-visualstudio
