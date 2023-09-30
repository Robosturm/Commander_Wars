
#include <openssl/evp.h>
#include <openssl/rsa.h>
#include <openssl/pem.h>
#include <openssl/x509.h>

#include <QSslKey>

#include "network/NetworkInterface.h"

#include "coreengine/gameconsole.h"

void NetworkInterface::attachKeys(QSslConfiguration &sslConfiguration)
{
    std::unique_ptr<EVP_PKEY_CTX, void (*)(EVP_PKEY_CTX *)> ctx{EVP_PKEY_CTX_new_id(EVP_PKEY_RSA, nullptr), EVP_PKEY_CTX_free};
    if (ctx.get() != nullptr)
    {
        EVP_PKEY * pkey = EVP_RSA_gen(2048);
        if (pkey != nullptr)
        {
            // write key info to bio
            std::unique_ptr<BIO, int (*)(BIO *)> privateKeyBio{BIO_new(BIO_s_mem()), BIO_free};
            std::unique_ptr<BIO, int (*)(BIO *)> publicKeyBio{BIO_new(BIO_s_mem()), BIO_free};
            PEM_write_bio_PrivateKey(privateKeyBio.get(), pkey, nullptr, nullptr, 0, nullptr, nullptr);            
            std::unique_ptr<X509, void (*)(X509 *)>x509{X509_new(), X509_free};
            ASN1_INTEGER_set(X509_get_serialNumber(x509.get()), 1);
            X509_gmtime_adj(X509_get_notBefore(x509.get()), 0);
            X509_gmtime_adj(X509_get_notAfter(x509.get()), 3600L * 24L * 365L * 10L);
            X509_set_pubkey(x509.get(), pkey);
            auto * nameField = X509_get_subject_name(x509.get());
            X509_NAME_add_entry_by_txt(nameField, "C",  MBSTRING_ASC, reinterpret_cast<const unsigned char *>("DE"),        -1, -1, 0);
            X509_NAME_add_entry_by_txt(nameField, "O",  MBSTRING_ASC, reinterpret_cast<const unsigned char *>("CommanderWars"), -1, -1, 0);
            // X509_NAME_add_entry_by_txt(nameField, "CN", MBSTRING_ASC, reinterpret_cast<const unsigned char *>("localhost"), -1, -1, 0);
            X509_set_issuer_name(x509.get(), nameField);
            if (X509_sign(x509.get(), pkey, EVP_sha1()))
            {
                PEM_write_bio_X509(publicKeyBio.get(), x509.get());
                // reserve buffer for bio data
                auto privateLength = BIO_pending(privateKeyBio.get());
                auto publicLength = BIO_pending(publicKeyBio.get());
                std::vector<char> privateKeyVector(privateLength + 1);
                std::vector<char> publicKeyVector(publicLength + 1);
                // read bio data into vectors
                BIO_read(privateKeyBio.get(), &privateKeyVector[0], privateLength);
                BIO_read(publicKeyBio.get(), &publicKeyVector[0], publicLength);
                // convert bytes into qbytearray
                QByteArray keyBytes = QByteArray(&privateKeyVector[0]);
                QSslKey key(keyBytes, QSsl::KeyAlgorithm::Rsa, QSsl::EncodingFormat::Pem);
                if (!key.isNull() && key.length() > 0)
                {
                    sslConfiguration.setPrivateKey(key);
                    // convert bytes into qbytearray
                    keyBytes = QByteArray(&publicKeyVector[0]);
                    QSslCertificate cert(keyBytes);
                    if (!cert.isNull())
                    {
                        sslConfiguration.setLocalCertificate(cert);
                    }
                    else
                    {
                        CONSOLE_PRINT("Failed to generate certificate", GameConsole::eLogLevels::eERROR);
                    }
                }
                else
                {
                    CONSOLE_PRINT("Failed to generate private qsslkey is null=" + QString::number(key.isNull()) + " length is " + QString::number(key.length()), GameConsole::eLogLevels::eERROR);
                }
            }
            else
            {
                CONSOLE_PRINT("Failed to self sign certificate", GameConsole::eLogLevels::eERROR);
            }
        }
        else
        {
            CONSOLE_PRINT("Failed to generate rsa key", GameConsole::eLogLevels::eERROR);
        }
    }
    else
    {
        CONSOLE_PRINT("Failed to generate openssl context", GameConsole::eLogLevels::eERROR);
    }
}
