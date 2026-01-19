#!/usr/bin/env python3
# Copyright (c) ZeroC, Inc.

"""
Setup helpers for the nightly channel.

This script configures the repository to use nightly/prerelease package feeds.

Usage:
    # Configure all languages with default channel (3.8)
    python scripts/setup-nightly.py

    # Configure for a different channel
    python scripts/setup-nightly.py --channel 3.7

    # CI usage (exports to GITHUB_ENV)
    python scripts/setup-nightly.py --github-env "$GITHUB_ENV"

    # Reset all nightly changes
    python scripts/setup-nightly.py --reset
"""

from __future__ import annotations

import argparse
import json
import os
import pathlib
import re
import shutil
import subprocess
import sys

REPO_ROOT = pathlib.Path(__file__).resolve().parents[1]
DEFAULT_CHANNEL = "3.8"

# Templates for generated config files
GRADLE_INIT_TEMPLATE = """\
beforeSettings {{ settings ->
    settings.pluginManagement {{ pluginManagement ->
        pluginManagement.repositories {{
            maven {{ url = uri("{maven_url}") }}
            gradlePluginPortal()
        }}
    }}
    settings.dependencyResolutionManagement {{ dependencyResolutionManagement ->
        dependencyResolutionManagement.repositories {{
            maven {{ url = uri("{maven_url}") }}
            mavenCentral()
            google()
        }}
    }}
}}
"""

NUGET_CONFIG_TEMPLATE = """\
<?xml version="1.0" encoding="utf-8"?>
<configuration>
  <!-- Define the package sources, nuget.org and zeroc.com. -->
  <!-- `clear` ensures no additional sources are inherited from another config file. -->
  <packageSources>
    <clear />
    <add key="nuget.org" value="https://api.nuget.org/v3/index.json" />
    <add key="zeroc.com" value="{nuget_url}" />
  </packageSources>

  <!-- Define mappings by adding package patterns beneath the target source. -->
  <!-- ZeroC C++ and .NET packages will be restored from zeroc.com, everything else from nuget.org. -->
  <packageSourceMapping>
    <packageSource key="nuget.org">
      <package pattern="*" />
    </packageSource>
    <packageSource key="zeroc.com">
      <package pattern="zeroc.ice.v142" />
      <package pattern="zeroc.ice.v143" />
      <package pattern="zeroc.ice.net" />
    </packageSource>
  </packageSourceMapping>
</configuration>
"""


def get_urls(channel: str) -> dict[str, str]:
    """Generate nightly repository URLs for a given channel."""
    base = "https://download.zeroc.com/nexus/repository"
    return {
        "pypi": f"{base}/pypi-{channel}-nightly/simple/",
        "maven": f"{base}/maven-{channel}-nightly/",
        "npm": f"{base}/npm-{channel}-nightly/",
        "nuget": f"{base}/nuget-{channel}-nightly/",
    }


def get_latest_nuget_version(channel: str, package_name: str) -> str | None:
    """Query NuGet feed for the latest version of a package."""
    import urllib.request

    urls = get_urls(channel)
    registration_url = f"{urls['nuget']}v3/registration/{package_name.lower()}/index.json"

    try:
        with urllib.request.urlopen(registration_url, timeout=30) as response:
            data = json.loads(response.read().decode("utf-8"))
            # Navigate: items[0].items[-1].catalogEntry.version (last item is latest)
            items = data.get("items", [])
            if items and items[0].get("items"):
                versions = items[0]["items"]
                return versions[-1]["catalogEntry"]["version"]
    except Exception as e:
        print(f"Warning: Could not query NuGet feed for {package_name}: {e}")
    return None


def write_file(path: pathlib.Path, content: str) -> None:
    """Write content to a file, creating parent directories if needed."""
    path.parent.mkdir(parents=True, exist_ok=True)
    path.write_text(content, encoding="utf-8")


def configure_nuget(channel: str) -> None:
    """Configure NuGet to use nightly feed (for both C++ and C#)."""
    urls = get_urls(channel)
    content = NUGET_CONFIG_TEMPLATE.format(nuget_url=urls["nuget"])
    dest = REPO_ROOT / "NuGet.Config"
    write_file(dest, content)
    print(f"Created {dest.relative_to(REPO_ROOT)}")


