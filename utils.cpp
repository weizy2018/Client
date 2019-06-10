#include "utils.h"

#include <ctime>

Utils * Utils::instance = nullptr;
Utils::Utils()
{

}

Utils * Utils::getInstance() {
    if (instance == nullptr) {
        instance = new Utils;
    }
    return instance;
}

string Utils::getCurrentTime() {
    time_t nowtime;
    nowtime = time(NULL);
    char tmp[TIME_LENGTH];
    strftime(tmp,sizeof(tmp),"%Y-%m-%d %H:%M:%S",localtime(&nowtime));
    return tmp;
}
