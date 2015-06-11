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

Refer to the [C++ demos](../cpp) for more examples that use the Ice services
(Glacier2, IceGrid, IceStorm).

## Build Instructions

### Prerequisites

The projects for the sample programs require the [Ice Builder for Visual Studio][3].
Add this extension to Visual Studio before opening the solution.

### Building the Demos

Open the solution file `CSharp demos.sln` to build the sample programs.
Right click on the desired demo in the Solution Explorer window and select "Build".

## Running the Demos

Before running a demo, make sure you've configured your environment to use Ice
as described in the [release notes][4].

Refer to the README file in each demo directory for usage instructions.

[1]: https://doc.zeroc.com/display/Ice36/Ice+Manual
[2]: https://zeroc.com/chat/index.html
[3]: https://github.com/zeroc-ice/ice-builder-visualstudio
[4]: https://doc.zeroc.com/display/Ice36/Ice+Release+Notes
