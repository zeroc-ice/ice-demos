#!/usr/bin/env python3
"""Runs the Ice endpointSelection demo and plots the load-balancing results."""

import subprocess
import sys
import time
import re
import os

try:
    import matplotlib.pyplot as plt
    import matplotlib.ticker as ticker
except ImportError:
    sys.exit("matplotlib is required. Install it with:  pip install matplotlib")

DEMO_DIR = os.path.dirname(os.path.abspath(__file__))
GRADLEW = os.path.join(DEMO_DIR, "gradlew")
PORTS = [4061, 4062, 4063, 4064]


def build():
    print("Building the demo...")
    subprocess.run(
        [GRADLEW, "build", "-x", "test"],
        cwd=DEMO_DIR,
        check=True,
        capture_output=True,
    )
    print("Build succeeded.")


def wait_for_ready(proc, port, timeout=60):
    """Read stdout lines until we see the adapter ready message."""
    deadline = time.monotonic() + timeout
    while time.monotonic() < deadline:
        line = proc.stdout.readline()
        if not line:
            if proc.poll() is not None:
                # Drain stderr for diagnostics.
                stderr = proc.stderr.read() if proc.stderr else ""
                sys.exit(
                    f"Server on port {port} exited unexpectedly (code {proc.returncode})\n"
                    f"stderr: {stderr}"
                )
            continue
        print(f"  [server:{port}] {line}", end="")
        if "ready" in line.lower():
            return
    sys.exit(f"Timed out waiting for server on port {port}")


def start_servers():
    """Start 4 server instances and return their Popen handles."""
    servers = []
    for port in PORTS:
        p = subprocess.Popen(
            [
                GRADLEW, ":server:run", "--quiet",
                f"--args={port} --Ice.PrintAdapterReady=1",
            ],
            cwd=DEMO_DIR,
            stdout=subprocess.PIPE,
            stderr=subprocess.PIPE,
            text=True,
        )
        servers.append(p)

    # Wait until every server prints its adapter ready message.
    print("Waiting for servers to be ready...")
    for p, port in zip(servers, PORTS):
        wait_for_ready(p, port)

    print(f"Started {len(servers)} servers on ports {PORTS}.")
    return servers


def run_client():
    """Run the client and return its stdout."""
    print("Running the client...")
    result = subprocess.run(
        [GRADLEW, ":client:run", "--quiet"],
        cwd=DEMO_DIR,
        capture_output=True,
        text=True,
        timeout=120,
    )
    if result.returncode != 0:
        print("Client stderr:", result.stderr, file=sys.stderr)
        sys.exit(f"Client exited with code {result.returncode}")
    return result.stdout


def stop_servers(servers):
    for p in servers:
        p.terminate()
    for p in servers:
        try:
            p.wait(timeout=10)
        except subprocess.TimeoutExpired:
            p.kill()
    print("Servers stopped.")


# ---------------------------------------------------------------------------
# Output parsing
# ---------------------------------------------------------------------------

# Matches scenario headers like:
#   === Random endpoint selection with connection IDs (11 groups) ===
HEADER_RE = re.compile(r"^===\s+(.+?)\s+===$")

# Matches connection count lines like:
#   Connections: 11
CONN_RE = re.compile(r"^Connections:\s+(\d+)")

# Matches distribution lines like:
#   127.0.0.1:4061 -> 250 (25.0%)
DIST_RE = re.compile(r"^\s+(\S+)\s+->\s+(\d+)\s+\([\d.]+%\)")


def parse_output(text):
    """Return a list of (title, connection_count, {server: count}) tuples, one per scenario."""
    scenarios = []
    current_title = None
    current_counts = {}
    current_connections = 0

    for line in text.splitlines():
        hdr = HEADER_RE.match(line)
        if hdr:
            # Save previous scenario if any.
            if current_title and current_counts:
                scenarios.append((current_title, current_connections, current_counts))
            current_title = hdr.group(1)
            current_counts = {}
            current_connections = 0
            continue

        conn = CONN_RE.match(line)
        if conn:
            current_connections = int(conn.group(1))
            continue

        dist = DIST_RE.match(line)
        if dist:
            server, count = dist.group(1), int(dist.group(2))
            current_counts[server] = count

    # Don't forget the last scenario.
    if current_title and current_counts:
        scenarios.append((current_title, current_connections, current_counts))

    return scenarios


# ---------------------------------------------------------------------------
# Plotting
# ---------------------------------------------------------------------------

COLORS = ["#4e79a7", "#f28e2b", "#e15759", "#76b7b2"]


def plot(scenarios):
    n = len(scenarios)
    cols = min(n, 3)
    rows = (n + cols - 1) // cols
    fig, axes = plt.subplots(rows, cols, figsize=(5 * cols, 5 * rows), sharey=True)
    axes = axes.flatten() if n > 1 else [axes]

    for ax, (title, num_connections, counts) in zip(axes, scenarios):
        labels = [s.split(":")[-1] for s in counts]  # show just the port
        values = list(counts.values())
        bars = ax.bar(labels, values, color=COLORS[: len(labels)], edgecolor="white")

        # Annotate each bar with its count.
        for bar, v in zip(bars, values):
            ax.text(
                bar.get_x() + bar.get_width() / 2,
                bar.get_height() + 8,
                str(v),
                ha="center",
                va="bottom",
                fontsize=10,
                fontweight="bold",
            )

        ax.set_title(f"{title}\n({num_connections} connection{'s' if num_connections != 1 else ''})",
                      fontsize=10, pad=10, wrap=True)
        ax.set_xlabel("Server port")
        ax.yaxis.set_major_locator(ticker.MaxNLocator(integer=True))

    # Hide any unused subplot slots.
    for ax in axes[n:]:
        ax.set_visible(False)

    axes[0].set_ylabel("Invocations")
    fig.suptitle("Ice Endpoint Selection — Load Distribution (1000 requests)", fontsize=13, y=1.02)
    fig.tight_layout()
    out = os.path.join(DEMO_DIR, "load_distribution.png")
    fig.savefig(out, dpi=150, bbox_inches="tight")
    print(f"Plot saved to {out}")
    plt.show()


# ---------------------------------------------------------------------------
# Main
# ---------------------------------------------------------------------------

def main():
    build()
    servers = start_servers()
    try:
        output = run_client()
    finally:
        stop_servers(servers)

    print("\n--- Client output ---")
    print(output)

    scenarios = parse_output(output)
    if not scenarios:
        sys.exit("Could not parse any scenario from the client output.")

    plot(scenarios)


if __name__ == "__main__":
    main()