def configure_java(channel: str) -> pathlib.Path:
    """Configure Gradle to use nightly Maven repository."""
    urls = get_urls(channel)
    content = GRADLE_INIT_TEMPLATE.format(maven_url=urls["maven"])
    gradle_home = REPO_ROOT / ".gradle-nightly"
    dest = gradle_home / "init.d" / "nightly.init.gradle"
    write_file(dest, content)
    print(f"Created {dest.relative_to(REPO_ROOT)}")
    return gradle_home


def configure_js(channel: str) -> None:
    """Configure npm to use nightly registry and prerelease versions for Ice packages."""
    urls = get_urls(channel)

    # Set global npm registry to nightly feed
    # Use shell=True for Windows compatibility (npm is a batch script on Windows)
    npm_cmd = f'npm config set registry {urls["npm"]}'
    print(f"Running: {npm_cmd}")
    result = subprocess.run(npm_cmd, shell=True, capture_output=True, text=True)
    if result.returncode != 0:
        print(f"npm config set failed: {result.stderr}")
        raise RuntimeError(f"npm config set failed: {result.stderr}")

    # Ice packages to update (non-scoped packages used in 3.7)
    ice_packages = {"ice", "slice2js"}

    # Glob patterns for JavaScript and TypeScript demo package.json files
    patterns = ["js/**/package.json", "typescript/**/package.json"]

    count = 0
    for pattern in patterns:
        for pkg_path in REPO_ROOT.glob(pattern):
            # Skip node_modules directories
            if "node_modules" in pkg_path.parts:
                continue

            # Update package.json to use * for Ice packages (allows prerelease versions)
            pkg_data = json.loads(pkg_path.read_text(encoding="utf-8"))
            modified = False

            for dep_type in ["dependencies", "devDependencies"]:
                if dep_type in pkg_data:
                    for dep_name in pkg_data[dep_type]:
                        if dep_name in ice_packages:
                            pkg_data[dep_type][dep_name] = "*"
                            modified = True

            if modified:
                pkg_path.write_text(json.dumps(pkg_data, indent=2) + "\n", encoding="utf-8")
                count += 1

                # Remove package-lock.json so npm install picks up new versions
                lock_path = pkg_path.parent / "package-lock.json"
                if lock_path.exists():
                    lock_path.unlink()
                    print(f"Removed {lock_path.relative_to(REPO_ROOT)}")

    print(f"Configured npm registry: {urls['npm']}")
    print(f"Updated {count} JS/TypeScript package.json files with nightly config")


def configure_swift(channel: str) -> None:
    """Configure Swift Package.swift files to use nightly repository."""
    # Pattern to match the ice.git URL in dependencies
    url_pattern = re.compile(
        r'\.package\(url:\s*"https://github\.com/zeroc-ice/ice\.git"'
    )
    url_replacement = '.package(url: "https://github.com/zeroc-ice/ice-swift-nightly"'

    # Pattern to match package: "ice" references (the package name changes with the repo)
    pkg_pattern = re.compile(r'package:\s*"ice"')
    pkg_replacement = 'package: "ice-swift-nightly"'

    count = 0
    for pkg_path in REPO_ROOT.glob("swift/**/Package.swift"):
        content = pkg_path.read_text(encoding="utf-8")
        if url_pattern.search(content):
            new_content = url_pattern.sub(url_replacement, content)
            new_content = pkg_pattern.sub(pkg_replacement, new_content)
            pkg_path.write_text(new_content, encoding="utf-8")
            count += 1

    print(f"Updated {count} Swift Package.swift files with nightly repository")


