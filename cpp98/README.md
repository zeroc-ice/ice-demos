# C++ Demos

- [Overview](#overview)
- [Building and Running the Demos on Linux and OS X](#building-and-running-the-demos-on-linux-and-os-x)
- [Building and Running the Demos on Windows](#building-and-running-the-demos-on-windows)
## Overview

This directory contains C++ sample programs for various Ice components. These
examples are provided to get you started on using a particular Ice feature or
coding technique.

Most of the subdirectories here correspond directly to Ice components, such as
[IceGrid](./IceGrid), [Glacier2](./Glacier2), and so on. We've also included the
following additional subdirectories:

- [Manual](./Manual) contains complete examples for some of the code snippets
in the [Ice manual][1].

- [Chat](./Chat) contains the C++ server, a command-line client,
and a QT client for the ZeroC [Chat Demo][2].

- [IceTouch](./IceTouch) contains iPhone demos for the Ice Touch SDKs.

## Building and Running the Demos on Linux and OS X

### Prerequisites

The makefiles require GNU make 3.80 or later.

On OS X, the command-line demos require the Xcode Command Line Tools to be
installed (use `xcode-select --install` to install them). The Ice Touch sample
programs require the [Ice Builder for Xcode][3].

If you've installed Ice in a non-standard location, you'll need to set the
`ICE_HOME` environment variable with the path name of the
installation directory:

    export ICE_HOME=~/testing/Ice

### Building the Demos

Review the settings in `make/Make.rules`. For example, set `OPTIMIZE=yes`
to build with optimization.

When you're ready to start the build, run `make`:

    make

To build the iOS examples, open `IceTouch/demos.xcworkspace` in Xcode.

### Running the Demos

Before running a demo, make sure you've configured your environment to use Ice
as described in the [release notes][4].

If you've installed Ice in a non-standard location, you'll need to set
`LD_LIBRARY_PATH` (Linux) or `DYLD_LIBRARY_PATH` (OS X)
with the library directory of your Ice installation:

```bash
export LD_LIBRARY_PATH=$ICE_HOME/lib     (Linux x86)
export LD_LIBRARY_PATH=$ICE_HOME/lib64   (Linux x64)
export DYLD_LIBRARY_PATH=$ICE_HOME/lib   (OS X)
```

Refer to the README file in each demo directory for usage instructions.

## Building and Running the Demos on Windows

### Prerequisites

The projects for the sample programs require the [Ice Builder for Visual Studio][5].
Add this extension to Visual Studio before opening the solution.

### Building the Demos

#### Building the demos using NuGet packages:

Open the solution file `C++ demos.sln` to build the sample programs.

The demos are configure to use Ice distribution from Nuget packages, this packages are
automatically downloaded during build. If you have disabled the automatic download of
Nuget packages in Visual Studio you need to restore the packages before you build.

Packages can be restored from "Tools > NuGet Package Manager > Manage NuGet Packages for Solution..."
command in Visual Studio.

After restoring the packages select your target configuration: Debug or Release and platform
Win32 or x64. Right click on the desired demo in the Solution Explorer window and select "Build".

#### Building the demos without using NuGet packages:

- Build from command line:
  * Open a Visual Studio command prompt

            cd ice-demos\cpp
            MSBuild /p:ICE_SRC_DIST=all /p:IceHome:<Ice dist path> "C++ demos.sln"

- Build from Visual Studio
  * Open a Visual Studio command prompt
  
            set ICE_SRC_DIST=all
            devenv

  * When Visual Studio starts set IceHome in Ice Builder options, "Tools > Options > Projects and Solutions > Ice Builder"
  * Disable automatic restoring of NuGet packages in Visual Studio from "Tools > Options > NuGet Package Manager"
  * Select your target configuration: Debug or Release and platform Win32 or x64. Right click on the desired demo in the Solution Explorer window and select "Build".

### Running the Demos

Before running a demo, make sure you've configured your environment to use Ice
as described in the [release notes][3].

Refer to the README file in each demo directory for usage instructions.

[1]: https://doc.zeroc.com/display/Ice37/Ice+Manual
[2]: https://zeroc.com/chat/index.html
[3]: https://github.com/zeroc-ice/ice-builder-xcode
[4]: https://doc.zeroc.com/display/Ice37/Ice+Release+Notes
[5]: https://github.com/zeroc-ice/ice-builder-visualstudio
