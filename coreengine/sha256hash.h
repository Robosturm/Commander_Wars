#pragma once

#include <QIODevice>

class Sha256Hash final
{
    static constexpr quint32 MAX_DATA_LENGTH = 64;

public:
    explicit Sha256Hash();
    ~Sha256Hash() = default;

    void addData(QIODevice* device);
    void addData(const QByteArray & data);
    QByteArray result();
private:
    void transform(const quint8 data[]);
private:
    quint8 m_data[MAX_DATA_LENGTH];
    quint32 m_datalen;
    quint64 m_bitlen;
    quint32 m_state[8];
};

