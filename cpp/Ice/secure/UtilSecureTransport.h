// Copyright (c) ZeroC, Inc.

#include <CoreFoundation/CoreFoundation.h>
#include <string>

/// Loads a DER-encoded certificate from a file and returns it as a CFArrayRef.
/// The caller is responsible for releasing the returned object.
/// @param file The path to the certificate file.
/// @return A CFArrayRef containing the SecCertificateRef.
/// @throws std::runtime_error if the file cannot be opened or the certificate cannot be decoded.
CFArrayRef loadCertificateFromFile(const std::string& file);

/// Loads a certificate/key pair (SecIdentityRef) from the keychain using the certificate label.
/// Returns a CFArrayRef suitable for use with SSLSetCertificate (caller must CFRelease).
///
/// @param label The label of the certificate/key pair in the keychain.
/// @return A CFArrayRef containing the SecIdentityRef.
/// @throws std::runtime_error if the identity cannot be found.
CFArrayRef loadSecIdentityWithLabel(const std::string& label);