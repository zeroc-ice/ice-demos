# Scripts

## setup-nightly.py

Configures the repository to use Ice nightly packages instead of stable releases.

### Usage

```bash
# Configure all languages (default)
python scripts/setup-nightly.py

# Configure a single language
python scripts/setup-nightly.py --language csharp

# Use a different channel
python scripts/setup-nightly.py --channel 3.7
```

After running, source the generated environment script:

```bash
source scripts/nightly-env.sh   # bash/zsh
.\scripts\nightly-env.ps1       # PowerShell
```

### What it does

- **C++**: Creates `cpp/NuGet.Config` for Windows builds pointing to nightly NuGet feed
- **C#**: Creates `csharp/NuGet.Config` pointing to nightly NuGet feed
- **Java**: Creates `.gradle-nightly/` with init script for nightly Maven repository
- **JavaScript**: Creates `.npmrc` and updates `package.json` to use `*` for `@zeroc/*` versions
- **Python**: Sets `UV_EXTRA_INDEX_URL` / `PIP_EXTRA_INDEX_URL` for nightly PyPI
- **Swift**: Updates `Package.swift` files to use `ice-swift-nightly` repository
- **All**: Sets `IceVersion` environment variable for C# prerelease packages

### Resetting changes

To reset all nightly configuration:

```bash
python scripts/setup-nightly.py --reset
```

This removes generated files and restores `package.json` files via git.
