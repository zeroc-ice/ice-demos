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

SecKeychainRef openKeychain(const string& path)
{
    SecKeychainRef keychain;
    OSStatus err = SecKeychainOpen(path.c_str(), &keychain);
    if (err)
    {
        throw runtime_error("SSL transport: unable to open keychain: '" + path + "'");
    }

    SecKeychainStatus status;
    err = SecKeychainGetStatus(keychain, &status);
    if (err == noErr)
    {
        err = SecKeychainUnlock(keychain, 0, nullptr, FALSE);
        if (err)
        {
            CFRelease(keychain);
            throw runtime_error("SSL transport: unable to unlock keychain.");
        }
    }
    else if (err == errSecNoSuchKeychain)
    {
        err = SecKeychainCreate(path.c_str(), 0, nullptr, false, nullptr, &keychain)
        if (err)
        {
            throw runtime_error("SSL transport: unable to create keychain.");
        }
    }
    else
    {
        throw runtime_error("SSL transport: unable to open keychain");
    }

    // Set keychain settings to avoid keychain lock.
    SecKeychainSettings settings;
    settings.version = SEC_KEYCHAIN_SETTINGS_VERS1;
    settings.lockOnSleep = FALSE;
    settings.useLockInterval = FALSE;
    settings.lockInterval = INT_MAX;
    if ((err = SecKeychainSetSettings(&keychain, &settings)))
    {
        CFRelease(keychain);
        throw runtime_error("SSL transport: error setting keychain settings");
    }

    return keychain;
}

// Load keychain items (Certificates or Private Keys) from a file. On return items param contain
// the list of items, the caller must release it.
CFArrayRef
loadKeychainItems(const string& file, SecKeychainRef keychain, const string& password)
{
    CFMutableDataRef data = readCertFile(file);

    SecItemImportExportKeyParameters params;
    memset(&params, 0, sizeof(params));
    params.version = SEC_KEY_IMPORT_EXPORT_PARAMS_VERSION;
    params.flags |= kSecKeyNoAccessControl;

    if (!password.empty())
    {
        params.passphrase = CFStringCreateWithCString(nullptr, password.c_str(), kCFStringEncodingUTF8);
    }

    UniqueRef<CFArrayRef> items;
    SecExternalItemType importType = type;
    SecExternalFormat format = kSecFormatPKCS12;
    UniqueRef<CFStringRef> path(toCFString(file));
    OSStatus err = SecItemImport(data.get(), nullptr, &format, &importType, 0, &params, keychain, &items.get());

    if (err != noErr)
    {
        ostringstream os;
        os << "SSL transport: error reading " << (type == kSecItemTypePrivateKey ? "private key" : "certificate");
        os << " '" << file << "':\n" << sslErrorToString(err);
        throw CertificateReadException(__FILE__, __LINE__, os.str());
    }

    if (type != kSecItemTypeUnknown && importType != kSecItemTypeAggregate && importType != type)
    {
        ostringstream os;
        os << "SSL transport: error reading " << (type == kSecItemTypePrivateKey ? "private key" : "certificate");
        os << " '" << file << "' doesn't contain the expected item";
        throw CertificateReadException(__FILE__, __LINE__, os.str());
    }

    return items.release();
}