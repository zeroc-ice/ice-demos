
#include <wincrypt.h>
#include <string>
#include <stdexcept>

/// Creates an in memory certificate store from the given certificate file. The caller is responsible for
/// closing the store using `CertCloseStore`.
/// @param path The path to the certificate file in DER format.
/// @return a handle to the certificate store containing the loaded certificates.
/// @throws std::runtime_error if the store cannot be created or the certificate cannot be loaded.
HCERTSTORE loadCertificateStore(const std::string& path);