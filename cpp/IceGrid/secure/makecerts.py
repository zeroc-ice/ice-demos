#!/usr/bin/env python
# **********************************************************************
#
# Copyright (c) 2003-2016 ZeroC, Inc. All rights reserved.
#
# **********************************************************************

import os, sys, IceCertUtils

dn = IceCertUtils.DistinguishedName("IceGrid Secure Demo CA", "Ice", "ZeroC, Inc.", "Jupiter", "Florida", "US")

DNelements = {
    'C': "Country name",
    'ST':"State or province name",
    'L': "Locality",
    'O': "Organization name",
    'OU':"Organizational unit name",
    'CN':"Common name",
    'emailAddress': "Email address"
}

def question(message, expected = None):
   sys.stdout.write(message)
   sys.stdout.write(' ')
   sys.stdout.flush()
   choice = sys.stdin.readline().strip()
   if expected:
      return choice in expected
   else:
      return choice

while True:
    print("")
    print("The subject name for your CA will be:\n  " + str(dn))
    print("")
    if question("Do you want to keep this as the CA subject name? (y/n) [y]", ['n', 'N']):
        for k,v in DNelements.items():
            v = question(v + ": ")
            if k == 'C' and len(v) > 2:
               print("The contry code can't be longer than 2 characters")
               continue
            setattr(dn, k, v)
    else:
        break

#
# Create a certificate factory
#
factory = IceCertUtils.CertificateFactory(dn=dn)

# Save the CA certificate
factory.getCA().save(os.path.join("certs", "ca.pem"))

try:
    factory.getCA().save(os.path.join("certs", "ca.jks"))
except Exception as ex:
    print("warning: couldn't generate JKS certificate `ca.jks':\n" + str(ex))

# Create and the certificates for the different components
factory.create("Master").save(os.path.join("certs", "master.p12"))
factory.create("Slave").save(os.path.join("certs", "slave.p12"))
factory.create("Node").save(os.path.join("certs", "node.p12"))
factory.create("Glacier2").save(os.path.join("certs", "glacier2.p12"))
factory.create("Server").save(os.path.join("certs", "server.p12"))

factory.destroy()
