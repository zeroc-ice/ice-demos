# Ice Inheritance

The Inheritance demo shows how to write a simple filesystem application using interface inheritance.

We recommend running each program in a separate Python virtual environment.
If you are new to Python virtual environments, see [Python Virtual Environments].

## Running the server

Navigate to the `server` directory.

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

Use the Slice-to-Python compiler to generate Python code from the `Filesystem.ice` file:

```bash
slice2py ../slice/Filesystem.ice
```

### 4. Run the server

```bash
python main.py
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

Use the Slice-to-Python compiler to generate Python code from the `Filesystem.ice` file:

```bash
slice2py ../slice/Filesystem.ice
```

### 4. Run the client

```bash
python main.py
```

[Python Virtual Environments]: https://docs.python.org/3/tutorial/venv.html
