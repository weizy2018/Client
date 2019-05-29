#ifndef UTILS_H
#define UTILS_H

#include <string>

#define LOGIN_ACTION            0
#define SEND_MESSAGE_ACTION     1

#define TYPE_LENGTH             6
#define ACCOUNT_LENGTH          21
#define PASSWORD_LENGTH         21
#define MESSAGE_LENGTH          150

#define PORT                    6688
#define SERVER_ADDR             "127.0.0.1"

#define LOGIN_SUCCESS           "success"
#define LOGIN_FAIL              "fail"

using namespace std;

struct SendContext {
    int type;
    char receiver[ACCOUNT_LENGTH];      //如果type==LOGIN_ACTION 则receiver的内容为password， sender的内容为account
    char sender[ACCOUNT_LENGTH];
    char message[MESSAGE_LENGTH];
};

struct User {
    char account[ACCOUNT_LENGTH];
    char password[PASSWORD_LENGTH];
};

class Utils
{
private:
    Utils();
    Utils(const Utils &);
    Utils & operator=(const Utils &);

    static Utils * instance;

public:
    int sockedfd;
    char receiver[ACCOUNT_LENGTH];
    char sender[ACCOUNT_LENGTH];

public:
    static Utils * getInstance();

};

#endif // UTILS_H
