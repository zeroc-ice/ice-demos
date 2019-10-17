# Ruby Demos

## Overview

This directory contains Ruby sample programs for various Ice components. These
examples are provided to get you started on using a particular Ice feature or
coding technique.

The following subdirectories contain sample programs:

- [Ice](./Ice) has several examples that illustrate fundamental Ice concepts.

- [IceDiscovery](./IceDiscovery) demonstrates the use of our multicast service
discovery plug-in.

- [Manual](./Manual) contains complete examples for some of the code snippets
in the [Ice manual][1].

Refer to the [C++11 demos](../cpp11) for more examples that use Ice services
such as IceGrid and IceStorm.

## Prerequisites

You'll need to install Ice for Ruby in order to use any of the sample programs.
Our [documentation][2] describes how to install the Ruby packages.

## Running the Demos

Refer to the README file in each demo directory for usage instructions.

Note: When running a client with Ruby 1.8, you must start the interpreter with
the `-rubygems` option as shown in the example below:

```
ruby -rubygems Client.rb
```

[1]: https://doc.zeroc.com/ice/3.7/introduction
[2]: https://doc.zeroc.com/ice/3.7/release-notes/using-the-ruby-distribution
