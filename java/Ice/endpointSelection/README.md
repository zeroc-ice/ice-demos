# Ice Endpoint Selection

This demo illustrates how Ice's **random endpoint selection** interacts with **connection caching** and **connection
IDs**. It helps explain why using a small pool of connection IDs with random endpoint selection can produce skewed load
distribution across replicated servers.

## The scenario

A client creates proxies to a service backed by 4 replicated servers. Each proxy is assigned a random connection ID
from a small pool (e.g., 11 values) to achieve "sticky" session affinity within short call sequences.

The client runs 5 scenarios to compare different configurations:

1. **Random + connection IDs (11 groups):** Each connection ID group randomly selects a server on first use, then
   sticks to that server. With only 11 groups across 4 servers, the "balls into bins" effect produces noticeable skew —
   some servers get more groups than others.

2. **Random + connection IDs (100 groups):** With more groups, the distribution becomes more even as the law of large
   numbers takes effect.

3. **Random + no connection IDs:** Without connection IDs, the default connection caching sends all 1000 requests
   through a single connection to one randomly selected server.

4. **Random + connection caching disabled:** With `ice_connectionCached(false)`, Ice doesn't prefer endpoints with
   established connections. Each invocation gets a connection selected based on the endpoint selection policy, producing
   a roughly uniform distribution.

5. **Pre-established connections with connection caching:** Pre-establish connections to all replicas explicitly (via
   `ice_ping` on single-endpoint proxies), then let each new proxy randomly pick among the established connections. This
   combines the efficiency of connection caching with uniform load distribution — no connection IDs needed. In
   production, you would also disable the idle timeout to keep the pre-established connections open indefinitely.

## Why skew happens with a small connection ID pool

When `ice_connectionCached(true)` (the default), Ice prefers reusing existing connections. A connection ID narrows which
connection is eligible for reuse: Ice looks for an existing connection to any endpoint whose connection ID matches.

On the **first** invocation with a given connection ID, no matching connection exists, so Ice selects an endpoint using
the configured endpoint selection policy (Random, in this case) and establishes a new connection.

On **subsequent** invocations with the same connection ID, Ice reuses the connection established on the first
invocation. The endpoint selection policy is not consulted again for this connection ID.

With 11 connection IDs and 4 servers, each group independently picks a random server. This is the classic "balls into
bins" probability problem. With 11 balls and 4 bins, uniform distribution is unlikely:

- The expected number of balls per bin is 2.75
- Standard deviation is about 1.4
- Some bins getting 1 ball while others get 4+ is common

This means some servers may handle 400 invocations while others handle only 100.

## Building the demo

The demo has two Gradle projects, **client** and **server**, both using the [application plugin].

To build the demo, run:

```shell
./gradlew build
```

## Running the demo

Start 4 server instances, each in its own terminal:

```shell
./gradlew :server:run --quiet --args='4061'
./gradlew :server:run --quiet --args='4062'
./gradlew :server:run --quiet --args='4063'
./gradlew :server:run --quiet --args='4064'
```

Then, in a separate terminal, start the client:

```shell
./gradlew :client:run --quiet
```

The client runs all 4 scenarios and prints the invocation distribution for each.

## Plotting the results

A Python script is included to automate the full demo and plot the load distribution across all 5 scenarios. It starts
the servers, runs the client, parses the output, and generates a bar chart.

```shell
pip install matplotlib
python plot_demo.py
```

[Application plugin]: https://docs.gradle.org/current/userguide/application_plugin.html
