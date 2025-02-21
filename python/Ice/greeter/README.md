# Greeter

The Greeter demo illustrates how to send a request and wait for the response.

This demo provides two implementations for the server: a synchronous dispatch implementation (in `server`), and an asynchronous dispatch implementation (in `server_amd`). The same client works with both.

We recommend running each application in a separate Python virtual environment. If you are new to Python virtual environments, see [Python Virtual Environments].

## Running the server

Create and activate a Python virtual environment in the `server` or `server_amd` directory to run the synchronous or asynchronous version of the server application, respectively.

From the activated virtual environment in the `server` or `server_amd` directory:

- Install the program dependencies:

```shell
pip install -r requirements.txt
```

- Compile the Greeter.ice file with the Slice compiler for Python

```shell
slice2py --output-dir generated ../slice/Greeter.ice
```

- Run the server program

```shell
python main.py
```

## Running the client

In a separate terminal, create a Python virtual environment in the `client` directory to run the client application.

From the activated virtual environment in the `client` directory:

- Install the program dependencies:

```shell
pip install -r requirements.txt
```

- Compile the Greeter.ice file with the Slice compiler for Python

```shell
slice2py --output-dir generated ../slice/Greeter.ice
```

- Run the client program

```shell
python main.py
```

[Python Virtual Environments]: https://docs.python.org/3/tutorial/venv.html
