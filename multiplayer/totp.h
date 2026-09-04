#ifndef TOTP_H
#define TOTP_H

#include <QByteArray>
#include <QString>

/**
 * @brief The Totp class implements RFC 6238 time-based one-time passwords (TOTP)
 * as used by common authenticator apps (Aegis, 2FAS, FreeOTP+, Google Authenticator,
 * Microsoft Authenticator, Bitwarden, KeePassXC, ...).
 * Only QtCore facilities are used (QMessageAuthenticationCode / QCryptographicHash /
 * QRandomGenerator), so no additional dependencies are required.
 */
class Totp final
{
public:
    Totp() = delete;
    /**
     * @brief DEFAULT_SECRET_BYTES default length of a newly generated shared secret (160 bit)
     */
    static constexpr qint32 DEFAULT_SECRET_BYTES = 20;
    /**
     * @brief TIME_STEP_SECONDS default TOTP time step
     */
    static constexpr qint64 TIME_STEP_SECONDS = 30;
    /**
     * @brief DEFAULT_DIGITS default amount of digits of a generated code
     */
    static constexpr qint32 DEFAULT_DIGITS = 6;
    /**
     * @brief DEFAULT_VALIDATION_WINDOW accepted time steps before/after the current one (+-1 => 90 s acceptance window)
     */
    static constexpr qint32 DEFAULT_VALIDATION_WINDOW = 1;

    /**
     * @brief generateSecret creates a new cryptographically secure random shared secret
     * @param byteCount amount of random bytes
     * @return raw secret bytes
     */
    static QByteArray generateSecret(qint32 byteCount = DEFAULT_SECRET_BYTES);
    /**
     * @brief base32Encode RFC 4648 Base32 encoding without padding (format used by authenticator apps)
     * @param data raw bytes
     * @return uppercase base32 string without padding characters
     */
    static QString base32Encode(const QByteArray & data);
    /**
     * @brief base32Decode RFC 4648 Base32 decoding, tolerant to lowercase and whitespace, padding optional
     * @param encoded base32 string
     * @return raw bytes, empty array on invalid input
     */
    static QByteArray base32Decode(const QString & encoded);
    /**
     * @brief createCode creates the TOTP code for a given secret and time step counter (RFC 6238 / RFC 4226)
     * @param secret raw shared secret
     * @param timeStep time step counter (seconds since epoch / TIME_STEP_SECONDS)
     * @param digits amount of digits of the code
     * @return zero-padded numeric code
     */
    static QString createCode(const QByteArray & secret, qint64 timeStep, qint32 digits = DEFAULT_DIGITS);
    /**
     * @brief createCode creates the current TOTP code for a given secret
     * @param secret raw shared secret
     * @param digits amount of digits of the code
     * @return zero-padded numeric code
     */
    static QString createCode(const QByteArray & secret, qint32 digits = DEFAULT_DIGITS);
    /**
     * @brief validateCode checks if a user provided code matches the secret for the current time
     *        (or a nearby time step within the validation window). The comparison is constant time.
     * @param secret raw shared secret
     * @param code user provided code (whitespace is stripped)
     * @param window accepted time steps before/after the current one
     * @param digits amount of digits of the code
     * @return true if the code is valid
     */
    static bool validateCode(const QByteArray & secret, const QString & code,
                             qint32 window = DEFAULT_VALIDATION_WINDOW, qint32 digits = DEFAULT_DIGITS);
    /**
     * @brief buildOtpAuthUrl creates an otpauth:// url for enrollment (usable for QR codes)
     * @param issuer name of the server/service shown in the app
     * @param username account name shown in the app
     * @param base32Secret shared secret encoded with base32Encode
     * @return otpauth url, e.g. otpauth://totp/Issuer:user?secret=...&issuer=...
     */
    static QString buildOtpAuthUrl(const QString & issuer, const QString & username, const QString & base32Secret);
    /**
     * @brief selfTest verifies the implementation against the RFC 6238 Appendix B test vectors (SHA-1)
     * @return true if all test vectors pass
     */
    static bool selfTest();
};

#endif // TOTP_H
