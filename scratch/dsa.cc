#include <iostream>
#include <openssl/dsa.h>
#include <openssl/err.h>
#include <openssl/sha.h>

void handleErrors()//openSSlのエラー処理
{
  ERR_print_errors_fp(stderr);
  abort();
}

int main()
{
  // メッセージ
  std::string message = "Hello, OpenSSL DSA!";

  // DSA鍵の生成
  DSA* dsa = DSA_new();//DSAキーオブジェクト　下記で生成したDSA鍵ペア、DSAパラメータを格納

  if (dsa == nullptr)
  {
    std::cerr << "Failed to create DSA key" << std::endl;
    handleErrors();
  }

  if (DSA_generate_parameters_ex(dsa, 2048/*DSAパラメータのビット長*/, nullptr, 0, nullptr, nullptr, nullptr) != 1)//DSAパラメータ(p,q,g)生成
  {
    std::cerr << "Failed to generate DSA parameters" << std::endl;
    handleErrors();
  }

  if (DSA_generate_key(dsa) != 1)//DSA鍵ペア生成(x,y)
  {
    std::cerr << "Failed to generate DSA key pair" << std::endl;
    handleErrors();
  }

  std::cout << dsa << std::endl;

  // メッセージのハッシュ計算
  unsigned char digest[SHA256_DIGEST_LENGTH];//SHA256_DIGEST_LENGTHはSHA-256ハッシュのバイト長を表す定数
  SHA256(reinterpret_cast<const unsigned char*>(message.c_str()), message.length(), digest);//与えられたデータ（メッセージ）のハッシュ値を計算

  // DSA署名生成
  unsigned char signature[DSA_size(dsa)];
  unsigned int signatureLength;
  if (DSA_sign(0, digest, SHA256_DIGEST_LENGTH, signature, &signatureLength, dsa) != 1)
  {
    std::cerr << "Failed to generate DSA signature" << std::endl;
    handleErrors();
  }

  // DSA署名検証
  if (DSA_verify(0, digest, SHA256_DIGEST_LENGTH, signature, signatureLength, dsa) != 1)//検証成功で１を返す。
  {
    std::cerr << "DSA signature verification failed" << std::endl;
    handleErrors();
  }

  std::cout << "DSA signature verification succeeded" << std::endl;

  // DSA鍵の解放
  DSA_free(dsa);

  return 0;
}
