# DataStorm keyFilter

This demo illustrates the use of key filters. The reader uses the `_regex` predefined key filter. The reader will only
subscribe and receive samples for keys matching the regular expression provided on construction of the reader object.

To build the demo run:

```shell
cmake -B build -S .
cmake --build build --config Release
```

To run the demo, start the writer:

**Linux/macOS:**

```shell
./build/writer
```

**Windows:**

```shell
build\Release\writer
```

In a separate window, start the reader:

**Linux/macOS:**

```shell
./build/reader
```

**Windows:**

```shell
build\Release\reader
```
