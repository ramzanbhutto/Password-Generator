// password_generator.h
#ifndef PASSWORD_GENERATOR_H
#define PASSWORD_GENERATOR_H
#include<string>
#include<random>
#include<algorithm>
#include<stdexcept>
#include<cctype>

class PasswordGenerator{
private:
  std::random_device rd;
  std::mt19937 gen;  // Mersenne Twister random number generator
  
public:
  // Character set flags that can be combined
  enum CharacterSet{
    UPPERCASE = 1,
    LOWERCASE = 2,
    DIGITS = 4,
    SPECIAL = 8,
    ALL = UPPERCASE | LOWERCASE | DIGITS | SPECIAL
  };

  PasswordGenerator() : gen(rd()){}

  // Generates a random password with specified length and character sets
  std::string generate(int length, int charsets=ALL){
    if(length<8){
      throw std::invalid_argument("Password must be at least 8 characters long");
    }
    const std::string upper = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    const std::string lower = "abcdefghijklmnopqrstuvwxyz";
    const std::string digits = "0123456789";
    const std::string special = "!@#$%^&*()-_=+[]{}|;:,.<>?/";
    std::string charset;
    std::string required;  // Ensure at least one char from each selected set
    if(charsets & UPPERCASE){
      charset += upper;
      required += upper[std::uniform_int_distribution<>(0, upper.length()-1)(gen)];
    }
    if(charsets & LOWERCASE){
      charset += lower;
      required += lower[std::uniform_int_distribution<>(0, lower.length()-1)(gen)];
    }
    if(charsets & DIGITS){
      charset += digits;
      required += digits[std::uniform_int_distribution<>(0, digits.length()-1)(gen)];
    }
    if(charsets & SPECIAL){
      charset += special;
      required += special[std::uniform_int_distribution<>(0, special.length()-1)(gen)];
    }
    if(charset.empty()){
      throw std::invalid_argument("At least one character set must be selected");
    }
    std::string password = required;
    std::uniform_int_distribution<> dist(0, charset.length()-1);
    for(int i=required.length(); i<length; i++){
      password += charset[dist(gen)];
    }
    std::shuffle(password.begin(), password.end(), gen);  // Shuffle to randomize required characters
    return password;
  }

  // Calculates password strength on a scale of 0-100
  int calculateStrength(const std::string& password){
    int strength = 0;
    bool hasUpper=false, hasLower=false, hasDigit=false, hasSpecial=false;
    for(char c : password){
      if(std::isupper(c)) hasUpper = true;
      else if(std::islower(c)) hasLower = true;
      else if(std::isdigit(c)) hasDigit = true;
      else hasSpecial = true;
    }
    // Award points for character diversity
    if(hasUpper) strength += 20;
    if(hasLower) strength += 20;
    if(hasDigit) strength += 20;
    if(hasSpecial) strength += 20;
    // Award points for length
    if(password.length()>=12) strength += 10;
    if(password.length()>=16) strength += 10;
    return strength;
  }

  // Returns human-readable strength description
  std::string getStrengthDescription(int strength){
    if(strength>=90) return "Excellent";
    if(strength>=70) return "Strong";
    if(strength>=50) return "Good";
    if(strength>=30) return "Weak";
    return "Very Weak";
  }
};
#endif
