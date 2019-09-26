//
// serialiser.h
// Created by TheFatNinja 
// 26-09-2019
//

#ifndef REDIS_VIEWER_SERIALISER_H
#define REDIS_VIEWER_SERIALISER_H

#include <string>
#include <functional>
#include <map>

struct SerialiserValue {
    std::string value_type;
    std::function<void*()> GetValue;
    std::function<void(void*)> SetValue;
};

typedef std::map<std::string, std::map<std::string, SerialiserValue>> SerialiserMap;

class Serialiser {

public:
    Serialiser() = delete;

    static bool Serialise(const SerialiserMap &map, const std::string &path);
    static bool Deserialise(const SerialiserMap &map, const std::string &path);

};

#endif //REDIS_VIEWER_SERIALISER_H
