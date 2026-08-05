# HEAANBOOT

- The code is implementation for Eurocrypt2018 paper.

- For current version of HEAAN please visit https://github.com/snucrypto/HEAAN

- You can also find this version in https://github.com/snucrypto/HEAAN/releases/tag/1.1

## PRNG Control

We add new encrypt method to select a seed:
```
Ciphertext Scheme::encryptMsg(Plaintext& msg, ZZ seed)
```

### Example

```
NTL::ZZ seed = ZZ(1);
Ciphertext cipher = scheme.encryptMsg(plain, seed);
```

## Dependencies

- GMP
  - On debian: sudo apt install libgmp10 libgmp-dev
- NTL
  - On debian: sudo apt install libntl-dev

## Instalation

cd HEAAN/lib
make -j$(nproc)


## New methods

In order to inject erros in the middle of the computations we add this new
methods:
```
Ciphertext addBitFlip(Ciphertext& cipher1, Ciphertext& cipher2, uint32_t step, uint32_t coeff, uint32_t bit);
Ciphertext multBitFlip(Ciphertext& cipher1, Ciphertext& cipher2, uint32_t step, uint32_t coeff, uint32_t bit);
Ciphertext leftRotateFastBitFlip(Ciphertext& cipher, long rotSlots, uint32_t step, uint32_t coeff, uint32_t bit);
void reScaleByAndEqualBitFlip(Ciphertext& cipher, long bitsDown, uint32_t step, uint32_t coeff, uint32_t bit) {
```

# License
Copyright (c) by CryptoLab inc. This program is licensed under a Creative Commons Attribution-NonCommercial 3.0 Unported License. You should have received a copy of the license along with this work. If not, see http://creativecommons.org/licenses/by-nc/3.0/.
