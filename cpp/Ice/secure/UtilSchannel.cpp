// Copyright (c) ZeroC, Inc.

#include "UtilSchannel.h"
#include <fstream>
#include <stdexcept>
#include <vector>

using namespace std;

namespace
{
    vector<unsigned char> readFile(const string& path)
    {
        ifstream file(path, ios::binary);
        if (!file)
        {
            throw runtime_error("Unable to read certificate file");
        }
        return vector<unsigned char>((istreambuf_iterator<char>(file)), istreambuf_iterator<char>());
    }
}

HCERTSTORE
Util::loadCertificateStore(const string& path)
{
    // Create an empty in-memory certificate store.
    HCERTSTORE store = CertOpenStore(CERT_STORE_PROV_MEMORY, 0, 0, 0, nullptr);
    if (!store)
    {
        throw runtime_error("Unable to create in-memory certificate store");
    }

    // Read the certificate file into a buffer.
    vector<unsigned char> buffer = readFile(path);

    // Add the certificate to the store. We use X509_ASN_ENCODING because the certificate is DER-encoded.
    if (!CertAddEncodedCertificateToStore(
            store,
            X509_ASN_ENCODING,
            &buffer[0],
            static_cast<DWORD>(buffer.size()),
            CERT_STORE_ADD_NEW,
            nullptr))
    {
        // If adding the certificate fails, close the store and throw an error.
        CertCloseStore(store, 0);
        throw runtime_error("Failed to add certificate to store");
    }

    // Return the certificate store. The caller is responsible for closing it using CertCloseStore.
    return store;
}

PCCERT_CONTEXT
Util::certificateContextFromPKCS12File(const string& path, const wstring& password)
{
    // Read the PKCS#12 file into a buffer.
    vector<unsigned char> buffer = readFile(path);

    CRYPT_DATA_BLOB pfxBlob;
    pfxBlob.cbData = static_cast<DWORD>(buffer.size());
    pfxBlob.pbData = reinterpret_cast<BYTE*>(&buffer[0]);

    // Import the PKCS#12 file into a temporary certificate store.
    // We use CRYPT_USER_KEYSET to persist the private key in the user's key container.
    HCERTSTORE store = PFXImportCertStore(&pfxBlob, password.c_str(), CRYPT_USER_KEYSET);
    if (store == nullptr)
    {
        throw runtime_error("Unable to import certificate");
    }

    // Find a certificate with an associated private key.
    // This ensures we retrieve the identity certificate (not just a CA certificate).
    PCCERT_CONTEXT cert =
        CertFindCertificateInStore(store, X509_ASN_ENCODING, 0, CERT_FIND_HAS_PRIVATE_KEY, 0, nullptr);

    CertCloseStore(store, 0);

    if (!cert)
    {
        throw runtime_error("Unable to find certificate with private key");
    }

    // Return the certificate context. The caller is responsible for freeing it using CertFreeCertificateContext.
    return cert;
}
