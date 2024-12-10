This demo illustrates the use of sample filters. The reader uses the `_regex`
predefined sample filter. The reader will only receive samples matching the
regular expression provided on construction of the reader object. Note that the
filtering of the samples is performed on the writer to minimize the number of
events sent over the wire.

To run the demo, start the writer:
```
writer
```

In a separate window, start the reader:
```
reader
```
