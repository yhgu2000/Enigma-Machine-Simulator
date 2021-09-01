#include "Rotor.hpp"
#include <QDebug>

const rotor_t Rotor::kDefault_t = []() {
  rotor_t rot;
  for (uint8_t i = 0; i < kRotorMod; ++i)
    rot[i] = i;
  return rot;
}();

const Rotor& Rotor::kDefault = reinterpret_cast<const Rotor&>(kDefault_t);

QString
Rotor::hash()
{
  char hash[5] = { 0, 0, 0, 0, 0 };
  for (uint8_t i = 0; i < kRotorMod; ++i) {
    hash[i % 5] ^= (i ^ (*this)[i]) << (i % 3);
  }
  for (uint8_t i = 0; i < 5; ++i) {
    hash[i] = 'A' + hash[i] % 26;
  }
  return QString::fromLatin1(hash, 5);
}

void
Rotor::reverse_to(Rotor& r)
{
  for (uint8_t i = 0; i < kRotorMod; ++i)
    r[(*this)[i]] = i;
}

bool
Rotor::encode(QString& s) const
{
  s.resize(kRotorMod << 1);
  for (uint8_t i = 0; i < kRotorMod; ++i) {
    s[i << 1] = 'A' + ((*this)[i] >> 4);
    s[i << 1 | 1] = 'A' + ((*this)[i] & 0x0f);
  }
  return true;
}

bool
Rotor::decode(const QString& s)
{
  if (s.size() != kRotorMod << 1)
    return false;

  rotor_t tmp;
  for (uint8_t i = 0; i < kRotorMod; ++i)
    tmp[i] = 0xff;

  for (uint8_t i = 0; i < kRotorMod; ++i) {
    auto high = uint8_t(s[i << 1].toLatin1() - 'A');
    if (high & 0xf0)
      return false;

    auto low = uint8_t(s[i << 1 | 1].toLatin1() - 'A');
    if (low & 0xf0)
      return false;

    tmp[i] = uint8_t(high << 4 | low);
  }

  reinterpret_cast<rotor_t&>(*this) = tmp;
  return true;
}
