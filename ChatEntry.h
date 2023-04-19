#include <mariadb/conncpp.hpp>
#include <string>
using namespace std;

#ifndef USERENTRY_H
#define USERENTRY_H

class userEntry {
public:
     userEntry();
     userEntry(sql::SQLString u, sql::SQLString e, sql::SQLString p, sql::SQLString I);
     string text();
     string json();

     string username;
     string email;
     string pass;
     string ID;

private:

};

#endif /* USERENTRY_H */

