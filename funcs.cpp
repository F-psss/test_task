#include "funcs.h"

QBitArray bytesToBits(const QByteArray &bytes)
{
    QBitArray bits;
    bits.resize(bytes.size()*8);
    // Convert from QByteArray to QBitArray
    for(int i=0; i<bytes.count(); ++i)
        for(int b=0; b<8; ++b)
            bits.setBit(i*8+b, bytes.at(i)&(1<<(7-b)));
    return bits;
}

QByteArray bitsToBytes(const QBitArray &bits)
{
    QByteArray bytes;
    bytes.resize(bits.size()/8);

    // Convert from QBitArray to QByteArray
    for(int b=0; b<bits.count(); ++b)
        bytes[b/8] = (bytes.at(b/8) | ((bits[b]?1:0)<<(b%8)));
    return bytes;
}
