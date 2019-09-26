//
// serialiser.cpp
// Created by TheFatNinja 
// 26-09-2019
//

#include "serialiser.h"
#include <iostream>
#include <fstream>

bool Serialiser::Serialise(const SerialiserMap &map, const std::string &path) {
    std::ofstream serialisation_file(path);
    for (const auto &section_iterator : map) {
        auto section_name = section_iterator.first;
        auto section_properties = section_iterator.second;
        // TODO: write section
        for (const auto &property: section_properties) {
            auto property_name = property.first;
            auto property_accessor = property.second;
            // TODO: write properties
        }
    }
    return false;
}

bool Serialiser::Deserialise(const SerialiserMap &map, const std::string &path) {
    return false;
}
