// ННГУ, ВМК, Курс "Методы программирования-2", С++, ООП
//
// tset.cpp - Copyright (c) Гергель В.П. 04.10.2001
//   Переработано для Microsoft Visual Studio 2008 Сысоевым А.В. (19.04.2015)
//
// Множество - реализация через битовые поля

#include "tset.h"

// Fake variables used as placeholders in tests
static const int FAKE_INT = -1;
static TBitField FAKE_BITFIELD(1);
static TSet FAKE_SET(1);

TSet::TSet(int mp) : BitField(mp)
{
    MaxPower = mp;
    BitField = TBitField(mp);
}

// конструктор копирования
TSet::TSet(const TSet& s) : BitField(s.MaxPower)
{
    MaxPower = s.MaxPower;
    BitField = TBitField(s.BitField);
}

// конструктор преобразования типа
TSet::TSet(const TBitField& bf) : BitField(bf.GetLength())
{
    MaxPower = bf.GetLength();
    BitField = TBitField(bf);
}

TSet::operator TBitField()
{
    return { BitField };
}

int TSet::GetMaxPower(void) const // получить макс. к-во эл-тов
{
    return MaxPower;
}

int TSet::IsMember(const int Elem) const // элемент множества?
{
    return BitField.GetBit(Elem);
}

void TSet::InsElem(const int Elem) // включение элемента множества
{
    BitField.SetBit(Elem);
}

void TSet::DelElem(const int Elem) // исключение элемента множества
{
    BitField.ClrBit(Elem);
}

// теоретико-множественные операции

TSet& TSet::operator=(const TSet& s) // присваивание
{
    BitField = s.BitField;
    MaxPower = s.MaxPower;
    return *this;
}

int TSet::operator==(const TSet& s) const // сравнение
{
    return BitField == s.BitField;
}

int TSet::operator!=(const TSet& s) const // сравнение
{
    return BitField != s.BitField;
}

TSet TSet::operator+(const TSet& s) // объединение
{
    TSet t(*this);
    t.BitField = t.BitField | s.BitField;
    return t;
}

TSet TSet::operator+(const int Elem) // объединение с элементом
{
    TSet t(*this);
    t.BitField.SetBit(Elem);
    return t;
}

TSet TSet::operator-(const int Elem) // разность с элементом
{
    TSet t(*this);
    t.BitField.ClrBit(Elem);
    return t;
}

TSet TSet::operator*(const TSet& s) // пересечение
{
    TSet t(*this);
    t.BitField = t.BitField & s.BitField;
    return t;
}

TSet TSet::operator~(void) // дополнение
{
    TSet t(*this);
    return ~t;
}

// перегрузка ввода/вывода

istream& operator>>(istream& istr, TSet& s) // ввод
{
    for (int i = 0; i < s.BitField.GetLength(); i++)
    {
        int t;
        istr >> t;
        if (t > 0)
            s.BitField.SetBit(i);
    }
    return istr;
}

ostream& operator<<(ostream& ostr, const TSet& s) // вывод
{
    for (int i = 0; i < s.BitField.GetLength(); i++)
    {
        ostr << s.BitField.GetBit(i);
    }
    return ostr;
}
