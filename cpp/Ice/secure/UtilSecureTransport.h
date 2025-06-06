// Copyright (c) ZeroC, Inc.

#include <CoreFoundation/CoreFoundation.h>
#include <string>

namespace Util
{
    /// Loads a DER-encoded certificate from a file and returns it as a CFArrayRef.
    /// The caller is responsible for releasing the returned object using `CFRelease`.
    /// @param file The path to the certificate file.
    /// @return A CFArrayRef containing a single SecCertificateRef.
    /// @throws std::runtime_error if the file cannot be opened or the certificate cannot be decoded.
    CFArrayRef loadCertificateFromFile(const std::string& file);

    /// Loads a certificate and private key (SecIdentityRef) from the keychain by matching the certificate subject.
    /// Returns a CFArrayRef suitable for use with SSLSetCertificate.
    /// The caller is responsible for releasing the returned object using `CFRelease`.
    ///
    /// @param subject The substring to match against the certificate's subject.
    /// @return A CFArrayRef containing the SecIdentityRef.
    /// @throws std::runtime_error if the identity cannot be found.
    CFArrayRef loadSecIdentityWithMatchingSubject(const std::string& subject);
}
