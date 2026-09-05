#include "multiplayer/totp.h"

#include <QDateTime>
#include <QMessageAuthenticationCode>
#include <QRandomGenerator>
#include <QUrl>

#include "coreengine/gameconsole.h"

QByteArray Totp::generateSecret(qint32 byteCount)
{
    QByteArray secret;
    secret.resize(byteCount);
    QRandomGenerator::system()->generate(secret.begin(), secret.end());
    return secret;
}

QString Totp::base32Encode(const QByteArray & data)
{
    static const char ALPHABET[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ234567";
    QString encoded;
    encoded.reserve((data.size() * 8 + 4) / 5);
    quint32 buffer = 0;
    qint32 bitsLeft = 0;
    for (const char byte : data)
    {
        buffer = (buffer << 8) | static_cast<quint8>(byte);
        bitsLeft += 8;
        while (bitsLeft >= 5)
        {
            encoded.append(ALPHABET[(buffer >> (bitsLeft - 5)) & 0x1F]);
            bitsLeft -= 5;
        }
    }
    if (bitsLeft > 0)
    {
        encoded.append(ALPHABET[(buffer << (5 - bitsLeft)) & 0x1F]);
    }
    return encoded;
}

QByteArray Totp::base32Decode(const QString & encoded)
{
    QByteArray decoded;
    quint32 buffer = 0;
    qint32 bitsLeft = 0;
    for (const QChar c : encoded)
    {
        qint32 value = -1;
        const ushort u = c.toUpper().unicode();
        if (u >= 'A' && u <= 'Z')
        {
            value = u - 'A';
        }
        else if (u >= '2' && u <= '7')
        {
            value = u - '2' + 26;
        }
        else if (c == '=' || c.isSpace())
        {
            continue;
        }
        if (value < 0)
        {
            return QByteArray();
        }
        buffer = (buffer << 5) | static_cast<quint32>(value);
        bitsLeft += 5;
        if (bitsLeft >= 8)
        {
            decoded.append(static_cast<char>((buffer >> (bitsLeft - 8)) & 0xFF));
            bitsLeft -= 8;
        }
    }
    return decoded;
}

QString Totp::createCode(const QByteArray & secret, qint64 timeStep, qint32 digits)
{
    QByteArray timeBytes;
    timeBytes.resize(8);
    for (qint32 i = 7; i >= 0; --i)
    {
        timeBytes[i] = static_cast<char>(timeStep & 0xFF);
        timeStep >>= 8;
    }
    const QByteArray hmac = QMessageAuthenticationCode::hash(timeBytes, secret, QCryptographicHash::Sha1);
    const qint32 offset = static_cast<quint8>(hmac[hmac.size() - 1]) & 0x0F;
    quint32 binary = (static_cast<quint32>(static_cast<quint8>(hmac[offset])) & 0x7F) << 24;
    binary |= static_cast<quint32>(static_cast<quint8>(hmac[offset + 1])) << 16;
    binary |= static_cast<quint32>(static_cast<quint8>(hmac[offset + 2])) << 8;
    binary |= static_cast<quint32>(static_cast<quint8>(hmac[offset + 3]));
    quint32 modulus = 1;
    for (qint32 i = 0; i < digits; ++i)
    {
        modulus *= 10;
    }
    return QStringLiteral("%1").arg(binary % modulus, digits, 10, QLatin1Char('0'));
}

QString Totp::createCode(const QByteArray & secret, qint32 digits)
{
    const qint64 timeStep = QDateTime::currentDateTimeUtc().toSecsSinceEpoch() / TIME_STEP_SECONDS;
    return createCode(secret, timeStep, digits);
}

bool Totp::validateCode(const QByteArray & secret, const QString & code, qint32 window, qint32 digits)
{
    const QString trimmedCode = code.simplified().remove(QLatin1Char(' '));
    if (trimmedCode.length() != digits || secret.isEmpty())
    {
        return false;
    }
    const QByteArray codeBytes = trimmedCode.toUtf8();
    const qint64 currentStep = QDateTime::currentDateTimeUtc().toSecsSinceEpoch() / TIME_STEP_SECONDS;
    for (qint32 offset = -window; offset <= window; ++offset)
    {
        const QByteArray expected = createCode(secret, currentStep + offset, digits).toUtf8();
        // constant time comparison to avoid timing attacks
        qint32 diff = expected.length() ^ codeBytes.length();
        const qint32 length = expected.length() < codeBytes.length() ? expected.length() : codeBytes.length();
        for (qint32 i = 0; i < length; ++i)
        {
            diff |= expected[i] ^ codeBytes[i];
        }
        if (diff == 0)
        {
            return true;
        }
    }
    return false;
}

QString Totp::buildOtpAuthUrl(const QString & issuer, const QString & username, const QString & base32Secret)
{
    const QString label = issuer + QLatin1Char(':') + username;
    return QStringLiteral("otpauth://totp/") + QString::fromUtf8(QUrl::toPercentEncoding(label))
           + QStringLiteral("?secret=") + base32Secret
           + QStringLiteral("&issuer=") + QString::fromUtf8(QUrl::toPercentEncoding(issuer))
           + QStringLiteral("&algorithm=SHA1&digits=6&period=30");
}

bool Totp::selfTest()
{
    bool success = true;
    // RFC 6238 Appendix B test vectors for SHA-1 with ASCII secret "12345678901234567890"
    const QByteArray secret = QByteArrayLiteral("12345678901234567890");
    struct TestVector
    {
        qint64 timeSeconds;
        qint32 digits;
        const char * expected;
    };
    static const TestVector vectors[] =
    {
        {59,          8, "94287082"},
        {59,          6, "287082"},
        {1111111109,  8, "07081804"},
        {1234567890,  8, "89005924"},
        {2000000000,  8, "69279037"},
        {20000000000, 8, "65353130"},
    };
    for (const auto & vector : vectors)
    {
        const QString code = createCode(secret, vector.timeSeconds / TIME_STEP_SECONDS, vector.digits);
        if (code != QLatin1String(vector.expected))
        {
            CONSOLE_PRINT("Totp self test failed for time " + QString::number(vector.timeSeconds) +
                          ": expected " + vector.expected + " got " + code, GameConsole::eLogLevels::eERROR);
            CONSOLE_PRINT("2-Factor Authentication is broken and won't work correctly", GameConsole::eLogLevels::eERROR);
            success = false;
        }
    }
    // base32 round trip
    const QByteArray testData = generateSecret(DEFAULT_SECRET_BYTES);
    if (base32Decode(base32Encode(testData)) != testData)
    {
        CONSOLE_PRINT("Totp self test failed: base32 round trip mismatch", GameConsole::eLogLevels::eERROR);
        CONSOLE_PRINT("2-Factor Authentication is broken and won't work correctly", GameConsole::eLogLevels::eERROR);
        success = false;
    }
    if (success)
    {
        CONSOLE_PRINT("Totp self test passed", GameConsole::eLogLevels::eDEBUG);
    }
    return success;
}
