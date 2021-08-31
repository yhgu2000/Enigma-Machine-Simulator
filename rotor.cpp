#include "Rotor.hpp"

const rotor_t Rotor::kDefault = []() {
  rotor_t rot;
  for (uint8_t i = 0; i < kRotorMod; ++i)
    rot[i] = i;
  return rot;
}();

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
