// Copyright (c) ZeroC, Inc.

#include <Ice/Ice.h>
#include <stdexcept>
#include <string>

/// Creates an in memory certificate store from the given certificate file. The caller is responsible for
/// closing the store using `CertCloseStore`.
/// @param path The path to the certificate file in DER format.
/// @return a handle to the certificate store containing the loaded certificates.
/// @throws std::runtime_error if the store cannot be created or the certificate cannot be loaded.
HCERTSTORE loadCertificateStore(const std::string& path);

PCCERT_CONTEXT
certificateContextFromPKCS12File(const std::string& path, const std::string& password);