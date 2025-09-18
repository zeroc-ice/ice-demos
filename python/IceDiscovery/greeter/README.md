# IceDiscovery Greeter

This demo illustrates how to configure the IceDiscovery plug-in.  
The IceDiscovery plug-in allows a client application to discover Ice objects without hardcoding any addressing information.

We recommend running each program in a separate Python virtual environment.  
If you are new to Python virtual environments, see [Python Virtual Environments].

## Running the server

Navigate to the `server` or `server_asyncio` directory, depending on whether you want to run the synchronous or asynchronous server.

### 1. Create and activate a Python virtual environment

#### macOS and Linux

```bash
python3 -m venv venv
source venv/bin/activate
```

#### Windows (PowerShell)

```powershell
python -m venv venv
venv\Scripts\activate
```

### 2. Install program dependencies

```bash
pip install -r requirements.txt
```

### 3. Compile the Slice definitions

Use the Slice-to-Python compiler to generate Python code from the `Greeter.ice` file:

```bash
slice2py ../slice/Greeter.ice
```

### 4. Run the server

```bash
python main.py --Ice.Trace.Locator
```

## Running the client

In a separate terminal, navigate to the `client` directory.

### 1. Create and activate a Python virtual environment

#### macOS and Linux

```bash
python3 -m venv venv
source venv/bin/activate
```

#### Windows (PowerShell)

```powershell
python -m venv venv
venv\Scripts\activate
```

### 2. Install program dependencies

```bash
pip install -r requirements.txt
```

### 3. Compile the Slice definitions

Use the Slice-to-Python compiler to generate Python code from the `Greeter.ice` file:

```bash
slice2py ../slice/Greeter.ice
```

### 4. Run the client

```bash
python main.py --Ice.Trace.Locator
```

> [!NOTE]  
> The `--Ice.Trace.Locator` command-line option is optional: it enables tracing (logging) for locator resolution and  
> helps you understand the locator logic implemented by the IceDiscovery plug-in.

[Python Virtual Environments]: https://docs.python.org/3/tutorial/venv.html
