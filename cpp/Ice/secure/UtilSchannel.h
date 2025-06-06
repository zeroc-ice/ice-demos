// Copyright (c) ZeroC, Inc.

#include <Ice/Ice.h>
#include <stdexcept>
#include <string>

namespace Util
{
    /// Creates an in-memory certificate store from the given certificate file.
    /// The caller is responsible for closing the store using `CertCloseStore`.
    /// @param path The path to the certificate file in DER format.
    /// @return A handle to the certificate store containing the loaded certificate.
    /// @throws std::runtime_error if the store cannot be created or the certificate cannot be loaded.
    HCERTSTORE loadCertificateStore(const std::string& path);

    /// Loads a certificate context (certificate and associated private key) from a PKCS#12 file.
    /// This is typically used to load a server certificate for use with Schannel.
    /// The caller is responsible for freeing the returned context using `CertFreeCertificateContext`.
    /// @param path The path to the PKCS#12 file.
    /// @param password The password for the PKCS#12 file.
    /// @return A certificate context containing the certificate and its private key.
    /// @throws std::runtime_error if the certificate cannot be imported or does not contain a private key.
    PCCERT_CONTEXT
    certificateContextFromPKCS12File(const std::string& path, const std::wstring& password);
}
