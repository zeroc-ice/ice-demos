#!/bin/bash
# Setup script for Ice demos - configures environment based on quality (stable/nightly)
# This script is sourced on login shell startup via /etc/profile.d/

if [ -d /ice-demos/scripts ]; then
    # Only configure nightly feeds for nightly builds
    if [ "$QUALITY" = "nightly" ]; then
        if [ ! -f /ice-demos/scripts/nightly-env.sh ]; then
            python3 /ice-demos/scripts/setup-nightly.py --channel 3.7 2>/dev/null || true
        fi
        if [ -f /ice-demos/scripts/nightly-env.sh ]; then
            source /ice-demos/scripts/nightly-env.sh
        fi
    fi
fi
