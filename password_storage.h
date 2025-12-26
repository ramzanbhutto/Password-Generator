// password_storage.h
#ifndef PASSWORD_STORAGE_H
#define PASSWORD_STORAGE_H
#include<string>
#include<vector>
#include<fstream>
#include<sstream>
#include<ctime>
#include<algorithm>
#include "encryption.h"
#include "base64.h"
#include "password_entry.h"

class PasswordStorage{
private:
  std::string filename;
  std::string masterPassword;
  std::vector<PasswordEntry> entries;

  // Gets current timestamp in readable format
  std::string getCurrentTimestamp(){
    time_t now = time(0);
    tm *ltm = localtime(&now);
    char buf[80];
    strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", ltm);
    return std::string(buf);
  }

  // Escapes special characters for JSON format
  std::string escapeJson(const std::string& str){
    std::string escaped;
    for(char c : str){
      switch(c){
        case '"': escaped += "\\\""; break;
        case '\\': escaped += "\\\\"; break;
        case '\n': escaped += "\\n"; break;
        case '\r': escaped += "\\r"; break;
        case '\t': escaped += "\\t"; break;
        default: escaped += c;
      }
    }
    return escaped;
  }

  // Unescapes JSON special characters
  std::string unescapeJson(const std::string& str){
    std::string unescaped;
    for(size_t i=0; i<str.length(); i++){
      if(str[i]=='\\' && i+1<str.length()){
        switch(str[i+1]){
          case '"': unescaped += '"'; i++; break;
          case '\\': unescaped += '\\'; i++; break;
          case 'n': unescaped += '\n'; i++; break;
          case 'r': unescaped += '\r'; i++; break;
          case 't': unescaped += '\t'; i++; break;
          default: unescaped += str[i];
        }
      }else{
        unescaped += str[i];
      }
    }
    return unescaped;
  }

public:
  PasswordStorage(const std::string& file, const std::string& master) : filename(file), masterPassword(master){}

  // Adds a new encrypted password entry
  void addEntry(const std::string& service, const std::string& username, const std::string& password, const std::string& notes=""){
    auto encrypted = Encryption::encrypt(password, masterPassword);
    PasswordEntry entry;
    entry.service = service;
    entry.username = username;
    entry.encryptedPassword = Base64::encode(encrypted.ciphertext);
    entry.salt = Base64::encode(encrypted.salt);
    entry.iv = Base64::encode(encrypted.iv);
    entry.timestamp = getCurrentTimestamp();
    entry.notes = notes;
    entries.push_back(entry);
  }

  // Retrieves and decrypts a password
  std::string getPassword(const std::string& service, const std::string& username=""){
    for(const auto& entry : entries){
      if(entry.service==service && (username.empty() || entry.username==username)){
        Encryption::EncryptedData data;
        data.ciphertext = Base64::decode(entry.encryptedPassword);
        data.salt = Base64::decode(entry.salt);
        data.iv = Base64::decode(entry.iv);
        return Encryption::decrypt(data, masterPassword);
      }
    }
    throw std::runtime_error("No matching entry found");
  }

  // Returns all stored entries without decrypting passwords
  std::vector<PasswordEntry> listEntries() const{
    return entries;
  }

  // Deletes an entry matching service and optional username
  bool deleteEntry(const std::string& service, const std::string& username=""){
    auto it = std::remove_if(entries.begin(), entries.end(), [&](const PasswordEntry& e){
      return e.service==service && (username.empty() || e.username==username);
    });
    if(it!=entries.end()){
      entries.erase(it, entries.end());
      return true;
    }
    return false;
  }

  // Saves all entries to JSON file
  void save(){
    std::ofstream file(filename);
    if(!file.is_open()){
      throw std::runtime_error("Can't open file for writing");
    }
    file<<"[\n";
    for(size_t i=0; i<entries.size(); i++){
      const auto& e = entries[i];
      file<<"  {\n";
      file<<"    \"service\": \""<<escapeJson(e.service)<<"\",\n";
      file<<"    \"username\": \""<<escapeJson(e.username)<<"\",\n";
      file<<"    \"password\": \""<<escapeJson(e.encryptedPassword)<<"\",\n";
      file<<"    \"salt\": \""<<escapeJson(e.salt)<<"\",\n";
      file<<"    \"iv\": \""<<escapeJson(e.iv)<<"\",\n";
      file<<"    \"timestamp\": \""<<escapeJson(e.timestamp)<<"\",\n";
      file<<"    \"notes\": \""<<escapeJson(e.notes)<<"\"\n";
      file<<"  }"<<(i<entries.size()-1 ? "," : "")<<"\n";
    }
    file<<"]\n";
    file.close();
  }

  // Loads entries from JSON file
  void load(){
    std::ifstream file(filename);
    if(!file.is_open()){
      return;  // File doesn't exist yet, that's okay
    }
    entries.clear();
    std::string line, content;
    while(std::getline(file, line)){
      content += line;
    }
    file.close();
    // Simple JSON parser - extracts objects between { }
    size_t pos = 0;
    while((pos=content.find("{", pos))!=std::string::npos){
      size_t end = content.find("}", pos);
      if(end==std::string::npos) break;
      PasswordEntry entry;
      std::string block = content.substr(pos, end-pos+1);
      // Lambda to extract value for a given key
      auto extract = [&](const std::string& key) -> std::string{
        std::string search = "\""+key+"\":";
        size_t start = block.find(search);
        if(start==std::string::npos) return "";
        start = block.find("\"", start+search.length());
        if(start==std::string::npos) return "";
        size_t finish = block.find("\"", start+1);
        if(finish==std::string::npos) return "";
        return unescapeJson(block.substr(start+1, finish-start-1));
      };
      entry.service = extract("service");
      entry.username = extract("username");
      entry.encryptedPassword = extract("password");
      entry.salt = extract("salt");
      entry.iv = extract("iv");
      entry.timestamp = extract("timestamp");
      entry.notes = extract("notes");
      entries.push_back(entry);
      pos = end+1;
    }
  }

  int getEntryCount() const{
    return entries.size();
  }
};
#endif
