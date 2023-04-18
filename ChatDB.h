#include <stdlib.h>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>
/* uncomment for applications that use vectors */
#include <vector>

//#include "mysql_connection.h"
//#include "mysql_driver.h"
#include <mariadb/conncpp.hpp>

//#include <cppconn/driver.h>
//#include <cppconn/exception.h>
//#include <cppconn/resultset.h>
//#include <cppconn/statement.h>
//#include <cppconn/prepared_statement.h>
#include "ChatEntry.h"

#ifndef CHATDB_H
#define CHATDB_H

#define DB_URL "jdbc:mariadb://localhost:3306/ChatApp"
#define USER "root"
#define PASS "newpassword"

using namespace std;

class userDB {
public:
    userDB();
   /* vector<contactEntry> find(string search);
    vector<contactEntry> findByFirst(string first);
    vector<contactEntry> findByLast(string last);*/
    bool confirm(string user, string pass);

    userEntry fetchEntry(string id);
    void reg(string username,string email,string pass);
   /* void editEntry(string idnum,string first,string last,string phone,string type, string email, string age);
    void deleteEntry(string idnum);*/
private:
    const string db_url=DB_URL;
    const string user=USER;
    const string pass=PASS;
    sql::Driver* driver;
    sql::Properties properties;
    std::unique_ptr<sql::Connection> conn;

};

#endif /* CHATDB_H */
