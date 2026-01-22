#!/bin/bash
if [ -d /ice-demos/scripts ]; then
    if [ ! -f /ice-demos/scripts/nightly-env.sh ]; then
        python3 /ice-demos/scripts/setup-nightly.py --channel 3.7 2>/dev/null || true
    fi
    if [ -f /ice-demos/scripts/nightly-env.sh ]; then
        source /ice-demos/scripts/nightly-env.sh
    fi
fi
