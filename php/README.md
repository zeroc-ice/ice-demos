# PHP Demos

## Overview

This directory contains PHP sample programs for various Ice components. These
examples are provided to get you started on using a particular Ice feature or
coding technique.

The following subdirectories contain sample programs:

- [Chat](./Chat) contains a GUI client for the ZeroC [Chat Demo][1].

- [Glacier2](./Glacier2) has an example that maintains a Glacier2 session.

- [Ice](./Ice) has several examples that illustrate fundamental Ice concepts.

- [IceDiscovery](./IceDiscovery) demonstrates the use of our multicast service
discovery plug-in.

### Building the Demos

```
php build.php
```

If you have installed Ice in a non-standard location, you'll need to set
the `ICE_HOME` environment variable with the path name of the installation
directory:

#### Linux and macOS

```
export ICE_HOME=/opt/Ice-3.7
```

#### Windows

```
set ICE_HOME=C:\Ice-3.7.2
```

## Running the Demos

Refer to the README file in each demo directory for usage instructions.

[1]: https://zeroc.com/chat/index.html
