// Copyright (c) ZeroC, Inc.

package com.example.ice.endpointselection.client;

import com.example.visitorcenter.GreeterPrx;
import com.zeroc.Ice.Communicator;
import com.zeroc.Ice.Connection;
import com.zeroc.Ice.Endpoint;
import com.zeroc.Ice.EndpointSelectionType;
import com.zeroc.Ice.ObjectPrx;
import com.zeroc.Ice.TCPConnectionInfo;

import java.util.Collections;
import java.util.LinkedHashMap;
import java.util.Map;
import java.util.Set;

/**
 * This demo illustrates how Ice's random endpoint selection interacts with connection caching and connection IDs.
 *
 * <p>It creates a proxy with 4 endpoints (simulating 4 replicated servers), then sends 1000 requests using connection
 * IDs to group invocations — a pattern commonly used for session affinity.
 *
 * <p>The demo shows that with a small number of connection ID groups (e.g., 11) distributed randomly across a small
 * number of servers (e.g., 4), significant skew is expected. This is a consequence of how random assignment works with
 * small sample sizes — not a bug.
 */
class Client {
    public static void main(String[] args) {
        // Create an Ice communicator. We'll use this communicator to create proxies and manage outgoing connections.
        try (Communicator communicator = new Communicator(args)) {

            // Create a proxy to the Greeter object with 4 endpoints, simulating 4 replicated servers.
            // If you run the servers on different hosts, replace "localhost" with the appropriate hostnames.
            var greeter = GreeterPrx.createProxy(
                communicator,
                "greeter:tcp -h localhost -p 4061:tcp -h localhost -p 4062:tcp -h localhost -p 4063:tcp -h localhost -p 4064");

            // Select endpoints randomly. This is the default for indirect proxies resolved by IceGrid with a Random
            // load-balancing policy.
            greeter = GreeterPrx.uncheckedCast(greeter.ice_endpointSelection(EndpointSelectionType.Random));

            System.out.println("=== Random endpoint selection with connection IDs (11 groups) ===");
            System.out.println("This simulates a proxy factory that assigns a random connection ID from a small pool.");
            System.out.println("Each connection ID group gets randomly assigned to a server on first use,");
            System.out.println("then sticks to that server due to connection caching.\n");

            runWithConnectionIds(greeter, 1000, 11);

            System.out.println("\n=== Random endpoint selection with connection IDs (100 groups) ===");
            System.out.println("With more connection ID groups, the distribution becomes more even.\n");

            runWithConnectionIds(greeter, 1000, 100);

            System.out.println("\n=== Random endpoint selection without connection IDs ===");
            System.out.println("Without connection IDs, the default connection caching sends all requests");
            System.out.println("through a single connection to one server.\n");

            runWithoutConnectionIds(greeter, 1000);

            System.out.println("\n=== Random endpoint selection with connection caching disabled ===");
            System.out.println("With connection caching disabled, Ice doesn't prefer existing connections.");
            System.out.println("Each invocation gets a connection based on the endpoint selection policy.\n");

            runWithCachingDisabled(greeter, 1000);

            System.out.println("\n=== Pre-established connections with connection caching ===");
            System.out.println("Pre-establish connections to all replicas, then each new proxy randomly picks");
            System.out.println("among the established connections and caches its selection.\n");

            runWithPreEstablishedConnections(greeter, 1000);
        }
    }

    /**
     * Sends {@code totalInvocations} requests using random connection IDs from a pool of {@code groupCount} groups.
     * Each call creates a new proxy with a random connection ID from a small pool, simulating session affinity.
     *
     * <p>With connection caching enabled (the default), the first invocation with a given connection ID randomly
     * selects
     * a server and establishes a connection. Subsequent invocations with the same connection ID reuse that connection.
     * With 11 groups across 4 servers, each group independently picks a random server, so the distribution across
     * servers follows the "balls into bins" probability: some servers will receive more groups than others.
     */
    private static void runWithConnectionIds(GreeterPrx greeter, int totalInvocations, int groupCount) {
        Map<String, Integer> serverCounts = new LinkedHashMap<>();
        Set<Connection> connections = Collections.newSetFromMap(new java.util.IdentityHashMap<>());

        for (int i = 0; i < totalInvocations; i++) {
            // Assign a random connection ID from a small pool to simulate session affinity.
            String connectionId = "conn-" + (int) (Math.random() * groupCount) + "G";
            GreeterPrx proxy = GreeterPrx.uncheckedCast(greeter.ice_connectionId(connectionId));

            proxy.greet("client");

            connections.add(proxy.ice_getConnection());
            String server = getRemoteAddress(proxy);
            serverCounts.merge(server, 1, Integer::sum);
        }

        printDistribution(serverCounts, totalInvocations, connections.size());
    }

