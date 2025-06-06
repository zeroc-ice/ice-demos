// Copyright (c) ZeroC, Inc.

#include "UtilSecureTransport.h"

#include <Security/SecureTransport.h>
#include <Security/Security.h>

#include <fstream>
#include <iostream>

using namespace std;

CFArrayRef
Util::loadCertificateFromFile(const string& file)
{
    // Open the certificate file in binary mode.
    ifstream is(file.c_str(), ios::in | ios::binary);
    if (!is.good())
    {
        throw runtime_error("error opening file '" + file + "'");
    }

    // Determine the size of the file.
    is.seekg(0, is.end);
    size_t size = static_cast<size_t>(is.tellg());
    is.seekg(0, is.beg);

    // Create a mutable CFData buffer to hold the file contents.
    CFMutableDataRef data = CFDataCreateMutable(kCFAllocatorDefault, static_cast<CFIndex>(size));
    CFDataSetLength(data, static_cast<CFIndex>(size));

    // Read the file contents into the CFData buffer.
    is.read(reinterpret_cast<char*>(CFDataGetMutableBytePtr(data)), static_cast<streamsize>(size));
    if (!is.good())
    {
        CFRelease(data);
        throw runtime_error("error reading file '" + file + "'");
    }

    // Create a SecCertificateRef from the DER-encoded data.
    SecCertificateRef cert = SecCertificateCreateWithData(kCFAllocatorDefault, data);
    CFRelease(data);
    if (!cert)
    {
        throw runtime_error("error creating certificate from file '" + file + "'");
    }

    // Wrap the certificate in a CFArrayRef as expected by Secure Transport APIs like SSLSetCertificate.
    CFArrayRef certArray = CFArrayCreate(
        kCFAllocatorDefault,
        const_cast<const void**>(reinterpret_cast<void**>(&cert)),
        1,
        &kCFTypeArrayCallBacks);

    // Release the original certificate reference now that it's retained by the array.
    CFRelease(cert);

    // Return the array containing the single certificate. The caller is responsible for releasing it using CFRelease.
    return certArray;
}

CFArrayRef
Util::loadSecIdentityWithMatchingSubject(const std::string& subject)
{
    // Convert the subject string to a CFStringRef.
    CFStringRef subjectCstr = CFStringCreateWithCString(kCFAllocatorDefault, subject.c_str(), kCFStringEncodingUTF8);

    // Build a query dictionary to search for an identity (certificate + private key) in the keychain.
    CFMutableDictionaryRef query =
        CFDictionaryCreateMutable(nullptr, 0, &kCFTypeDictionaryKeyCallBacks, &kCFTypeDictionaryValueCallBacks);

    // Specify that we are searching for a SecIdentityRef.
    CFDictionarySetValue(query, kSecClass, kSecClassIdentity);

    // Match identities whose subject contains the given substring.
    CFDictionarySetValue(query, kSecMatchSubjectContains, subjectCstr);

    // Limit the result to one match.
    CFDictionarySetValue(query, kSecMatchLimit, kSecMatchLimitOne);

    // Request the actual reference (SecIdentityRef) as the result.
    CFDictionarySetValue(query, kSecReturnRef, kCFBooleanTrue);

    CFTypeRef identity = nullptr;
    OSStatus err = SecItemCopyMatching(query, &identity);

    CFRelease(query);
    CFRelease(subjectCstr);

    if (err != errSecSuccess || identity == nullptr)
    {
        throw std::runtime_error(
            "error: cannot load certificate identity matching '" + subject + "': " + std::to_string(err));
    }

    // Wrap the identity in a CFArrayRef, as required by Secure Transport (e.g., SSLSetCertificate).
    // Additional certificates (e.g., intermediate CAs) can be added to this array if needed.
    CFMutableArrayRef chain = CFArrayCreateMutable(kCFAllocatorDefault, 1, &kCFTypeArrayCallBacks);
    CFArrayAppendValue(chain, identity);

    // Release the identity reference, since the array retains it.
    CFRelease(identity);

    // Return the array containing the certificate chain. The caller is responsible for releasing it using CFRelease.
    return chain;
}
