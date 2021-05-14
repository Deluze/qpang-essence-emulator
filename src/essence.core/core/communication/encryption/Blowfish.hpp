//
// Blowfish C++ implementation
//
// CC0 - PUBLIC DOMAIN
// This work is free of known copyright restrictions.
// http://creativecommons.org/publicdomain/zero/1.0/
#ifndef _BLOWFISH_H
#define _BLOWFISH_H

//std
#include <array>
#include <stdint.h>
#include <cstddef>
#include <memory>
#include <mutex>
#include <vector>

static const std::string PUBLIC_KEY = "66642423323E34357D5F7E2E33384C6160272B52452F252D49613D7C3958283F00";

class Blowfish {
public:
	Blowfish(const std::vector<char> &key);
	std::vector<char> Encrypt(const std::vector<char> &src) const;
	std::vector<char> Decrypt(const std::vector<char> &src) const;

private:
	void SetKey(const char *key, size_t byte_length);
	void EncryptBlock(uint32_t *left, uint32_t *right) const;
	void DecryptBlock(uint32_t *left, uint32_t *right) const;
	uint32_t Feistel(uint32_t value) const;

private:
	uint32_t pary_[18];
	uint32_t sbox_[4][256];
};

#endif //_BLOWFISH_H