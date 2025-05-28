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
loadCertificateStore(const string& path)
{
    HCERTSTORE store = CertOpenStore(CERT_STORE_PROV_MEMORY, 0, 0, 0, nullptr);
    if (!store)
    {
        throw runtime_error("Unable create in memory certificate store");
    }

    vector<unsigned char> buffer = readFile(path);
    if (buffer.empty())
    {
        CertCloseStore(store, 0);
        throw runtime_error("CA certificate file is empty");
    }

    if (!CertAddEncodedCertificateToStore(
            store,
            X509_ASN_ENCODING | PKCS_7_ASN_ENCODING,
            &buffer[0],
            static_cast<DWORD>(buffer.size()),
            CERT_STORE_ADD_NEW,
            nullptr))
    {
        CertCloseStore(store, 0);
        throw runtime_error("Error adding certificate to store");
    }
    return store;
}

PCCERT_CONTEXT
certificateContextFromPKCS12File(const string& path, const string& password)
{
    vector<unsigned char> buffer = readFile(path);
    if (buffer.empty())
    {
        throw runtime_error("The PFX file buffer is empty.");
    }

    CRYPT_DATA_BLOB pfxBlob;
    pfxBlob.cbData = static_cast<DWORD>(buffer.size());
    pfxBlob.pbData = reinterpret_cast<BYTE*>(&buffer[0]);

    wstring wPassword = Ice::stringToWstring(password);
    HCERTSTORE store = PFXImportCertStore(&pfxBlob, wPassword.c_str(), CRYPT_USER_KEYSET);
    if (store == nullptr)
    {
        throw runtime_error("Unable to import certificate");
    }

    PCCERT_CONTEXT cert =
        CertFindCertificateInStore(store, X509_ASN_ENCODING, 0, CERT_FIND_HAS_PRIVATE_KEY, 0, nullptr);
    CertCloseStore(store, 0);
    if (!cert)
    {
        throw runtime_error("Unable to import certificate");
    }
    return cert;
}