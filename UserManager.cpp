#include "UserManager.h"

std::string UserManager::GenerateSaltedSha256(const std::string &password) {
    const std::string salt1 = "abde3";
    const std::string salt2 = "uwyz2";
    std::string salted_input = salt1 + password + salt2;
    unsigned char hash[EVP_MAX_MD_SIZE];
    unsigned int hash_length;

    EVP_MD_CTX *context = EVP_MD_CTX_new();
    const EVP_MD *md = EVP_sha256();

    EVP_DigestInit_ex(context, md, NULL);
    EVP_DigestUpdate(context, salted_input.c_str(), salted_input.length());
    EVP_DigestFinal_ex(context, hash, &hash_length);
    EVP_MD_CTX_free(context);

    std::stringstream ss;
    for (unsigned int i = 0; i < hash_length; ++i) {
        ss << std::hex << std::setw(2) << std::setfill('0') << (int) hash[i];
    }
    return ss.str();
}

UserManager::UserManager(const std::string &connection_) : _connect(connection_) {}

void UserManager::CheckUser() {
    try {
        if (!_connect.is_open()) {
            std::cerr << "Can't open database" << std::endl;
            return;
        }

        pqxx::work W(_connect);
        pqxx::result R = W.exec_params("SELECT * FROM user_info WHERE username = $1", this->username);

        if (R.size() != 0) {
            if_exist = true;
        }
    } catch (const std::exception &e) {
        std::cerr << "CheckUser " << username << " Error: " << e.what() << std::endl;
    }
}

void UserManager::CreateUser(const std::string &username, const std::string &email, const std::string &password) {
    try {
        this->username = username;
        this->email = email;
        this->password = GenerateSaltedSha256(password);
        this->token = boost::uuids::to_string(boost::uuids::random_generator()());

        CheckUser();

        if (if_exist) {
            std::cerr << "User " << this->username << " already exists" << std::endl;
            return;
        }

        pqxx::work crt(_connect);
        crt.exec_params(
                "INSERT INTO user_info (email, username, password, token) VALUES ($1, $2, $3, $4)",
                this->email, this->username, this->password, this->token
        );
        crt.commit();
    } catch (const std::exception &e) {
        std::cerr << "Create User " << username << " Error: " << e.what() << std::endl;
    }
}

void UserManager::DeleteUser(const std::string &username) {
    try {
        this->username = username;
        if (!_connect.is_open()) {
            std::cerr << "Can't open database" << std::endl;
            return;
        }

        CheckUser();

        if (!if_exist) {
            std::cerr << "User " << this->username << " does not exist" << std::endl;
            return;
        }

        pqxx::work crt(_connect);
        crt.exec_params(
                "DELETE FROM user_info WHERE username = $1", this->username
        );
        crt.commit();
    } catch (const std::exception &e) {
        std::cerr << "Delete User " << username << " Error: " << e.what() << std::endl;
    }
}

userInfo UserManager::GetUserInfo(const std::string &username) {
    userInfo user_info;
    try {
        this->username = username;
        if (!_connect.is_open()) {
            std::cerr << "Can't open database" << std::endl;
            return user_info;
        }

        CheckUser();

        if (!if_exist) {
            std::cerr << "User " << this->username << " does not exist" << std::endl;
            return user_info;
        }

        pqxx::work W(_connect);
        pqxx::result R = W.exec_params("SELECT * FROM user_info WHERE username = $1", username);

        const pqxx::row &row = R[0];
        user_info.email = row["email"].as<std::string>();
        user_info.username = row["username"].as<std::string>();
        user_info.token = row["token"].as<std::string>();
        user_info.created_date = row["created_date"].as<std::string>();
        user_info.permission = row["permission"].as<int>();
        user_info.balance = row["balance"].as<double>();

    } catch (const std::exception &e) {
        std::cerr << "Get User " << username << "'s Info Error: " << e.what() << std::endl;
    }

    return user_info;
}

void UserManager::ChangeUserInfo(const std::string &username, const std::string &type, const std::string &value) {
    try{
        this->username = username;
        if (!_connect.is_open()) {
            std::cerr << "Can't open database" << std::endl;
            return;
        }

        CheckUser();

        if (!if_exist) {
            std::cerr << "User " << this->username << " does not exist" << std::endl;
            return;
        }

        if (type != "email"|type != "password"|type != "permission"|type != "token"|type != "balance"){
            std::cerr << "Invalid Type: " << type << std::endl;
            return;
        }

        std::string _value;
        if (type == "password")
            _value = UserManager::GenerateSaltedSha256(value);
        else
            _value = value;

        pqxx::work U(_connect);
        U.exec_params(
                "UPDATE user_info SET " + type + " = $1 WHERE username = $2",
                _value, this->username
                );
        U.commit();

    } catch (const std::exception &e) {
        std::cerr << "Change User " << username << "'s Info Error: " << e.what() << std::endl;
    }
}