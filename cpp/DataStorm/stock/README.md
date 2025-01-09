This demo illustrates the use of partial updates. The writer adds stocks to the
topic and send partial updates to update the stock price or volume. The reader
prints out the stock information and partial updates.

The demo uses Slice to define the `Demo::Stock` class in the `Stock.ice` file.

To run the demo, start the writer and specify the ticker of the stocks to write:

```
writer
```

In a separate window, start the reader and specify the stock tickers you wish to
follow:

```
reader
```

You can start multiple writers and readers to publish or follow different
stocks.