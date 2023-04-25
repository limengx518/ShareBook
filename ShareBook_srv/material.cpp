#include "material.h"
#include "encodephoto.h"
#include "test.h"

Material::Material(const std::string &tid, std::string jottingId):
    MaterialInterface(tid),_jottingProxy(jottingId)
{

}

Material::Material(const std::string id, std::string jottingId, std::string path, int type):
    MaterialInterface(id),_jottingProxy(jottingId),m_path(path)
{
    switch(type){
    case 1:
        m_type = PICTURE;
        break;
    case 2:
        m_type = AUDIO;
        break;
    case 3:
        m_type = VIDEO;
        break;
    default:
        std::cerr<<"Material type error"<<std::endl;
        break;
    }
}

const std::string Material::jottingId() const
{
    return _jottingProxy.id();
}

nlohmann::json Material::getAbstract()
{
    nlohmann::json j;
    if(m_type==PICTURE){
//        std::string picId = m_path.substr(16,m_path.size());
//        picId = picId.substr(0,picId.size()-4);
//        j["picId"] = picId;
        j["picId"] = m_path;
        if(strcmp(TEST_TYPE,"PATH")){
            j["picContent"] = encodePhoto(m_path);
        }else{
            j["picContent"] = m_path;
        }
    }
    return j;
//    return encodePhoto(m_path);
}

const std::string &Material::path() const
{
    return m_path;
}

const std::string Material::type() const
{
    return "1";
}
