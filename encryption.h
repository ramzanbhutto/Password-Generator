// encryption.h
#ifndef ENCRYPTION_H
#define ENCRYPTION_H
#include<string>
#include<vector>
#include<stdexcept>
#include<algorithm>
#include<openssl/evp.h> // OpenSSL EVP library for high-level cryptographic functions
#include<openssl/rand.h> // OpenSSL RAND library for random number generation

class Encryption{
private:
  static const int KEY_SIZE = 32;  // 256 bits for AES-256
  static const int IV_SIZE = 16;   // 128 bits for AES block size
  static const int SALT_SIZE = 16;
  static const int ITERATIONS = 10000;  // PBKDF2 iterations for key derivation

  // Derives encryption key from master password using PBKDF2
  static std::vector<unsigned char> deriveKey(const std::string& password, const std::vector<unsigned char>& salt){
    std::vector<unsigned char> key(KEY_SIZE);
    PKCS5_PBKDF2_HMAC(password.c_str(), password.length(), salt.data(), salt.size(), ITERATIONS, EVP_sha256(), KEY_SIZE, key.data());
    return key;
  }

public:
  struct EncryptedData{
    std::vector<unsigned char> salt;
    std::vector<unsigned char> iv;
    std::vector<unsigned char> ciphertext;
  };

  // Encrypts plaintext using AES-256-CBC
  static EncryptedData encrypt(const std::string& plaintext, const std::string& password){
    EncryptedData result;
    result.salt.resize(SALT_SIZE);
    result.iv.resize(IV_SIZE);
    // Generate random salt and IV for this encryption
    if(RAND_bytes(result.salt.data(), SALT_SIZE)!=1 || RAND_bytes(result.iv.data(), IV_SIZE)!=1){
      throw std::runtime_error("Failed to generate random bytes");
    }
    auto key = deriveKey(password, result.salt);
    EVP_CIPHER_CTX* ctx = EVP_CIPHER_CTX_new();
    if(!ctx) throw std::runtime_error("Failed to create cipher context");
    try{
      if(EVP_EncryptInit_ex(ctx, EVP_aes_256_cbc(), nullptr, key.data(), result.iv.data())!=1){
        throw std::runtime_error("Encryption initialization failed");
      }
      result.ciphertext.resize(plaintext.length() + EVP_CIPHER_block_size(EVP_aes_256_cbc()));
      int len=0, ciphertext_len=0;
      if(EVP_EncryptUpdate(ctx, result.ciphertext.data(), &len, reinterpret_cast<const unsigned char*>(plaintext.c_str()), plaintext.length())!=1){
        throw std::runtime_error("Encryption failed");
      }
      ciphertext_len = len;
      if(EVP_EncryptFinal_ex(ctx, result.ciphertext.data()+len, &len)!=1){
        throw std::runtime_error("Encryption finalization failed");
      }
      ciphertext_len += len;
      result.ciphertext.resize(ciphertext_len);
      EVP_CIPHER_CTX_free(ctx);
      std::fill(key.begin(), key.end(), 0);  // Zero out key from memory for security
    }catch(...){
      EVP_CIPHER_CTX_free(ctx);
      std::fill(key.begin(), key.end(), 0);
      throw;
    }
    return result;
  }

  // Decrypts ciphertext using AES-256-CBC
  static std::string decrypt(const EncryptedData& data, const std::string& password){
    auto key = deriveKey(password, data.salt);
    EVP_CIPHER_CTX* ctx = EVP_CIPHER_CTX_new();
    if(!ctx) throw std::runtime_error("Failed to create cipher context");
    try{
      if(EVP_DecryptInit_ex(ctx, EVP_aes_256_cbc(), nullptr, key.data(), data.iv.data())!=1){
        throw std::runtime_error("Decryption initialization failed");
      }
      std::vector<unsigned char> plaintext(data.ciphertext.size() + EVP_CIPHER_block_size(EVP_aes_256_cbc()));
      int len=0, plaintext_len=0;
      if(EVP_DecryptUpdate(ctx, plaintext.data(), &len, data.ciphertext.data(), data.ciphertext.size())!=1){
        throw std::runtime_error("Decryption failed");
      }
      plaintext_len = len;
      if(EVP_DecryptFinal_ex(ctx, plaintext.data()+len, &len)!=1){
        throw std::runtime_error("Wrong password or corrupted data");
      }
      plaintext_len += len;
      EVP_CIPHER_CTX_free(ctx);
      std::fill(key.begin(), key.end(), 0);  // Zero out key from memory
      return std::string(reinterpret_cast<char*>(plaintext.data()), plaintext_len);
    }catch(...){
      EVP_CIPHER_CTX_free(ctx);
      std::fill(key.begin(), key.end(), 0);
      throw;
    }
  }
};
#endif
