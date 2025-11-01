# Password Generator

A small C++ command-line utility that generates secure random passwords, ensures each generated password contains at least one uppercase letter, one lowercase letter, one digit and one special character, and saves generated passwords (with timestamps) to a JSON-formatted file.

## Features

- Generates passwords with a user-specified length (minimum 8).
- Ensures inclusion of at least one character from each category:
  - Uppercase letters
  - Lowercase letters
  - Digits
  - Special characters
- Uses a Mersenne Twister RNG to shuffle the generated password for extra randomness.
- Appends each generated password and its creation timestamp to `passwords.json` in a simple JSON object format.

> Note: This project provides a small example implementation intended for learning and demonstration. Do not use this tool to store production secrets — it appends JSON objects to a file without encryption and with a simple custom format. If you want to secure your passwords, consider using established password managers and secure storage solutions or password hasher like Argon2id.

## Repository structure

- `main.cpp` — main program and password generation logic.
- `json.h` — tiny helper class for keeping password entries and producing a simple JSON-like string.
- `CMakeLists.txt` — CMake build configuration.

## Requirements

- C++ compiler with C++11 or later support (project uses <random> and shuffle).
- CMake >= 3.10
- A standard development toolchain (gcc/clang on Linux/macOS or MSVC on Windows).

## Build instructions

1. Clone the Repository
  ''' bash
   git clone https://github.com/ramzanbhutto/Password-Generator.git
   cd Password-Generator
   '''

2. Create and switch to a build directory, then run CMake and build:
   ```
   mkdir build
   cd build
   cmake ..
   cmake --build .
   ```

3. The executable will be created as `./password_generator` in the build directory.

## Usage

Run the program from the build directory:
```bash
./password_generator
```

Example interaction:
```plaintext
Enter the desired password length (minimum 8): 12
Password before shuffle: Ab3#kL9@1sT2
Generated Password: 1k#A2b9sT@L3
Password saved to passwords.json
```

Each run appends a JSON object to `passwords.json`. The object uses the timestamp as the key and the password as the value. Example appended output (one object per line):
```bash
{
  "2025-11-01 21:15:00": "1k#A2b9sT@L3"
}
```
Note: The current implementation appends a new JSON object on each run — the file will contain multiple JSON objects (one per line), not a single aggregated JSON array.

## Customization ideas

- Store entries in a proper JSON array and use a JSON library (e.g., nlohmann/json) to manage reading/writing.
- Add command-line flags to:
  - Specify output file path
  - Choose whether to append or overwrite
  - Exclude/require character categories
  - Generate multiple passwords in one run
- Add optional encryption for saved passwords.
- Improve timestamp format or timezone handling.

## Security considerations

- The program seeds srand(time(0)) for initial character picks but uses std::random_device + mt19937 for shuffling. Consider replacing any use of rand() with C++ <random> facilities for consistent cryptographic-quality randomness if this tool will be used for real secrets.
- Do not store real, sensitive passwords in plaintext on disk. Use secure storage/encryption for production use.

## Contributing

Contributions, bug reports and improvements are welcome. Please open an issue or submit a pull request with a description of the change.

## License

This project does not include an explicit license file. If you would like to add a license, add a `LICENSE` file (for example, MIT or Apache 2.0) to the repository.
