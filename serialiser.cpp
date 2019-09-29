//
// serialiser.cpp
// Created by TheFatNinja 
// 26-09-2019
//

#include "serialiser.h"
#include <fstream>
#include <utility>

using namespace std::string_literals;

void Serialisable::GetNameValue(const std::string &line, std::string &name, std::string &value) {
    int split = line.find("=");
    name = line.substr(0, split);
    value = line.substr(split + 1);
}

Serialisable::Serialisable(std::string name) {
    class_name = std::move(name);
}

bool Serialisable::Serialise(const std::string &path) {
    std::ofstream serialisation_file(path);
    if (!serialisation_file) {
        return false;
    }
    serialisation_file << "#" << class_name << std::endl;
    for (const auto &section_iterator : serialiser_map) {
        auto section_name = section_iterator.first;
        auto section_properties = section_iterator.second;
        serialisation_file << std::endl << "%" << section_name << std::endl;
        for (const auto &property: section_properties) {
            auto property_name = property.first;
            auto property_accessor = property.second;
            void *value = property_accessor.GetValue();
            ValueType type = property_accessor.value_type;
            switch (type) {
                case BooleanType: {
                    bool boolean_value = *static_cast<bool*>(value);
                    std::string string_value = boolean_value ? "True" : "False";
                    serialisation_file << "b" << property_name << "=" << string_value << std::endl;
                    break;
                }
                case IntegerType: {
                    int integer_value = *static_cast<int*>(value);
                    serialisation_file << "i" << property_name << "=" << std::to_string(integer_value) << std::endl;
                    break;
                }
                case LongType: {
                    long long_value = *static_cast<long*>(value);
                    serialisation_file << "l" << property_name << "=" << std::to_string(long_value) << std::endl;
                    break;
                }
                case FloatType: {
                    float float_value = *static_cast<float*>(value);
                    serialisation_file << "f" << property_name << "=" << std::to_string(float_value) << std::endl;
                    break;
                }
                case DoubleType: {
                    double double_value = *static_cast<double *>(value);
                    serialisation_file << "d" << property_name << "=" << std::to_string(double_value) << std::endl;
                    break;
                }
                case CharacterType: {
                    char character_value = *static_cast<char*>(value);
                    serialisation_file << "c" << property_name << "=" << character_value << std::endl;
                    break;
                }
                case StringType: {
                    std::string string_value = *static_cast<std::string*>(value);
                    serialisation_file << "s" << property_name << "=" << string_value << std::endl;
                    break;
                }
            }
        }
    }
    serialisation_file.close();
    return true;
}

bool Serialisable::Deserialise(const std::string &path) {
    std::ifstream serialisation_file(path);
    if (!serialisation_file) {
        return false;
    }
    std::string line;
    std::string section;
    std::string property_name;
    std::string value;
    while (std::getline(serialisation_file, line)) {
        char ident = line.front();
        switch (ident) {
            case '#': {
                std::string name = line.substr(1);
                if (name != class_name) {
                    std::cerr << "File's class name doesn't match" << std::endl;
                    return false;
                }
                break;
            }
            case '%':
                section = line.substr(1);
                break;
            case 'b': {
                GetNameValue(line.substr(1), property_name, value);
                bool boolean_value;
                if (value == "True") {
                    boolean_value = true;
                }
                else if (value == "False") {
                    boolean_value = false;
                }
                else {
                    std::cerr << "Invalid boolean " << value << std::endl;
                    break;
                }
                serialiser_map[section][property_name].SetValue(&boolean_value);
                break;
            }
            case 'i': {
                GetNameValue(line.substr(1), property_name, value);
                int integer_value = std::stoi(value);
                serialiser_map[section][property_name].SetValue(&integer_value);
                break;
            }
            case 'l': {
                GetNameValue(line.substr(1), property_name, value);
                long long_value = std::stol(value);
                serialiser_map[section][property_name].SetValue(&long_value);
                break;
            }
            case 'f': {
                GetNameValue(line.substr(1), property_name, value);
                float float_value = std::stof(value);
                serialiser_map[section][property_name].SetValue(&float_value);
                break;
            }
            case 'd': {
                GetNameValue(line.substr(1), property_name, value);
                double double_value = std::stod(value);
                serialiser_map[section][property_name].SetValue(&double_value);
                break;
            }
            case 'c': {
                GetNameValue(line.substr(1), property_name, value);
                char character_value = value.front();
                serialiser_map[section][property_name].SetValue(&character_value);
                break;
            }
            case 's': {
                GetNameValue(line.substr(1), property_name, value);
                serialiser_map[section][property_name].SetValue(&value);
                break;
            }
            default:
                if (!line.empty()) {
                    std::cerr << "Unknown ident " << ident << std::endl;
                }
        }
    }
    return true;
}
