/*#include <iostream>
#include <string>
#include <openssl/sha.h>

std::string sha256(const std::string& message) {
    unsigned char digest[SHA256_DIGEST_LENGTH];
    SHA256_CTX sha256Context;

    SHA256_Init(&sha256Context);
    SHA256_Update(&sha256Context, message.c_str(), message.length());
    SHA256_Final(digest, &sha256Context);

    std::string hash;
    for (int i = 0; i < SHA256_DIGEST_LENGTH; i++) {
        hash += static_cast<char>(digest[i]);
    }

    return hash;
}

int main() {
    std::string message = "Hello, World!";
    std::string hash = sha256(message);

    std::cout << "Message: " << message << std::endl;
    std::cout << "SHA256 Hash: " << hash << std::endl;

    return 0;
}*/
#include "ns3/core-module.h"
#include <openssl/sha.h>

using namespace ns3;

int main()
{
  // メッセージの定義
  std::string message = "Hello, ns-3 with OpenSSL!";

  // SHA256ハッシュの計算
  unsigned char hash[SHA256_DIGEST_LENGTH];
  SHA256_CTX sha256;
  SHA256_Init(&sha256);
  SHA256_Update(&sha256, message.c_str(), message.length());
  SHA256_Final(hash, &sha256);

  // ハッシュ値の表示
  std::cout << "Message: " << message << std::endl;
  std::cout << "SHA256 Hash: ";
  for (int i = 0; i < SHA256_DIGEST_LENGTH; ++i) {
    printf("%02x", hash[i]);
  }
  std::cout << std::endl;

  return 0;
}




