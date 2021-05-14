#include "BlowfishInstance.h"

//Credits: Niels Keurentjes
int char2int(char input)
{
	if (input >= '0' && input <= '9')
		return input - '0';
	if (input >= 'A' && input <= 'F')
		return input - 'A' + 10;
	if (input >= 'a' && input <= 'f')
		return input - 'a' + 10;
	throw std::invalid_argument("Invalid input string");
}

void BlowfishInstance::Initialize(BlowfishInstance::CryptoType crypto)
{
	try
	{
		switch (crypto)
		{
		case CryptoType::PUBLIC:
		{
			std::call_once(_initialFlag, [this] {
				_publicKey.clear();
				for (size_t i = 0; i < PUBLIC_KEY.length(); i += 2) {
					auto hex = PUBLIC_KEY.substr(i, 2);
					_publicKey.push_back((char2int(hex[0]) << 4) + char2int(hex[1]));
				}
				_publicBlowfish.reset(new Blowfish(_publicKey));
				publicBfInitialized = true;
				});
		} break;
		case CryptoType::AUTHENTICATED:
		{
			std::call_once(_flag, [this] {
				_authenticationKey = GenerateRandomKey();
				_authenticatedBlowfish.reset(new Blowfish(_authenticationKey));
				});
		} break;
		default: break;
		}
	}
	catch (const std::system_error& e)
	{
		printf(e.what() + '\n');
	}
}

BlowfishInstance::Key BlowfishInstance::GetKeyPart()
{
	char min_7 = _authenticationKey[0] - 7;
	return { min_7,
		_authenticationKey[1],
		_authenticationKey[2],
		_authenticationKey[3] };
}

void BlowfishInstance::Decrypt(std::vector<char>& buffer, BlowfishInstance::CryptoType crypto)
{
	switch (crypto)
	{
	case CryptoType::PUBLIC: if (_publicBlowfish) buffer = _publicBlowfish->Decrypt(buffer); break;
	case CryptoType::AUTHENTICATED: if (_authenticatedBlowfish) buffer = _authenticatedBlowfish->Decrypt(buffer); break;
	}
}

void BlowfishInstance::Encrypt(std::vector<char>& buffer, BlowfishInstance::CryptoType crypto)
{
	switch (crypto)
	{
	case CryptoType::PUBLIC: if (_publicBlowfish) buffer = _publicBlowfish->Encrypt(buffer); break;
	case CryptoType::AUTHENTICATED: if (_authenticatedBlowfish) buffer = _authenticatedBlowfish->Encrypt(buffer); break;
	}
}

//Credits: Bill Lynch
BlowfishInstance::Key BlowfishInstance::GenerateRandomKey()
{
	Key ret;
	std::random_device rd;
	std::mt19937 mt(rd());
	std::uniform_int_distribution<uint32_t> dist(10, 230);
	for (size_t i{ 0 }; i < 4; ++i)
	{
		ret.push_back(dist(mt));
	}
	ret[0] += 7;
	ret.push_back(0x29);
	ret.push_back(0xA1);
	ret.push_back(0xD3);
	ret.push_back(0x56);
	return ret;
}