# Ice Filesystem

This demo shows the power of interface inheritance in Slice. In particular, it demonstrates how a derived servant class
can inherit from a base servant class to reuse the Slice interface implementation provided by this base servant.

> [!NOTE]
> In Ice for C++, Java, JavaScript, and Python, reusing a base servant in a derived servant doesn't require extra
> coding: it's completely automatic. In Swift, you need to define and implement the `dispatch` method as shown in
> [MNode.swift][1], [MFile.swift][2], and [MDirectory.swift][3].

You can build the client and server applications with:

```shell
swift build
```

You can start the Server program with:

```shell
swift run Server
```

Then, in a separate terminal, start the Client program with:

```shell
swift run Client
```

[1]: Sources/Server/MNode.swift
[2]: Sources/Server/MFile.swift
[3]: Sources/Server/MDirectory.swift
