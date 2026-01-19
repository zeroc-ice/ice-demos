# Scripts

## setup-nightly.py

Configures the repository to use Ice nightly packages instead of stable releases.

### Usage

```bash
# Configure all languages (default channel 3.8)
python scripts/setup-nightly.py

# Use a different channel
python scripts/setup-nightly.py --channel 3.7

# Reset all nightly changes
python scripts/setup-nightly.py --reset
```

After running, source the generated environment script:

```bash
source scripts/nightly-env.sh   # bash/zsh
.\scripts\nightly-env.ps1       # PowerShell
```

### What it does

- **C++**: Updates `vcxproj` and `packages.config` files with nightly Ice package versions
- **C#**: Creates `NuGet.Config` pointing to nightly NuGet feed
- **Java**: Creates `.gradle-nightly/` with init script for nightly Maven repository
- **JavaScript/TypeScript**: Sets npm registry and updates `package.json` to use `*` for Ice packages
- **Python**: Sets `UV_EXTRA_INDEX_URL` / `PIP_EXTRA_INDEX_URL` for nightly PyPI
- **Swift**: Updates `Package.swift` files to use `ice-swift-nightly` repository

Environment variables exported:
- `CHANNEL` - Ice channel version
- `ICE_NUGET_SOURCE` - NuGet source URL for C++ on Windows
- `ICE_NUGET_PRERELEASE` - Enable prerelease NuGet packages
- `IceVersion` - NuGet version pattern for C#
- `UV_EXTRA_INDEX_URL` / `PIP_EXTRA_INDEX_URL` - PyPI index for Python
- `GRADLE_USER_HOME` - Gradle home with nightly init script
- `MAVEN_REPO` - Maven repository URL for Java
