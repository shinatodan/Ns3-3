#include <iostream>
#include <iomanip>
#include <openssl/dsa.h>
#include <openssl/err.h>
#include <openssl/sha.h>

void handleErrors()
{
  ERR_print_errors_fp(stderr);
  abort();
}

/*uint64_t ConvertSignatureToUint64(const unsigned char* signature, unsigned int length)
{
  uint64_t result = 0;
  for (unsigned int i = 0; i < sizeof(uint64_t) && i < length; i++)
  {
    result <<= 8;
    result |= static_cast<uint64_t>(signature[length - 1 - i]);
  }
  return result;
}

void ConvertSignatureToBytes(uint64_t signature_uint64, unsigned char* signature_bytes, unsigned int length)
{
  for (unsigned int i = 0; i < length; i++)
  {
    signature_bytes[length - 1 - i] = static_cast<unsigned char>((signature_uint64 >> (i * 8)) & 0xFF);
  }
}*/

int main()
{
  std::string message = "Hello, OpenSSL DSA!";

  DSA* dsa = DSA_new();
  if (dsa == nullptr)
  {
    std::cerr << "Failed to create DSA key" << std::endl;
    handleErrors();
  }

  if (DSA_generate_parameters_ex(dsa, 2048, nullptr, 0, nullptr, nullptr, nullptr) != 1)
  {
    std::cerr << "Failed to generate DSA parameters" << std::endl;
    handleErrors();
  }

  if (DSA_generate_key(dsa) != 1)
  {
    std::cerr << "Failed to generate DSA key pair" << std::endl;
    handleErrors();
  }

  unsigned char digest[SHA512_DIGEST_LENGTH];
  SHA512(reinterpret_cast<const unsigned char*>(message.c_str()), message.length(), digest);

  unsigned char signature[DSA_size(dsa)];
  unsigned int signatureLength;
  if (DSA_sign(0, digest, SHA512_DIGEST_LENGTH, signature, &signatureLength, dsa) != 1)
  {
    std::cerr << "Failed to generate DSA signature" << std::endl;
    handleErrors();
  }

  if (DSA_verify(0, digest, SHA512_DIGEST_LENGTH, /*signature_bytes*/signature, signatureLength, dsa) == 1)
  {
    std::cerr << "DSA signature verification succeeded" << std::endl;
  }
  else if (DSA_verify(0, digest, SHA512_DIGEST_LENGTH, /*signature_bytes*/signature, signatureLength, dsa) != 1)
  {
    std::cerr << "DSA signature verification failed" << std::endl;
    handleErrors();
  }

  DSA_free(dsa);

  return 0;
}