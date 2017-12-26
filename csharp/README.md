# C# Demos

## Overview

This directory contains C# sample programs for various Ice components. These
examples are provided to get you started on using a particular Ice feature or
coding technique.

Most of the subdirectories here correspond directly to Ice components, such as
[Glacier2](./Glacier2), [IceBox](./IceBox), and so on. We've also included the
following additional subdirectories:

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

The demos are configured to use the Ice distribution from NuGet packages. These
packages are automatically downloaded during the build. If you have disabled the
automatic download of NuGet packages in Visual Studio, you need to restore the
packages before you build.

Packages can be restored via `Tools > NuGet Package Manager > Manage NuGet
Packages for Solution...` in Visual Studio.

After restoring the packages, right click on the desired demo in the Solution
Explorer window and select `Build`.

#### Building the demos using a source build:

- Build from command line:
  * Open a Visual Studio command prompt
  ```
  cd ice-demos\csharp
  MSBuild /p:ICE_HOME=<Ice dist path> /p:Platform="Any CPU" "CSharp demos.sln"
  ```

- Build from Visual Studio:
  * Open a Visual Studio command prompt
  ```
  set ICE_HOME=<Ice dist path>
  devenv
  ```

  * When Visual Studio starts disable automatic restoring of NuGet packages in
    Visual Studio from `Tools > Options > NuGet Package Manager`
  * Right click on the desired demo in the Solution Explorer window and select
    `Build`.

## Running the Demos

For most demos, you can simply run `client` and `server` in separate Command
Prompt windows.  Refer to the README.md file in each demo directory for the
exact usage instructions.

Some demos require Ice services such as IceGrid and IceStorm that are not
included in the `zeroc.ice.net` NuGet package. To run these demos, the simplest
is to first install the Ice MSI and add its bin directory to your PATH. Please
refer to [Using the Windows Binary Distributions][4] for additional information.

[1]: https://doc.zeroc.com/display/Ice37/Ice+Manual
[2]: https://zeroc.com/chat/index.html
[3]: https://github.com/zeroc-ice/ice-builder-visualstudio
[4]: https://doc.zeroc.com/display/Rel/Using+the+Windows+Binary+Distributions+for+Ice+3.7.1
