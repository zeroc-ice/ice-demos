The source code provided here implements a C++ evictor base class. To
use it, you must derive a class from `EvictorBase` and implement the
pure virtual functions `add` and `evict`.

`add` is called when the evictor needs to instantiate a servant, and
`evict` is called when the evictor has decided to evict a servant.
Note that you can pass information from `add` to the corresponding
call to `evict` via the cookie argument.

For more details on this evictor, see [Servant Evictors][1].

[1]: https://doc.zeroc.com/ice/4.0/best-practices/servant-evictors
