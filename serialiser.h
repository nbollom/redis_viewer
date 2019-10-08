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
#include <iostream>

typedef enum {
    BooleanType,
    IntegerType,
    LongType,
    FloatType,
    DoubleType,
    CharacterType,
    StringType,
} ValueType;

struct SerialiserValue {
    ValueType value_type;
    std::function<void*()> GetValue;
    std::function<void(void*)> SetValue;
};

typedef std::map<std::string, std::map<std::string, SerialiserValue>> SerialiserMap;

class Serialisable {

private:
    std::string class_name;
    SerialiserMap serialiser_map;

    static void GetNameValue(const std::string &line, std::string &name, std::string &value);

protected:
    template <typename T>
    void SerialisableProperty(const std::string &section, const std::string &name, ValueType type, T &reference, T default_value) {
        // Set value to default and let the read overwrite later
        reference = default_value;
        serialiser_map[section][name] = {
                type,
                [&]() { return &reference; },
                [&](void *value) { reference = *((T*)value); }
        };
    }

public:
    explicit Serialisable(std::string name);
    bool Serialise(const std::string &path);
    bool Deserialise(const std::string &path);

};

#endif //REDIS_VIEWER_SERIALISER_H
