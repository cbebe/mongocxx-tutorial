#include <iostream>

#include "mongocxx/instance.hpp"

#include "mongodb_handler.h"

void print_result(const bool &result, const char *operation)
{
    std::cout << "the " << operation << (result ? " worked." : " didn't work.") << std::endl;
}

int main()
{
    mongocxx::instance instance;
    MongoTutorial::MongoDbHandler mhandler;

    bool result = mhandler.RemoveCharacterFromDb("60bc2cdd7c0aa10a364bc672");
    print_result(result, "delete");
    std::cout << "Hellowo!" << std::endl;
}