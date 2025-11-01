#include<iostream>
#include<cstdlib>
#include<ctime>
#include<string>
#include<algorithm>
#include<random>
#include<fstream>
#include "json.h"

using namespace std;

string getCurrentTimestamp(){
  time_t now = time(0);
  tm *ltm = localtime(&now);

  char buf[80]; // 80 characters should be enough for timestamp
  strftime(buf, sizeof(buf), "%Y-%m-%d %X", ltm);  // strftime formats the time according to the format string provided 
  return string(buf);
}

string generatePassword(int length){
  const string upper = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
  const string lower = "abcdefghijklmnopqrstuvwxyz";
  const string digits = "0123456789";
  const string special = "!@#$%^&*()-_=+[]{}|;:,.<>?/";

  const string allChars = upper + lower + digits + special;
  string password;
  srand(time(0));
  
  // Ensure the password contains at least one character from each category
  password += upper[rand() % upper.length()];
  password += lower[rand() % lower.length()];
  password += digits[rand() % digits.length()];
  password += special[rand() % special.length()];
  for(int i=4; i<length; i++){
    password += allChars[rand() % allChars.length()];
  }

  // Shuffle the password to ensure randomness
  cout<<"Password before shuffle: "<<password<<endl;
 // random_shuffle(password.begin(), password.end()); // Deprecated in C++14 and removed in C++17
  random_device rd;  // Obtain a random number from hardware
  mt19937 g(rd());   // Seed the generator -> Mersenne Twister ; where g is the generator, mt19937 is the type of generator and rd() is the seed 
  shuffle(password.begin(), password.end(), g);

  return password;
}

int main(){
  int length;
  cout<<"Enter the desired password length (minimum 8): ";
  cin>>length;

  if(length>=8){
    string password = generatePassword(length);
    cout<<"Generated Password: "<<password<<endl;

    // Create JSON object and add entry
    JSON json;
    string timestamp = getCurrentTimestamp();
    json.addEntry(password, timestamp);

    ofstream file("passwords.json", ios::app);
    if(file.is_open()){
      file<<json.toString()<<endl;
      file.close();
      cout<<"Password saved to passwords.json"<<endl;
    }
    else cout<<"Unable to open file to save password."<<endl;
  }

  else cout<<"Password length should be at least 8 characters."<<endl;

  return 0;
}
