# Ice Greeter

The Greeter demo illustrates how to send a request and wait for the response.

This demo provides two implementations for the server: a synchronous dispatch implementation (in `server`), and an
asynchronous dispatch implementation (in `server_asyncio`). The same client works with both.

We recommend running each program in a separate Python virtual environment. If you are new to Python virtual environments,
see [Python Virtual Environments].

## Running the Server

Navigate to the `server` or `server_asyncio` directory, depending on whether you want to run the synchronous or
asynchronous version of the server.

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
python main.py
```

## Running the Client

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
python main.py
```

[Python Virtual Environments]: https://docs.python.org/3/tutorial/venv.html
