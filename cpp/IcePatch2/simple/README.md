This demo illustrates how to write a simple IcePatch2 client
application.

In order to use this demo, you must first prepare a sample data
directory for the IcePatch2 server. The contents of the data
directory are what the client will receive during the patch process.
For convenience, this example uses its own source files as the data
to be patched.

Copy the files you want IcePatch2 to serve into the serverData directory and
then prepare the data files using the following command:
```
$ icepatch2calc serverData
```
Next, start the IcePatch2 server.
```
$ icepatch2server --Ice.Config=config.server
```
Before we start the patch client, we must first create an empty
directory where the downloaded files will be placed. For example, open
a new command window and execute this command:
```
$ mkdir C:\icepatch_download
```
Now you can start the IcePatch2 client. It will use the clientData directory
as its data directory.
```
$ client
```
If you add files or delete files from the server data directory or make
changes to existing files, you must stop the server, run icepatch2calc
again to update the data files, and then restart the IcePatch2 server.
