#pragma once

#include <memory>
#include <random>

#include "core/communication/encryption/Blowfish.hpp"

class BlowfishInstance
{
public:
	using Key = std::vector<char>;
	enum class CryptoType
	{
		PUBLIC,
		AUTHENTICATED
	};

	void Initialize(CryptoType crypto);
	Key GetKeyPart();
	void Decrypt(std::vector<char>& buffer, CryptoType crypto = CryptoType::AUTHENTICATED);
	void Encrypt(std::vector<char>& buffer, CryptoType crypto = CryptoType::AUTHENTICATED);
	bool publicBfInitialized = false;
private:
	Key GenerateRandomKey();
	std::unique_ptr<Blowfish> _publicBlowfish;
	std::unique_ptr<Blowfish> _authenticatedBlowfish;
	Key _publicKey;
	Key _authenticationKey;
	std::once_flag _initialFlag;
	std::once_flag _flag;

};