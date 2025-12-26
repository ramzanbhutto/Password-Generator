// password_entry.h
#ifndef PASSWORD_ENTRY_H
#define PASSWORD_ENTRY_H
#include<string>

// Structure to hold password information
struct PasswordEntry{
  std::string service;            // Service name (e.g., Gmail, GitHub)
  std::string username;           // Username or email
  std::string encryptedPassword;  // Base64-encoded encrypted password
  std::string salt;               // Base64-encoded salt used in encryption
  std::string iv;                 // Base64-encoded initialization vector
  std::string timestamp;          // Creation timestamp
  std::string notes;              // Optional notes
};
#endif
