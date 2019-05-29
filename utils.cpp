#include "utils.h"

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