def configure_cpp(channel: str, cpp_version: str | None = None) -> None:
    """Update vcxproj and packages.config files to use nightly Ice package versions."""
    # Use provided version or query for the latest nightly version
    if cpp_version:
        nightly_version = cpp_version
    else:
        nightly_version = get_latest_nuget_version(channel, "zeroc.ice.v143")
        if not nightly_version:
            print("Warning: Could not determine nightly C++ version, skipping C++ updates")
            return

    # Update vcxproj files
    # Pattern matches: zeroc.ice.v142.X.Y.Z or zeroc.ice.v143.X.Y.Z in paths
    # Version starts with digit after toolset, excludes .props/.targets file names
    ice_pattern = re.compile(r"zeroc\.ice\.v(\d{3})\.(\d[0-9a-zA-Z.\-]*)")

    def ice_replacement(match: re.Match[str]) -> str:
        toolset = match.group(1)  # Preserve v142 or v143
        return f"zeroc.ice.v{toolset}.{nightly_version}"

    vcxproj_count = 0
    for vcxproj in REPO_ROOT.glob("cpp*/**/*.vcxproj"):
        content = vcxproj.read_text(encoding="utf-8")
        new_content = ice_pattern.sub(ice_replacement, content)
        if new_content != content:
            vcxproj.write_text(new_content, encoding="utf-8")
            vcxproj_count += 1

    # Update packages.config files
    # Pattern matches version attribute: version="X.Y.Z" for zeroc.ice packages
    pkg_pattern = re.compile(r'(<package\s+id="zeroc\.ice\.v\d{3}"\s+version=")[^"]+(")')

    def pkg_replacement(match: re.Match[str]) -> str:
        return f"{match.group(1)}{nightly_version}{match.group(2)}"

    config_count = 0
    for config in REPO_ROOT.glob("cpp*/**/packages.config"):
        content = config.read_text(encoding="utf-8")
        new_content = pkg_pattern.sub(pkg_replacement, content)
        if new_content != content:
            config.write_text(new_content, encoding="utf-8")
            config_count += 1

    print(f"Updated {vcxproj_count} vcxproj and {config_count} packages.config files with Ice version {nightly_version}")


def configure_env(channel: str, github_env: pathlib.Path | None, gradle_home: pathlib.Path) -> None:
    """Configure environment variables for nightly builds."""
    urls = get_urls(channel)
    env_vars = {
        "CHANNEL": channel,
        "ICE_NUGET_SOURCE": urls["nuget"],  # NuGet source for C++ on Windows
        "ICE_NUGET_PRERELEASE": "1",  # Enable -Prerelease flag for C++ NuGet
        "IceVersion": f"{channel}.*-*",  # NuGet prerelease version pattern for C#
        "UV_EXTRA_INDEX_URL": urls["pypi"],
        "PIP_EXTRA_INDEX_URL": urls["pypi"],
        "GRADLE_USER_HOME": str(gradle_home),
        "npm_config_@zeroc:registry": urls["npm"],  # npm registry for @zeroc packages
    }

    if github_env:
        # CI mode: append to GITHUB_ENV file
        github_env.parent.mkdir(parents=True, exist_ok=True)
        with github_env.open("a", encoding="utf-8") as fh:
            for key, value in env_vars.items():
                fh.write(f"{key}={value}\n")
        print(f"Exported environment variables to {github_env}")
    else:
        # Local mode: create shell scripts
        write_local_env_scripts(env_vars)


def write_local_env_scripts(env_vars: dict[str, str]) -> None:
    """Write local environment scripts for bash and PowerShell."""
    scripts_dir = REPO_ROOT / "scripts"
    scripts_dir.mkdir(parents=True, exist_ok=True)

    # Bash script
    sh_path = scripts_dir / "nightly-env.sh"
    sh_lines = ["#!/usr/bin/env bash"]
    sh_lines.extend(f'export {key}="{value}"' for key, value in env_vars.items())
    write_file(sh_path, "\n".join(sh_lines) + "\n")

    # PowerShell script
    ps1_path = scripts_dir / "nightly-env.ps1"
    ps1_lines = ["# PowerShell environment for nightly"]
    for key, value in env_vars.items():
        # Use [Environment]::SetEnvironmentVariable for names with special characters
        if any(c in key for c in "@:"):
            ps1_lines.append(f'[Environment]::SetEnvironmentVariable("{key}", "{value}", "Process")')
        else:
            ps1_lines.append(f'$env:{key}="{value}"')
    write_file(ps1_path, "\n".join(ps1_lines) + "\n")

    # Print path relative to current directory so user can copy-paste
    cwd = pathlib.Path.cwd()
    if os.name == "nt":
        rel_path = os.path.relpath(ps1_path, cwd)
        print(f"Activate nightly builds with: .\\{rel_path}")
    else:
        rel_path = os.path.relpath(sh_path, cwd)
        print(f"Activate nightly builds with: source {rel_path}")


