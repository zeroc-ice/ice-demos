# C++ Demos

- [Overview](#overview)
- [Building and Running the Demos on Linux and OS X](#building-and-running-the-demos-on-linux-and-os-x)
- [Building and Running the Demos on Windows](#building-and-running-the-demos-on-windows)
- [Building the Demo Apps for WinRT and Universal Windows Platform (UWP)](#building-the-demo-apps-for-winrt-and-uwp)
- [Building the demos for Ice-E](#building-the-demos-for-ice-e)

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

Packages can be restore from "Tools > NuGet Package Manager > Manage NuGet Packages for Solution..."
command in Visual Studio.

After restoring the packages select your target configuration: Debug or Release and platform
Win32 or x64. Right click on the desired demo in the Solution Explorer window and select "Build".

#### Building the demos without using NuGet packages:

- Build from command line:

 - Open a Visual Studio command prompt

    cd ice-demos\cpp
    MSBuild /p:UseIceHome=yes /p:IceHome:<Ice dist path> /p:Configuration<Debug|Release> /p:Platform:<x64|Win32> "C++ demos.sln"

- Build from Visual Studio

 - Open a Visual Studio command prompt

    set UseIceHome=yes
    devenv

 - When Visual Studio starts set IceHome in Ice Builder options, "Tools > Options > Projects and Solutions > Ice Builder"
 - Disable automatic restoring of NuGet packages in Visual Studio from "Tools > Options > NuGet Package Manager"
 - Select your target configuration: Debug or Release and platform Win32 or x64. Right click on the desired demo in the Solution Explorer window and select "Build".
 


### Running the Demos

Before running a demo, make sure you've configured your environment to use Ice
as described in the [release notes][3].

Refer to the README file in each demo directory for usage instructions.

## Building the Demo Apps for WinRT and UWP

### Prerequisites

The projects for the sample programs require the [Ice Builder for Visual Studio][5].
Add this extension to Visual Studio before opening the solution.

WinRT requires Windows 8.1 with Visual Studio 2013, and UWP requires Windows 10
with Visual Studio 2015.

### Building the Demos

Open the solution file `C++ demos (Windows 8.1 WinRT).sln` or `C++ demos (Universal Windows).sln`
to build the WinRT resp. UWP demo programs.

Select your target configuration: Debug or Release, and Win32, x86 or x64.
Right click on the desired demo in the Solution Explorer window and
select "Build".

Refer to the README file in each demo directory for usage instructions.

## Building the Demos for Ice-E

Review the settings in `make/Make.rules`. For example, set`OPTIMIZE=yes`
to build with optimization. Ensure `ICEE_TARGET_OS` is set to a supported
platform, either `debian` or `yocto`.

When you're ready to start the build, run `make`:

    make BUILD_WITH_ICEE=yes

The default build configuration will cross-compile demos for the target
set in `ICEE_TARGET_OS`. After a successful build, you need to deploy the
demos to the target device:

    make BUILD_WITH_ICEE=yes demo_deploy

This command will deploy the demos to the device specified by the
`DEPLOY_TARGET` variable in `make/Make.rules`.

After successfully deploying the demos, open an ssh session to the target
and change to the deployment directory:

    ssh debian@192.168.7.2
    cd ice-demos/cpp

Refer to the README file in each demo directory for usage instructions.

[1]: https://doc.zeroc.com/display/Ice37/Ice+Manual
[2]: https://zeroc.com/chat/index.html
[3]: https://github.com/zeroc-ice/ice-builder-xcode
[4]: https://doc.zeroc.com/display/Ice37/Ice+Release+Notes
[5]: https://github.com/zeroc-ice/ice-builder-visualstudio
