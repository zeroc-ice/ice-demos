# Ice SSL Test Certificates

The example applications in this repository use the SSL certificates found in this directory.
These certificates are intended for testing purposes and **must not** be used in a production environment.

The certificates are copied from the main Ice repository.

These certificates include `localhost` and `127.0.0.1` as Subject Alternative Names (DNS and IP),
which match the default endpoint configuration used by the demos.  

If you run the SSL demos with a different host or network configuration, you must regenerate the certificates to match
your setup.

Refer to the [certs/README.md] file in the main Ice repository for instructions on how to recreate the certificates.

[certs/README.md]: https://github.com/zeroc-ice/ice/tree/main/certs