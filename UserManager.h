#ifndef USERMANAGER_H
#define USERMANAGER_H

#include <iostream>
#include <pqxx/pqxx>
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <openssl/evp.h>
#include <sstream>
#include <iomanip>

struct userInfo {
    std::string email = "0";
    std::string username = "0";
    std::string token = "0";
    std::string created_date = "0";
    int permission = 0;
    double balance = 0.00;
};

class UserManager {
private:
    std::string email;
    std::string username;
    std::string password;
    std::string token;
    int permission = 0;
    double balance = 0.00;
    bool if_exist = false;

    std::string GenerateSaltedSha256(const std::string &password);

    void CheckUser();

public:
    UserManager(const std::string &username);

    void CreateUser(const std::string &email, const std::string &password);
    void DeleteUser();
    void ChangeUserInfo(const std::string &type, const std::string &value);
    userInfo GetUserInfo();
    bool CheckPassword(const std::string &password);
};

#endif // USERMANAGER_H