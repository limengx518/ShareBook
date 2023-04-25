#include "materialbroker.h"
#include <unordered_map>

MaterialBroker* MaterialBroker::m_materialBroker=NULL;
std::unordered_map<std::string,Material> MaterialBroker::m_newClean={};
std::unordered_map<std::string,Material> MaterialBroker::m_oldClean={};
std::mutex MaterialBroker::m_mutex={};

MaterialBroker *MaterialBroker::getInstance()
{
    if(m_materialBroker==nullptr){
        m_materialBroker=new MaterialBroker();
    }
    return m_materialBroker;
}

Material &MaterialBroker::findById(std::string id)
{
    Material* material=inCache(id);
    if(material==nullptr){
        return retrieveMaterial(id);
    }
    return *material;
}

Material *MaterialBroker::inCache(std::string id)
{
    //判断是否在缓存中
    if(m_oldClean.count(id)){
        return &m_oldClean.at(id);
    }

    if(m_newClean.count(id)){
        return &m_newClean.at(id);
    }

    return nullptr;
}

Material &MaterialBroker::retrieveMaterial(std::string materialId)
{
    std::string command="select * from Material where M_id="+materialId;
    sql::ResultSet* res=RelationalBroker::query(command);
    std::string id,path,jId;
    int type;
    // Loop through and print results
    while (res->next()) {
        id=std::to_string(res->getUInt(1));
        path=res->getString(2);
        type=res->getUInt(3);
        jId=std::to_string(res->getUInt(4));
    }

    Material material(id,jId,path,type);

    //将从数据库中拿出的数据放在缓存中(旧的净缓存)
    m_oldClean.insert({material.id(),material});

    return m_oldClean.at(materialId);

}

void MaterialBroker::flush()
{
    newCacheFlush();
}

void MaterialBroker::addMaterial(Material material)
{
    m_newClean.insert({material.id(),material});
}

void MaterialBroker::newCacheFlush()
{
    for(auto iter = m_newClean.begin(); iter != m_newClean.end();){
        std::cout<<"将图片素材存入数据库"<<std::endl;
        //保证当进行插入时，数据是不可以被更改的？？？？
        std::lock_guard<std::mutex> lk(m_mutex);

        //将数据插入数据库
        std::string command="insert into Material (M_id,M_path,J_id,M_type) values("+iter->first+",'"+iter->second.path()+"',"+iter->second.jottingId()+","+iter->second.type()+")";
        std::cout<<"command:"<<command<<std::endl;
        RelationalBroker::insert(command);

        //从缓存中删除相关数据
        std::cout<<"从缓存中删除"<<std::endl;
        //unordered_map这种链表型容器erase后返回的迭代器为当前的位置
        m_newClean.erase(iter++);
    }
}

std::vector<std::string> MaterialBroker::findJottingNewMaterial(std::string jottingId)
{
    std::vector<std::string> materials;
    for(const auto& item:m_newClean){
        if(item.second.jottingId()==jottingId){
            materials.push_back(item.first);
        }
    }
    return materials;
}

MaterialBroker::~MaterialBroker()
{
    //程序退出后，将缓存中的新数据存入数据库
    for(auto &material:m_newClean){
        std::string command="insert into Material (M_id,M_path,J_id) values("+material.second.id()+","+material.second.path()+","+material.second.jottingId()+")";
        RelationalBroker::insert(command);
    }
}

MaterialBroker::MaterialBroker()
{
    //(id,jottingId,path
//    Material material("10","10","path10");
//    m_newClean.insert({material.id(),material});
}
