#ifndef vpJsonParsing_HH
#define vpJsonParsing_HH

#ifdef VISP_HAVE_NLOHMANN_JSON
#include <nlohmann/json.hpp>

/*!
Parse the flag values defined in a JSON object.
if the flags are defined as an int, then this is int is directly returned.
If it is defined as a combination of options (defined from an enumeration E) then the logical or of theses enum values is returned.
Beware that invalid values may be defined in the JSON object: the int value may be invalid, or the parsing of enum values may fail.

\param j: the JSON object to parse

\return an int, corresponding to the combination of boolean flags

*/
template<typename E>
int flagsFromJSON(const nlohmann::json& j) {
    int flags = 0;
    if(j.is_array()) {
        flags = 0;
        for(const auto& v: j) {
            E value = v.get<E>(); // If a value is incorrect, this will default to the first value of the enum
            flags |= value;
        }
    } else if(j.is_number_integer()) {
        flags = j.get<int>();
    }
    return flags;
}

/*!
    Serialize flag values as a json array.
    \param flags the value to serialize
    \param options the possible values that can be contained in flags. A flag i is set if flags & options[i] != 0.

    \return a json object (an array) that contains the different flags of the variable flags.

*/
template<typename E>
nlohmann::json flagsToJSON(const int flags, const std::vector<E>& options) {
    nlohmann::json j = nlohmann::json::array();
    for(const E option: options) {
        if(flags & option) {
            j.push_back(option);
        }
    }
    return j;
}

template<typename T>
bool convertFromTypeAndBuildFrom(const nlohmann::json&, T&) {
    return false;
}
template<typename T, typename O, typename... Os>
bool convertFromTypeAndBuildFrom(const nlohmann::json& j, T& t) {
    if(j["type"] == O::jsonTypeName) {
        O other = j;
        t.buildFrom(other);
        return true;
    } else {
        return convertFromTypeAndBuildFrom<T, Os...>(j, t);
    }
}

#endif
#endif