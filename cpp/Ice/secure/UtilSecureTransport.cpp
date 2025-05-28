// Copyright (c) ZeroC, Inc.

#include "UtilSecureTransport.h"

#include <Security/SecureTransport.h>
#include <Security/Security.h>

#include <fstream>
#include <iostream>

using namespace std;

// Load a DER-encoded certificate from a file and return it as a CFArrayRef.
// The caller is responsible for releasing the returned object.
CFArrayRef
loadCertificateFromFile(const string& file)
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

    SecCertificateRef cert = SecCertificateCreateWithData(kCFAllocatorDefault, data);
    CFRelease(data);
    if (!cert)
    {
        throw runtime_error("error creating certificate from file '" + file + "'");
    }
    CFArrayRef certArray = CFArrayCreate(
        kCFAllocatorDefault,
        const_cast<const void**>(reinterpret_cast<void**>(&cert)),
        1,
        &kCFTypeArrayCallBacks);
    CFRelease(cert);
    return certArray;
}

CFArrayRef
loadSecIdentityWithLabel(const std::string& label)
{
    // Convert label to CFString
    CFStringRef labelStr = CFStringCreateWithCString(kCFAllocatorDefault, label.c_str(), kCFStringEncodingUTF8);

    // Build query dictionary
    CFMutableDictionaryRef query =
        CFDictionaryCreateMutable(nullptr, 0, &kCFTypeDictionaryKeyCallBacks, &kCFTypeDictionaryValueCallBacks);

    CFDictionarySetValue(query, kSecClass, kSecClassIdentity);
    CFDictionarySetValue(query, kSecMatchSubjectContains, labelStr);
    CFDictionarySetValue(query, kSecMatchLimit, kSecMatchLimitOne);
    CFDictionarySetValue(query, kSecReturnRef, kCFBooleanTrue);

    CFTypeRef identity = nullptr;
    OSStatus err = SecItemCopyMatching(query, &identity);

    CFRelease(query);
    CFRelease(labelStr);

    if (err != errSecSuccess || identity == nullptr)
    {
        throw std::runtime_error(
            "error: cannot load certificate identity with label '" + label + "': " + std::to_string(err));
    }

    {
        SecCertificateRef cert = nullptr;
        SecIdentityCopyCertificate((SecIdentityRef)identity, &cert);

        CFStringRef summary = SecCertificateCopySubjectSummary(cert);
        char buffer[256];
        if (CFStringGetCString(summary, buffer, sizeof(buffer), kCFStringEncodingUTF8)) {
            std::cout << "Matched certificate: " << buffer << std::endl;
        }
        CFRelease(cert);
        CFRelease(summary);
    }

    // Build chain array with identity (more certs can be added if needed).
    CFMutableArrayRef chain = CFArrayCreateMutable(kCFAllocatorDefault, 1, &kCFTypeArrayCallBacks);
    CFArrayAppendValue(chain, identity);

    // Release the identity reference, as it is now owned by the array.
    CFRelease(identity);

    return chain;
}