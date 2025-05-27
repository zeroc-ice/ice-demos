// Copyright (c) ZeroC, Inc.

#include "UtilSecureTransport.h"
#include <fstream>
#include <Security/SecureTransport.h>

using namespace std;

// Read a file into a CFMutableDataRef the caller is responsible for releasing the returned object.
CFMutableDataRef
readCertFile(const string& file)
{
    ifstream is(file.c_str(), ios::in | ios::binary);
    if (!is.good())
    {
        throw runtime_error("error opening file '" + file + "'");
    }

    is.seekg(0, is.end);
    size_t size = static_cast<size_t>(is.tellg());
    is.seekg(0, is.beg);

    CFMutableDataRef data = CFDataCreateMutable(kCFAllocatorDefault, static_cast<CFIndex>(size));
    CFDataSetLength(data, static_cast<CFIndex>(size));
    is.read(reinterpret_cast<char*>(CFDataGetMutableBytePtr(data)), static_cast<streamsize>(size));
    if (!is.good())
    {
        CFRelease(data);
        throw runtime_error("error reading file '" + file + "'");
    }
    return data;
}

// Load a DER-encoded certificate from a file and return it as a CFArrayRef.
// The caller is responsible for releasing the returned object.
CFArrayRef loadDerCertificate(const string& certFile)
{
    CFMutableDataRef data = readCertFile(certFile);
    SecCertificateRef cert = SecCertificateCreateWithData(kCFAllocatorDefault, data);
    CFRelease(data);
    if (!cert)
    {
        throw runtime_error("error creating certificate from file '" + certFile + "'");
    }
    CFArrayRef certArray = CFArrayCreate(
        kCFAllocatorDefault,
        const_cast<const void**>(reinterpret_cast<void**>(&cert)),
        1,
        &kCFTypeArrayCallBacks);
    CFRelease(cert);
    return certArray;
}
