#include "coreengine/sha256hash.h"

static constexpr quint32 ROTLEFT(quint32 a, quint32 b)
{
    return (((a) << (b)) | ((a) >> (32 - (b))));
}

static constexpr quint32 ROTRIGHT(quint32 a, quint32 b)
{
    return (((a) >> (b)) | ((a) << (32 - (b))));
}

static constexpr quint32 CH(quint32 x, quint32 y, quint32 z)
{
    return (((x) & (y)) ^ (~(x) & (z)));
}

static constexpr quint32 MAJ(quint32 x, quint32 y, quint32 z)
{
    return (((x) & (y)) ^ ((x) & (z)) ^ ((y) & (z)));
}

static constexpr quint32 EP0(quint32 x)
{
    return (ROTRIGHT(x, 2) ^ ROTRIGHT(x, 13) ^ ROTRIGHT(x, 22));
}
static constexpr quint32 EP1(quint32 x)
{
    return (ROTRIGHT(x, 6) ^ ROTRIGHT(x, 11) ^ ROTRIGHT(x, 25));
}
static constexpr quint32 SIG0(quint32 x)
{
    return (ROTRIGHT(x, 7) ^ ROTRIGHT(x, 18) ^ ((x) >> 3));
}
static constexpr quint32 SIG1(quint32 x)
{
    return (ROTRIGHT(x, 17) ^ ROTRIGHT(x, 19) ^ ((x) >> 10));
}

static const quint64 key[64] = {
    0x428a2f98,0x71374491,0xb5c0fbcf,0xe9b5dba5,0x3956c25b,0x59f111f1,0x923f82a4,0xab1c5ed5,
    0xd807aa98,0x12835b01,0x243185be,0x550c7dc3,0x72be5d74,0x80deb1fe,0x9bdc06a7,0xc19bf174,
    0xe49b69c1,0xefbe4786,0x0fc19dc6,0x240ca1cc,0x2de92c6f,0x4a7484aa,0x5cb0a9dc,0x76f988da,
    0x983e5152,0xa831c66d,0xb00327c8,0xbf597fc7,0xc6e00bf3,0xd5a79147,0x06ca6351,0x14292967,
    0x27b70a85,0x2e1b2138,0x4d2c6dfc,0x53380d13,0x650a7354,0x766a0abb,0x81c2c92e,0x92722c85,
    0xa2bfe8a1,0xa81a664b,0xc24b8b70,0xc76c51a3,0xd192e819,0xd6990624,0xf40e3585,0x106aa070,
    0x19a4c116,0x1e376c08,0x2748774c,0x34b0bcb5,0x391c0cb3,0x4ed8aa4a,0x5b9cca4f,0x682e6ff3,
    0x748f82ee,0x78a5636f,0x84c87814,0x8cc70208,0x90befffa,0xa4506ceb,0xbef9a3f7,0xc67178f2
};

Sha256Hash::Sha256Hash()
{
    m_datalen = 0;
    m_bitlen = 0;
    m_state[0] = 0x6a09e667;
    m_state[1] = 0xbb67ae85;
    m_state[2] = 0x3c6ef372;
    m_state[3] = 0xa54ff53a;
    m_state[4] = 0x510e527f;
    m_state[5] = 0x9b05688c;
    m_state[6] = 0x1f83d9ab;
    m_state[7] = 0x5be0cd19;
}

void Sha256Hash::transform(const quint8 data[])
{
    quint32 a, b, c, d, e, f, g, h, i, j, t1, t2, m[MAX_DATA_LENGTH];

    for (i = 0, j = 0; i < 16; ++i, j += 4)
    {
        m[i] = (data[j] << 24) | (data[j + 1] << 16) | (data[j + 2] << 8) | (data[j + 3]);
    }
    for ( ; i < MAX_DATA_LENGTH; ++i)
    {
        m[i] = SIG1(m[i - 2]) + m[i - 7] + SIG0(m[i - 15]) + m[i - 16];
    }
    a = m_state[0];
    b = m_state[1];
    c = m_state[2];
    d = m_state[3];
    e = m_state[4];
    f = m_state[5];
    g = m_state[6];
    h = m_state[7];

    for (i = 0; i < MAX_DATA_LENGTH; ++i)
    {
        t1 = h + EP1(e) + CH(e, f, g) + key[i] + m[i];
        t2 = EP0(a) + MAJ(a, b, c);
        h = g;
        g = f;
        f = e;
        e = d + t1;
        d = c;
        c = b;
        b = a;
        a = t1 + t2;
    }

    m_state[0] += a;
    m_state[1] += b;
    m_state[2] += c;
    m_state[3] += d;
    m_state[4] += e;
    m_state[5] += f;
    m_state[6] += g;
    m_state[7] += h;
}

void Sha256Hash::addData(QIODevice* device)
{
    addData(device->readAll());
}

void Sha256Hash::addData(const QByteArray & data)
{
    for (auto byte : data)
    {
        m_data[m_datalen] = byte;
        m_datalen++;
        if (m_datalen == MAX_DATA_LENGTH)
        {
            transform(m_data);
            m_bitlen += 512;
            m_datalen = 0;
        }
    }
}

QByteArray Sha256Hash::result()
{
    quint32 i= m_datalen;

    // Pad whatever data is left in the buffer.
    if (m_datalen < 56)
    {
        m_data[i++] = 0x80;
        while (i < 56)
        {
            m_data[i++] = 0x00;
        }
    }
    else
    {
        m_data[i++] = 0x80;
        while (i < 64)
        {
            m_data[i++] = 0x00;
        }
        transform(m_data);
        memset(m_data, 0, 56);
    }

    // Append to the padding the total message's length in bits and transform.
    m_bitlen += m_datalen * 8;
    m_data[63] = m_bitlen;
    m_data[62] = m_bitlen >> 8;
    m_data[61] = m_bitlen >> 16;
    m_data[60] = m_bitlen >> 24;
    m_data[59] = m_bitlen >> 32;
    m_data[58] = m_bitlen >> 40;
    m_data[57] = m_bitlen >> 48;
    m_data[56] = m_bitlen >> 56;
    transform(m_data);

    // Since this implementation uses little endian byte ordering and SHA uses big endian,
    // reverse all the bytes when copying the final state to the output hash.
    QByteArray hash(32, 0);
    for (i = 0; i < 4; ++i) {
        hash[i]      = (m_state[0] >> (24 - i * 8)) & 0x000000ff;
        hash[i + 4]  = (m_state[1] >> (24 - i * 8)) & 0x000000ff;
        hash[i + 8]  = (m_state[2] >> (24 - i * 8)) & 0x000000ff;
        hash[i + 12] = (m_state[3] >> (24 - i * 8)) & 0x000000ff;
        hash[i + 16] = (m_state[4] >> (24 - i * 8)) & 0x000000ff;
        hash[i + 20] = (m_state[5] >> (24 - i * 8)) & 0x000000ff;
        hash[i + 24] = (m_state[6] >> (24 - i * 8)) & 0x000000ff;
        hash[i + 28] = (m_state[7] >> (24 - i * 8)) & 0x000000ff;
    }
    return hash;
}
