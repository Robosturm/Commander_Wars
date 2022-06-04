#pragma once

#include <QByteArray>

class RsaKeyGenerator
{
public:
    RsaKeyGenerator() = delete;

    static QByteArray generatePrivateKey(bool & success);

private:
    static void seedRng();

private:
    static bool m_seeded;
};

