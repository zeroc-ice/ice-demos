# Filesystem

This demo implements the filesystem application shown at the end of Ruby mapping chapter in the Ice manual.

Ice for Ruby supports only client-side applications. As a result, you first need to start a Filesystem server
implemented in a language with server-side support, such as Python, Java, or C#.

Then, in a separate window:

- Compile Filesystem.ice with the Slice to Ruby compiler into Filesystem.rb

```shell
slice2rb Filesystem.ice
```

- Run the client application

```shell
ruby client.rb
```
