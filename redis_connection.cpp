//
// redis_connection.cpp
// Created by TheFatNinja 
// 18-10-2019
//

#include "redis_connection.h"

std::map<KeyType, std::string> KeyTypeName {
        {KeyTypeString, "string"},
        {KeyTypeList, "list"},
        {KeyTypeSet, "set"},
        {KeyTypeZSet, "zset"},
        {KeyTypeHash, "hash"},
        {KeyTypeStream, "stream"}
};
