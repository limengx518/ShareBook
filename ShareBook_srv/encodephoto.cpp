#include "encodephoto.h"
std::string encodePhoto(std::string photoPath){
    //将图片解码为二进制流
    try {
        std::cout<<"进入函数 encodePhoto:"<<photoPath<<std::endl;
        std::ifstream fin(photoPath, std::ios::binary);
        fin.seekg(0, std::ios::end);
        int iSize = fin.tellg();
        char* szBuf = new (std::nothrow) char[iSize];
        fin.seekg(0, std::ios::beg);
        fin.read(szBuf, sizeof(char) * iSize);
        fin.close();

        std::cout<<"iSize== "<<iSize<<std::endl;
        if(iSize<0){
            throw "encode error";
        }

        string data= base64_encode(szBuf,iSize);
        //二进制流
        return data;
    } catch (...) {
        std::cout<<"encode error";
    }
    return {};
}