def parse_args() -> argparse.Namespace:
    parser = argparse.ArgumentParser(
        description="Configure nightly feeds for all languages.",
        formatter_class=argparse.RawDescriptionHelpFormatter,
        epilog="""
Examples:
  %(prog)s                             # Configure all languages
  %(prog)s --channel 3.7               # Use 3.7 nightly channel
  %(prog)s --cpp-version 3.7.11        # Use specific C++ package version
  %(prog)s --reset                     # Reset all nightly changes
  %(prog)s --github-env "$GITHUB_ENV"  # CI mode
""",
    )
    parser.add_argument(
        "-c",
        "--channel",
        default=DEFAULT_CHANNEL,
        help=f"Ice channel version (default: {DEFAULT_CHANNEL})",
    )
    parser.add_argument(
        "--github-env",
        type=pathlib.Path,
        default=None,
        help="Path to GITHUB_ENV file for CI. If omitted, creates local shell scripts.",
    )
    parser.add_argument(
        "--reset",
        action="store_true",
        help="Reset all nightly changes (remove generated files, restore package.json via git).",
    )
    parser.add_argument(
        "--cpp-version",
        default=None,
        help="C++ NuGet package version to use. If omitted, queries the nightly feed for the latest version.",
    )
    return parser.parse_args()


def reset_nightly() -> None:
    """Reset all nightly configuration changes."""
    removed = []

    # Reset npm registry to default
    # Use shell=True for Windows compatibility (npm is a batch script on Windows)
    result = subprocess.run(
        "npm config delete registry",
        capture_output=True,
        text=True,
        shell=True,
    )
    if result.returncode == 0:
        print("Reset npm registry to default")

    # Remove NuGet.Config file (used by both C++ and C#)
    nuget_config = REPO_ROOT / "NuGet.Config"
    if nuget_config.exists():
        nuget_config.unlink()
        removed.append(nuget_config.relative_to(REPO_ROOT))

    # Remove .gradle-nightly directory
    gradle_dir = REPO_ROOT / ".gradle-nightly"
    if gradle_dir.exists():
        shutil.rmtree(gradle_dir)
        removed.append(gradle_dir.relative_to(REPO_ROOT))

    # Remove env scripts
    for script in ["nightly-env.sh", "nightly-env.ps1"]:
        path = REPO_ROOT / "scripts" / script
        if path.exists():
            path.unlink()
            removed.append(path.relative_to(REPO_ROOT))

    # Reset package.json and package-lock.json files using git
    result = subprocess.run(
        ["git", "checkout", "--", "js/**/package.json", "js/**/package-lock.json",
         "typescript/**/package.json", "typescript/**/package-lock.json"],
        cwd=REPO_ROOT,
        capture_output=True,
        text=True,
    )
    if result.returncode == 0:
        print("Restored js/**/package*.json and typescript/**/package*.json via git checkout")
    elif "error" in result.stderr.lower():
        print(f"Note: Could not restore package.json files: {result.stderr.strip()}")

    # Reset Swift Package.swift files using git (only Package.swift, not entire swift/ directory)
    result = subprocess.run(
        ["git", "checkout", "--", "swift/**/Package.swift"],
        cwd=REPO_ROOT,
        capture_output=True,
        text=True,
    )
    if result.returncode == 0:
        print("Restored swift/**/Package.swift via git checkout")
    elif "error" in result.stderr.lower():
        print(f"Note: Could not restore Package.swift files: {result.stderr.strip()}")

    # Reset vcxproj files using git
    result = subprocess.run(
        ["git", "checkout", "--", "cpp11/**/*.vcxproj", "cpp98/**/*.vcxproj"],
        cwd=REPO_ROOT,
        capture_output=True,
        text=True,
    )
    if result.returncode == 0:
        print("Restored cpp*/**/*.vcxproj via git checkout")
    elif "error" in result.stderr.lower():
        print(f"Note: Could not restore vcxproj files: {result.stderr.strip()}")

    # Reset packages.config files using git
    result = subprocess.run(
        ["git", "checkout", "--", "cpp11/**/packages.config", "cpp98/**/packages.config"],
        cwd=REPO_ROOT,
        capture_output=True,
        text=True,
    )
    if result.returncode == 0:
        print("Restored cpp*/**/packages.config via git checkout")
    elif "error" in result.stderr.lower():
        print(f"Note: Could not restore packages.config files: {result.stderr.strip()}")

    if removed:
        print(f"Removed: {', '.join(str(p) for p in removed)}")
    print("Nightly configuration reset complete.")


def main() -> int:
    args = parse_args()

    if args.reset:
        reset_nightly()
        return 0

    print(f"Configuring nightly feeds for channel {args.channel}...")

    configure_nuget(args.channel)
    gradle_home = configure_java(args.channel)
    configure_js(args.channel)
    configure_swift(args.channel)
    configure_cpp(args.channel, args.cpp_version)
    configure_env(args.channel, args.github_env, gradle_home)

    return 0


if __name__ == "__main__":
    sys.exit(main())