    /**
     * Sends {@code totalInvocations} requests using the default proxy configuration (connection caching enabled, no
     * connection ID). All invocations reuse the same connection to a single randomly selected server.
     */
    private static void runWithoutConnectionIds(GreeterPrx greeter, int totalInvocations) {
        Map<String, Integer> serverCounts = new LinkedHashMap<>();
        Set<Connection> connections = Collections.newSetFromMap(new java.util.IdentityHashMap<>());

        for (int i = 0; i < totalInvocations; i++) {
            greeter.greet("client");

            connections.add(greeter.ice_getConnection());
            String server = getRemoteAddress(greeter);
            serverCounts.merge(server, 1, Integer::sum);
        }

        printDistribution(serverCounts, totalInvocations, connections.size());
    }

    /**
     * Sends {@code totalInvocations} requests with connection caching disabled. Without caching, Ice does not prefer
     * endpoints with established connections when selecting a connection for the proxy. With random endpoint selection,
     * this gives a roughly uniform distribution across all servers.
     */
    private static void runWithCachingDisabled(GreeterPrx greeter, int totalInvocations) {
        Map<String, Integer> serverCounts = new LinkedHashMap<>();
        Set<Connection> connections = Collections.newSetFromMap(new java.util.IdentityHashMap<>());

        // Disable connection caching: Ice won't prefer endpoints with already-established connections.
        GreeterPrx proxy = GreeterPrx.uncheckedCast(greeter.ice_connectionCached(false));

        for (int i = 0; i < totalInvocations; i++) {
            proxy.greet("client");

            connections.add(proxy.ice_getConnection());
            String server = getRemoteAddress(proxy);
            serverCounts.merge(server, 1, Integer::sum);
        }

        printDistribution(serverCounts, totalInvocations, connections.size());
    }

    /**
     * Pre-establishes connections to all replicas, then sends {@code totalInvocations} requests. Each invocation
     * creates
     * a fresh proxy that randomly picks among the established connections and caches its selection.
     *
     * <p>This approach gives a uniform distribution (like disabling connection caching) while still benefiting from
     * connection caching: each proxy sticks to its selected server. In a real application, different parts of the code
     * would hold their own proxy instances, each cached to a random server.
     *
     * <p>In production, you would also disable the idle timeout (e.g., set it to 0) to ensure the pre-established
     * connections remain open indefinitely.
     */
    private static void runWithPreEstablishedConnections(GreeterPrx greeter, int totalInvocations) {
        Map<String, Integer> serverCounts = new LinkedHashMap<>();
        Set<Connection> connections = Collections.newSetFromMap(new java.util.IdentityHashMap<>());

        // Step 1: Pre-establish a connection to each replica by pinging a single-endpoint proxy.
        for (Endpoint endpoint : greeter.ice_getEndpoints()) {
            GreeterPrx single = GreeterPrx.uncheckedCast(greeter.ice_endpoints(new Endpoint[]{endpoint}));
            single.ice_ping();
        }

        // Step 2: Each invocation creates a fresh proxy. Since connections to all endpoints already exist,
        // the random endpoint selection picks one and caches it immediately.
        String proxyString = greeter.toString();
        Communicator communicator = greeter.ice_getCommunicator();
        for (int i = 0; i < totalInvocations; i++) {
            GreeterPrx fresh = GreeterPrx.createProxy(communicator, proxyString);
            fresh = GreeterPrx.uncheckedCast(fresh.ice_endpointSelection(EndpointSelectionType.Random));

            fresh.greet("client");

            connections.add(fresh.ice_getConnection());
            String server = getRemoteAddress(fresh);
            serverCounts.merge(server, 1, Integer::sum);
        }

        printDistribution(serverCounts, totalInvocations, connections.size());
    }

    /**
     * Returns the remote address (host:port) of the connection used by the given proxy.
     */
    private static String getRemoteAddress(ObjectPrx proxy) {
        try {
            Connection connection = proxy.ice_getConnection();
            if (connection != null && connection.getInfo() instanceof TCPConnectionInfo tcpInfo) {
                return tcpInfo.remoteAddress + ":" + tcpInfo.remotePort;
            }
        } catch (Exception e) {
            // ignore
        }
        return "unknown";
    }

    /**
     * Prints the distribution of invocations across servers.
     */
    private static void printDistribution(
            Map<String, Integer> serverCounts, int totalInvocations, int connectionCount) {
        System.out.println("Connections: " + connectionCount);
        System.out.println("Invocation distribution across servers:");
        serverCounts.forEach((server, count) -> {
            double percent = 100.0 * count / totalInvocations;
            System.out.printf("  %s -> %d (%.1f%%)%n", server, count, percent);
        });
    }
}
