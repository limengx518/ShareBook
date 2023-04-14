#include "relationalbroker.h"
#include "netizen.h"
#include <iostream>
#include "jotting.h"
#include "material.h"
#include <string>
#include "snowflakeidworker.h"

std::unique_ptr<sql::Connection> RelationalBroker::m_connection=NULL;

RelationalBroker::RelationalBroker()
{
    // Instantiate Driver
    sql::Driver* driver = sql::mariadb::get_driver_instance();

    // Configure Connection
    sql::SQLString url("jdbc:mariadb://localhost:3306/ShareBook");
    sql::Properties properties({{"user", "root"}, {"password", "root"}});

    // Establish Connection
    std::unique_ptr<sql::Connection> conn(driver->connect(url, properties));
    m_connection=std::move(conn);

}

void RelationalBroker::initDataBase()
{
    try {
        auto &idWorker = Singleton<IdWorker>::instance();
        //创造表
        std::unique_ptr<sql::PreparedStatement> stmnt(m_connection->prepareStatement("create table if not exists Netizen(N_id INT(15) UNSIGNED NOT NULL,N_nickName varchar(20) not null,N_avater varchar(100) not null,N_signal varchar(200) not null,primary key(N_id))"));
        stmnt->executeQuery();

        std::unique_ptr<sql::PreparedStatement> stmnt2(m_connection->prepareStatement("create table if not exists Jotting (J_id INT(15) UNSIGNED NOT NULL ,J_content varchar(2000) not null,J_time DATETIME not null,N_id INT(15) UNSIGNED NOT NULL,primary key(J_id),foreign key(N_id) references Netizen(N_id) on delete cascade on update cascade)"));
        stmnt2->executeQuery();


        std::unique_ptr<sql::PreparedStatement> stmnt3(m_connection->prepareStatement("create table if not exists Material (M_id INT(15) UNSIGNED NOT NULL ,M_path varchar(100) not null,M_type INT(10) UNSIGNED NOT NULL,J_id INT(15) UNSIGNED NOT NULL,primary key(M_id),foreign key(J_id) references Jotting(J_id) on delete cascade on update cascade)"));
        stmnt3->executeQuery();


        std::unique_ptr<sql::PreparedStatement> stmnt4(m_connection->prepareStatement("create table if not exists Relation (N_id INT(15) UNSIGNED NOT NULL,N_Fan_id INT(15) UNSIGNED NOT NULL,primary key (N_id,N_Fan_id),foreign key(N_id) references Netizen(N_id) on delete cascade on update cascade,foreign key(N_Fan_id) references Netizen(N_id) on delete cascade on update cascade)"));
        stmnt4->executeQuery();

        std::unique_ptr<sql::PreparedStatement> stmnt5(m_connection->prepareStatement("create table if not exists Comment (C_id INT(15) UNSIGNED NOT NULL,C_content varchar(2000) not null ,C_time DATETIME not null,N_id INT(15) UNSIGNED NOT NULL,J_id INT(15) UNSIGNED NOT NULL,primary key(C_id),foreign key(N_id) references Netizen(N_id) on delete cascade on update cascade,foreign key(J_id) references Jotting(J_id) on delete cascade on update cascade)"));
        stmnt5->executeQuery();

        std::unique_ptr<sql::PreparedStatement> stmnt7(m_connection->prepareStatement("insert into Netizen (N_id,N_nickName,N_avater,N_signal) values(?,?,?,?)"));
//        stmnt7->setUInt(1,(unsigned int)idWorker.nextId());
        stmnt7->setUInt(1,1);
        stmnt7->setString(2,"Rose");
        stmnt7->setString(3,"/root/ShareBook/Avatar/avatar1.png");
        stmnt7->setString(4,"窗前明月光");
        stmnt7->execute();

//        stmnt7->setUInt(1,(unsigned int)idWorker.nextId());
        stmnt7->setUInt(1,2);
        stmnt7->setString(2,"Monica");
        stmnt7->setString(3,"/root/ShareBook/Avatar/avatar2.png");
        stmnt7->setString(4,"疑是地上霜");
        stmnt7->execute();

//        stmnt7->setUInt(1,(unsigned int)idWorker.nextId());
        stmnt7->setUInt(1,3);
        stmnt7->setString(2,"Phoebe");
        stmnt7->setString(3,"/root/ShareBook/Avatar/avatar3.png");
        stmnt7->setString(4,"举头望明月");
        stmnt7->execute();

//        stmnt7->setUInt(1,(unsigned int)idWorker.nextId());
        stmnt7->setUInt(1,4);
        stmnt7->setString(2,"Joey");
        stmnt7->setString(3,"/root/ShareBook/Avatar/avatar4.png");
        stmnt7->setString(4,"低头思故乡");
        stmnt7->execute();

        std::unique_ptr<sql::PreparedStatement> stmnt8(m_connection->prepareStatement("insert into Jotting (J_id,J_content,J_time,N_id) values(?,?,?,?)"));

//        stmnt8->setUInt(1,(unsigned int)idWorker.nextId());
        stmnt8->setUInt(1,1);
        stmnt8->setString(2,"wind wind flower");
        stmnt8->setDateTime(3,"2021-05-22 13:00:01");
        stmnt8->setUInt(4,1);
        stmnt8->execute();

//        stmnt8->setUInt(1,(unsigned int)idWorker.nextId());
        stmnt8->setUInt(1,2);
        stmnt8->setString(2,"Turn it up now hear me");
        stmnt8->setDateTime(3,"2021-10-22 17:00:02");
        stmnt8->setUInt(4,2);
        stmnt8->execute();

//        stmnt8->setUInt(1,(unsigned int)idWorker.nextId());
        stmnt8->setUInt(1,3);
        stmnt8->setString(2,"Start at eight "
                            "See what you konw "
                            "What have you been through make a list");
        stmnt8->setDateTime(3,"2021-11-22 13:00:01");
        stmnt8->setUInt(4,1);
        stmnt8->execute();

//        stmnt8->setUInt(1,(unsigned int)idWorker.nextId());
        stmnt8->setUInt(1,4);
        stmnt8->setString(2,"Now scrap the page");
        stmnt8->setDateTime(3,"2021-12-22 17:00:02");
        stmnt8->setUInt(4,2);
        stmnt8->execute();

//         stmnt8->setUInt(1,(unsigned int)idWorker.nextId());
        stmnt8->setUInt(1,5);
        stmnt8->setString(2,"Writing ain't supposed to be easier no");
        stmnt8->setDateTime(3,"2022-05-24 16:00:01");
        stmnt8->setUInt(4,3);
        stmnt8->execute();

        std::unique_ptr<sql::PreparedStatement> stmnt6(m_connection->prepareStatement("insert into Material (M_id,M_path,M_type,J_id) values(?,?,?,?)"));

//        stmnt6->setUInt(1,(unsigned int)idWorker.nextId());
        stmnt6->setUInt(1,1);
        stmnt6->setString(2,"/root/ShareBook/Picture/picture1.png");
        stmnt6->setUInt(3,1);
        stmnt6->setUInt(4,1);
        stmnt6->execute();

//        stmnt6->setUInt(1,(unsigned int)idWorker.nextId());
        stmnt6->setUInt(1,2);
        stmnt6->setString(2,"/root/ShareBook/Picture/picture2.png");
        stmnt6->setUInt(3,1);
        stmnt6->setUInt(4,1);
        stmnt6->execute();

//        stmnt6->setUInt(1,(unsigned int)idWorker.nextId());
        stmnt6->setUInt(1,3);
        stmnt6->setString(2,"/root/ShareBook/Picture/picture3.png");
        stmnt6->setUInt(3,1);
        stmnt6->setUInt(4,1);
        stmnt6->execute();

//        stmnt6->setUInt(1,(unsigned int)idWorker.nextId());
        stmnt6->setUInt(1,4);
        stmnt6->setString(2,"/root/ShareBook/Picture/picture4.png");
        stmnt6->setUInt(3,1);
        stmnt6->setUInt(4,2);
        stmnt6->execute();

//        stmnt6->setUInt(1,(unsigned int)idWorker.nextId());
        stmnt6->setUInt(1,5);
        stmnt6->setString(2,"/root/ShareBook/Picture/picture5.png");
        stmnt6->setUInt(3,1);
        stmnt6->setUInt(4,3);
        stmnt6->execute();

//        stmnt6->setUInt(1,(unsigned int)idWorker.nextId());
        stmnt6->setUInt(1,6);
        stmnt6->setString(2,"/root/ShareBook/Picture/picture6.png");
        stmnt6->setUInt(3,1);
        stmnt6->setUInt(4,4);
        stmnt6->execute();

//        stmnt6->setUInt(1,(unsigned int)idWorker.nextId());
        stmnt6->setUInt(1,7);
        stmnt6->setString(2,"/root/ShareBook/Picture/picture7.png");
        stmnt6->setUInt(3,1);
        stmnt6->setUInt(4,5);
        stmnt6->execute();

        std::unique_ptr<sql::PreparedStatement> stmnt9(m_connection->prepareStatement("insert into Relation (N_id,N_Fan_id) values(?,?)"));

        stmnt9->setUInt(1,1);
        stmnt9->setUInt(2,3);
        stmnt9->execute();

        stmnt9->setUInt(1,2);
        stmnt9->setUInt(2,1);
        stmnt9->execute();

        stmnt9->setUInt(1,4);
        stmnt9->setUInt(2,1);
        stmnt9->execute();

        std::unique_ptr<sql::PreparedStatement> stmnt10(m_connection->prepareStatement("insert into Comment (C_id,C_content,C_time,N_id,J_id) values(?,?,?,?,?)"));

//        stmnt10->setUInt(1,(unsigned int)idWorker.nextId());
        stmnt10->setUInt(1,1);
        stmnt10->setString(2,"Good!!");
        stmnt10->setDateTime(3,"2021-05-23 11:30:02");
        stmnt10->setUInt(4,2);
        stmnt10->setUInt(5,1);
        stmnt10->execute();

//        stmnt10->setUInt(1,(unsigned int)idWorker.nextId());
        stmnt10->setUInt(1,2);
        stmnt10->setString(2,"This is a good song!");
        stmnt10->setDateTime(3,"2021-12-23 11:30:02");
        stmnt10->setUInt(4,1);
        stmnt10->setUInt(5,4);
        stmnt10->execute();

        std::unique_ptr<sql::PreparedStatement> stmnt11(m_connection->prepareStatement("create table if not exists JottingNotification (JN_id INT(15) UNSIGNED NOT NULL,JN_content varchar(2000) not null ,JN_time DATETIME not null,JN_expire boolean not null,N_id INT(15) UNSIGNED NOT NULL,J_id INT(15) UNSIGNED NOT NULL,primary key(JN_id))"));
        stmnt11->executeQuery();

        std::unique_ptr<sql::PreparedStatement> stmnt12(m_connection->prepareStatement("insert into JottingNotification (JN_id,JN_content,JN_time,JN_expire,N_id,J_id) values(?,?,?,?,?,?)"));

//        stmnt12->setUInt(1,(unsigned int)idWorker.nextId());
        stmnt12->setUInt(1,1);
        stmnt12->setString(2,"你关注的人有新发布的笔记！");
        stmnt12->setDateTime(3,"2021-11-22 13:00:01");
        stmnt12->setBoolean(4,false);
        stmnt12->setUInt(5,1);
        stmnt12->setUInt(6,3);
        stmnt12->execute();

        std::unique_ptr<sql::PreparedStatement> stmnt13(m_connection->prepareStatement("create table if not exists JottingNotificationRelation (JN_id INT(15) UNSIGNED NOT NULL,Recipient_id INT(15) UNSIGNED NOT NULL,primary key (JN_id,Recipient_id),foreign key(JN_id) references JottingNotification(JN_id) on delete cascade on update cascade,foreign key(Recipient_id) references Netizen(N_id) on delete cascade on update cascade)"));
        stmnt13->executeQuery();

        std::unique_ptr<sql::PreparedStatement> stmnt14(m_connection->prepareStatement("insert into JottingNotificationRelation (JN_id,Recipient_id) values(?,?)"));

        stmnt14->setUInt(1,1);
        stmnt14->setUInt(2,3);
        stmnt14->execute();


    }catch(sql::SQLException& e){
        std::cerr<< e.what() << std::endl;
    }


}

sql::ResultSet* RelationalBroker::query(std::string command)
{
    try {
        // Create a new Statement
        std::unique_ptr<sql::Statement> stmnt(m_connection->createStatement());
        // Execute query
        sql::ResultSet *res = stmnt->executeQuery(command);
        return res;

    }catch(sql::SQLException& e){
        std::cerr << "Error selecting tasks: " << e.what() << std::endl;
    }
    return nullptr;
}

void RelationalBroker::insert(std::string command)
{
    try {
        std::unique_ptr<sql::PreparedStatement> stmnt(m_connection->prepareStatement(command));
        stmnt->executeQuery();
    }
    catch(sql::SQLException& e){
      std::cerr << "Error inserting new task: " << e.what() << std::endl;
    }
}

void RelationalBroker::drop(std::string command)
{
    try {
        std::unique_ptr<sql::PreparedStatement> stmnt(m_connection->prepareStatement(command));
        stmnt->executeQuery();
    }
    catch(sql::SQLException& e){
      std::cerr << "Error delete task: " << e.what() << std::endl;
    }
}




