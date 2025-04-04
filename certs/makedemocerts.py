#!/usr/bin/env python
#
# Copyright (c) ZeroC, Inc. All rights reserved.
#

import os, sys, socket, getopt

try:
    import IceCertUtils
except Exception as ex:
    print("couldn't load IceCertUtils, did you install the `zeroc-icecertutils'\n"
          "package from the Python package repository?\nerror: " + str(ex))
    sys.exit(1)

def usage():
    print("Usage: " + sys.argv[0] + " [options]")
    print("")
    print("Options:")
    print("-h               Show this message.")
    print("-d | --debug     Debugging output.")
    print("--ip <ip>        The IP address for the server certificate.")
    print("--dns <dns>      The DNS name for the server certificate.")
    print("--use-dns        Use the DNS name for the server certificate common")
    print("                 name (default is to use the IP address)." )
    sys.exit(1)

#
# Check arguments
#
debug = False
ip = None
dns = None
usedns = False
impl = ""
try:
    opts, args = getopt.getopt(sys.argv[1:], "hd", ["help", "debug", "ip=", "dns=","use-dns","impl="])
except getopt.GetoptError as e:
    print("Error %s " % e)
    usage()
    sys.exit(1)

for (o, a) in opts:
    if o == "-h" or o == "--help":
        usage()
        sys.exit(0)
    elif o == "-d" or o == "--debug":
        debug = True
    elif o == "--ip":
        ip = a
    elif o == "--dns":
        dns = a
    elif o == "--use-dns":
        usedns = True
    elif o == "--impl":
        impl = a

def request(question, newvalue, value):
    while True:
        sys.stdout.write(question)
        sys.stdout.flush()
        input = sys.stdin.readline().strip()
        if input == 'n':
            sys.stdout.write(newvalue)
            sys.stdout.flush()
            return sys.stdin.readline().strip()
        else:
            return value

#
# Change to the directory where the certs files are stored
#
os.chdir(os.path.dirname(os.path.abspath(__file__)))

if not ip:
    try:
        ip = socket.gethostbyname(socket.gethostname())
    except:
        ip = "127.0.0.1"
    ip = request("The IP address used for the server certificate will be: " + ip + "\n"
                 "Do you want to keep this IP address? (y/n) [y]", "IP : ", ip)

if not dns:
    dns = "localhost"
    dns = request("The DNS name used for the server certificate will be: " + dns + "\n"
                  "Do you want to keep this DNS name? (y/n) [y]", "DNS : ", dns)

CertificateFactory = vars(IceCertUtils)[impl + "CertificateFactory"]
factory = CertificateFactory(debug=debug, cn="Ice Demos CA")

# The password for the private keys, etc.
password = "password"

#
# CA certificate
#
factory.getCA().save("cacert.pem", password=password).save("cacert.der", password=password)

# Client certificate
client = factory.create("client")
client.save("client.p12", password=password)

# Server certificate
server = factory.create("server", cn = (dns if usedns else ip), ip=ip, dns=dns, extendedKeyUsage="serverAuth,clientAuth")
server.save("server.p12", password=password)

try:
    factory.getCA().save("cacert.pem", password=password).save("cacert.jks", password=password) # Used by the Database/library demo
    server.save("server.jks", caalias="cacert", password=password)
    client.save("client.jks", caalias="cacert", password=password)

    # Don't try to generate the BKS if the JKS generation fails
    try:
        server.save("server.bks", caalias="cacert", password=password)
        client.save("client.bks", caalias="cacert", password=password)
    except Exception as ex:
        for f in ["server.bks", "client.bks"]:
            if os.path.exists(f): os.remove(f)
        print("warning: couldn't generate BKS certificates for Android applications:\n" + str(ex))
        print("Please fix this issue if you want to run the Android demos.")

except Exception as ex:
    for f in ["server.jks", "client.jks"]:
        if os.path.exists(f): os.remove(f)
    print("warning: couldn't generate JKS certificates for Java applications:\n" + str(ex))
    print("Please fix this issue if you want to run the Java demos.")

factory.destroy()
