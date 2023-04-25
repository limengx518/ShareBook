#ifndef BASE64_H
#define BASE64_H

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <algorithm>
using namespace std;

#define IMG_JPG "data:image/jpeg;base64,"	//jpg图片信息，其他类似

static const std::string base64_chars =
    "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
    "abcdefghijklmnopqrstuvwxyz"
    "0123456789+/";

static bool is_base64(const char c);
//编码
std::string base64_encode(const char * bytes_to_encode, unsigned int in_len);

//解码
std::string base64_decode(std::string const & encoded_string);
#endif // BASE64_H
