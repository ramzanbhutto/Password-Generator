#ifndef JSON_H
#define JSON_H
// use this to store the generated passwords along with their creation date and time 
#include<string>
#include<map>
class JSON{
public:
  JSON() {}
  ~JSON() {}

  void addEntry(const std::string& password, const std::string& timestamp){
    data[timestamp] = password;
  }

  std::string toString() const{
    std::string json = "{\n";
    
    for(auto it = data.begin(); it != data.end(); ++it){
      json += "  \"" + it->first + "\": \"" + it->second + "\"";
      if(std::next(it) != data.end())  json += ",";
      json += "\n";
    }

    json += "}";
    return json;
  }

private:
  std::map<std::string, std::string> data;

};
#endif
