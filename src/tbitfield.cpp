// ННГУ, ВМК, Курс "Методы программирования-2", С++, ООП
//
// tbitfield.cpp - Copyright (c) Гергель В.П. 07.05.2001
//   Переработано для Microsoft Visual Studio 2008 Сысоевым А.В. (19.04.2015)
//
// Битовое поле

#include "tbitfield.h"

// Fake variables used as placeholders in tests
static const int FAKE_INT = -1;
static TBitField FAKE_BITFIELD(1);

TBitField::TBitField(int len)
{
    if (len <= 0)
        throw("ERROR");
    BitLen = len;
    if (BitLen % (8 * sizeof(TELEM)) == 0) {
        MemLen = BitLen / (8 * sizeof(TELEM));
    }
    else {
        MemLen = BitLen / (8 * sizeof(TELEM))+1;
    }
    pMem = new TELEM[MemLen];
    for (int i = 0; i < MemLen; i++)
    {
        pMem[i] = 0;
    }
}

TBitField::TBitField(const TBitField &bf) // конструктор копирования
{
    BitLen = bf.BitLen;
    MemLen = bf.MemLen;
    pMem = new TELEM[MemLen];
    for (int i = 0; i < MemLen; i++)
    {
        pMem[i] = bf.pMem[i];
    }
}

TBitField::~TBitField()
{
    delete[] pMem;
}

int TBitField::GetMemIndex(const int n) const // индекс Мем для бита n
{
    return n / BlockSize;
}

TELEM TBitField::GetMemMask(const int n) const // битовая маска для бита n
{
    return 1 << (n % BlockSize);
}

// доступ к битам битового поля

int TBitField::GetLength(void) const // получить длину (к-во битов)
{
  return BitLen;
}

void TBitField::SetBit(const int n) // установить бит
{
    if ((n < 0)||(n>BitLen))
        throw("ERROR");
    int ind = GetMemIndex(n);
    pMem[ind] = pMem[ind] | GetMemMask(n);
}

void TBitField::ClrBit(const int n) // очистить бит
{
    if ((n < 0) || (n > BitLen))
        throw("ERROR");
    int ind = GetMemIndex(n);
    pMem[ind] = pMem[ind] & (~GetMemMask(n));
}

int TBitField::GetBit(const int n) const // получить значение бита
{
    if ((n < 0) || (n > BitLen))
        throw("ERROR");
    int ind = GetMemIndex(n);
    //cout << GetMemMask(n) << endl;
    return (pMem[ind] & GetMemMask(n))>0;
}

// битовые операции

TBitField& TBitField::operator=(const TBitField &bf) // присваивание
{
    if (*this != bf) {
        delete[] pMem;

        BitLen = bf.BitLen;
        MemLen = bf.MemLen;

        pMem = new TELEM[MemLen];
        for (int i = 0; i < MemLen; i++)
        {
            pMem[i] = bf.pMem[i];
        }
    }
    
    return *this;
}

int TBitField::operator==(const TBitField &bf) const // сравнение
{
    if(MemLen==bf.MemLen){
        for (int i = 0; i < MemLen; ++i) {
            if(pMem[i]!=bf.pMem[i]){
                return false;
            }
        }
        return true;
    }
  return false;
}

int TBitField::operator!=(const TBitField &bf) const // сравнение
{
    if(MemLen==bf.MemLen){
        for (int i = 0; i < MemLen; ++i) {
            if(pMem[i]!=bf.pMem[i]){
                return true;
            }
        }
        return false;
    }
    return true;
}

TBitField TBitField::operator|(const TBitField &bf) // операция "или"
{
    if (BitLen >= bf.BitLen) {
        TBitField t(*this);
        for (int i = 0; i < bf.MemLen; i++)
        {
            t.pMem[i] = t.pMem[i] | bf.pMem[i];
        }
        return t;
    } else{
        TBitField t(bf);
        for (int i = 0; i < MemLen; i++)
        {
            t.pMem[i] = t.pMem[i] | pMem[i];
        }
        return t;
    }
}

TBitField TBitField::operator&(const TBitField &bf) // операция "и"
{
    if (BitLen >= bf.BitLen) {
        TBitField t(*this);
        for (int i = 0; i < bf.MemLen; i++)
        {
            t.pMem[i] = t.pMem[i] & bf.pMem[i];
        }
        return t;
    } else{
        TBitField t(bf);
        for (int i = 0; i < MemLen; i++)
        {
            t.pMem[i] = t.pMem[i] & pMem[i];
        }
        return t;
    }
}

TBitField TBitField::operator~(void) // отрицание
{
    TBitField t(*this);
//    for (int i = 0; i < MemLen; i++)
//    {
//        t.pMem[i] = ~t.pMem[i];
//        t.pMem[i] = (t.pMem[i] << (BlockSize-BitLen))>>(BlockSize-BitLen); // Грязный хак, стёр единицы в начале
//    }
    if(MemLen!=1){
        for (int i = 0; i < MemLen-1; i++) {
            t.pMem[i] = ~t.pMem[i];
        }
    }
    int j = MemLen - 1;
    t.pMem[j] = ~t.pMem[j];
    if(BitLen%BlockSize != 0){
        t.pMem[j] = (t.pMem[j] << (BlockSize-BitLen%BlockSize))>>(BlockSize-BitLen%BlockSize); // Грязный хак, стёр единицы в начале
    }

    return t;
}

// ввод/вывод

istream &operator>>(istream &istr, TBitField &bf) // ввод
{
    cout << "Please, input one bit per string:" << endl;
    for (int i = 0; i < bf.BitLen; i++)
    {
        short a;
        istr>> a;
        if(a>0)
            bf.SetBit(i);
    }
    return istr;
}

ostream &operator<<(ostream &ostr, const TBitField &bf) // вывод
{
    for (int i = 0; i < bf.BitLen; i++)
    {
        ostr << bf.GetBit(i);
    }
    ostr << endl;
    return ostr;
}
