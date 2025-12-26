# PASSANOMIX - A Secure Password Manager

A command-line password manager with AES-256 encryption to keep your passwords safe.

## Features

- AES-256-CBC encryption with PBKDF2 key derivation
- Secure random password generation
- Password strength calculator
- JSON-based encrypted storage
- Master password protection

## Requirements

- C++ compiler with C++14 support
- CMake 3.10+
- OpenSSL library

## Installation

Install OpenSSL:
```bash
# Arch Linux
sudo pacman -S openssl

# Ubuntu/Debian
sudo apt-get install libssl-dev

# macOS
brew install openssl
```

Build the project:
```bash
git clone https://github.com/ramzanbhutto/Passanomix.git
cd Passanomix
mkdir build && cd build
cmake ..
make
./password_manager
```

## Usage

1. Enter your master password (minimum 8 characters)
2. Choose from the menu:
   - Generate and save new passwords
   - Retrieve saved passwords
   - View all entries
   - Delete entries

All passwords are encrypted with your master password before saving.

## File Structure

- `encryption.h` - AES-256 encryption
- `base64.h` - Base64 encoding
- `password_generator.h` - Password generation
- `password_entry.h` - Entry structure
- `password_storage.h` - Storage management
- `main.cpp` - User interface
- `passwords_secure.json` - Encrypted vault (auto-generated)

## Security Notes

- Never share your master password
- Keep backups of `passwords_secure.json`
- Passwords are encrypted with unique salt and IV
- Memory is zeroed after operations

## License

Free to use for personal and educational purposes.
