// base64.h
#ifndef BASE64_H
#define BASE64_H
#include<string>
#include<vector>

namespace Base64{
  static const std::string chars = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

  // Encodes binary data to Base64 string
  inline std::string encode(const std::vector<unsigned char>& data){
    std::string ret;
    int i=0;
    unsigned char arr3[3], arr4[4];
    for(size_t n=0; n<data.size(); n++){
      arr3[i++] = data[n];
      if(i==3){
        arr4[0] = (arr3[0] & 0xfc) >> 2;
        arr4[1] = ((arr3[0] & 0x03) << 4) + ((arr3[1] & 0xf0) >> 4);
        arr4[2] = ((arr3[1] & 0x0f) << 2) + ((arr3[2] & 0xc0) >> 6);
        arr4[3] = arr3[2] & 0x3f;
        for(int j=0; j<4; j++) ret += chars[arr4[j]];
        i = 0;
      }
    }
    // Handle remaining bytes
    if(i){
      for(int j=i; j<3; j++) arr3[j] = '\0';
      arr4[0] = (arr3[0] & 0xfc) >> 2;
      arr4[1] = ((arr3[0] & 0x03) << 4) + ((arr3[1] & 0xf0) >> 4);
      arr4[2] = ((arr3[1] & 0x0f) << 2) + ((arr3[2] & 0xc0) >> 6);
      for(int j=0; j<i+1; j++) ret += chars[arr4[j]];
      while(i++<3) ret += '=';  // Add padding
    }
    return ret;
  }

  // Decodes Base64 string to binary data
  inline std::vector<unsigned char> decode(const std::string& str){
    int i=0;
    unsigned char arr4[4], arr3[3];
    std::vector<unsigned char> ret;
    for(size_t n=0; n<str.length() && str[n]!='='; n++){
      size_t pos = chars.find(str[n]);
      if(pos==std::string::npos) continue;
      arr4[i++] = str[n];
      if(i==4){
        for(int j=0; j<4; j++) arr4[j] = chars.find(arr4[j]);
        arr3[0] = (arr4[0] << 2) + ((arr4[1] & 0x30) >> 4);
        arr3[1] = ((arr4[1] & 0xf) << 4) + ((arr4[2] & 0x3c) >> 2);
        arr3[2] = ((arr4[2] & 0x3) << 6) + arr4[3];
        for(int j=0; j<3; j++) ret.push_back(arr3[j]);
        i = 0;
      }
    }
    // Handle remaining bytes
    if(i){
      for(int j=i; j<4; j++) arr4[j] = 0;
      for(int j=0; j<4; j++) arr4[j] = chars.find(arr4[j]);
      arr3[0] = (arr4[0] << 2) + ((arr4[1] & 0x30) >> 4);
      arr3[1] = ((arr4[1] & 0xf) << 4) + ((arr4[2] & 0x3c) >> 2);
      for(int j=0; j<i-1; j++) ret.push_back(arr3[j]);
    }
    return ret;
  }
}
#endif
