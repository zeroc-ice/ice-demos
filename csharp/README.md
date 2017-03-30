# C# Demos

## Overview

This directory contains C# sample programs for various Ice components. These
examples are provided to get you started on using a particular Ice feature or coding
technique.

Most of the subdirectories here correspond directly to Ice components, such as
[Glacier2](./Glacier2), [IceBox](./IceBox), and so on. We've also included the following
additional subdirectories:

- [Manual](./Manual) contains complete examples for some of the code snippets
in the [Ice manual][1].

- [Chat](./Chat) contains a .NET GUI client for the ZeroC [Chat Demo][2].

Refer to the [C++11 demos](../cpp11) for more examples that use the Ice services
(Glacier2, IceGrid, IceStorm).

## Build Instructions

### Prerequisites

The projects for the sample programs require the [Ice Builder for Visual Studio][3].
Add this extension to Visual Studio before opening the solution.

### Building the Demos

#### Building the demos using NuGet packages:

Open the solution file `CSharp demos.sln` to build the sample programs.

The demos are configure to use Ice distribution from Nuget packages, this packages are
automatically downloaded during build. If you have disabled the automatic download of
Nuget packages in Visual Studio you need to restore the packages before you build.

Packages can be restored from "Tools > NuGet Package Manager > Manage NuGet Packages for Solution..."
command in Visual Studio.

After restoring the packages right click on the desired demo in the Solution Explorer
window and select "Build"

#### Building the demos without using NuGet packages:

- Build from command line:
  * Open a Visual Studio command prompt

            cd ice-demos\csharp
            MSBuild /p:ICE_SRC_DIST=all /p:IceHome=<Ice dist path> "CSharp demos.sln"

- Build from Visual Studio
  * Open a Visual Studio command prompt
  
            set ICE_SRC_DIST=all
            devenv

  * When Visual Studio starts set IceHome in Ice Builder options, "Tools > Options > Projects and Solutions > Ice Builder"
  * Disable automatic restoring of NuGet packages in Visual Studio from "Tools > Options > NuGet Package Manager"
  * Right click on the desired demo in the Solution Explorer window and select "Build".

## Running the Demos

Before running a demo, make sure you've configured your environment to use Ice
as described in the [release notes][4].

Refer to the README file in each demo directory for usage instructions.

[1]: https://doc.zeroc.com/display/Ice37/Ice+Manual
[2]: https://zeroc.com/chat/index.html
[3]: https://github.com/zeroc-ice/ice-builder-visualstudio
[4]: https://doc.zeroc.com/display/Ice37/Ice+Release+Notes
