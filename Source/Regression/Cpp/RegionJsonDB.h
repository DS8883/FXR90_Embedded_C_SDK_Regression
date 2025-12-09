// Helper for loading and querying RegionsList.json using nlohmann/json
// Place this file in the Regression/Cpp directory

#include <fstream>
#include <string>
#include <map>
#include <vector>
#include <nlohmann/json.hpp>

struct RegionJsonInfo {
    std::string name;
    std::string mode; // "ETSI" or "FCC"
    int channelCount;
    bool hoppingConfigurable;
    bool channelSelectable;
};

class RegionJsonDB {
public:
    std::map<std::string, RegionJsonInfo> regionMap;

    bool load(const std::string& jsonPath) {
        std::ifstream f(jsonPath);
        if (!f.is_open()) return false;
        nlohmann::json j;
        f >> j;
        for (const auto& entry : j["regions"]) {
            RegionJsonInfo info;
            info.name = entry["RegionName"].get<std::string>();
            info.mode = entry["Mode"].get<std::string>();
            info.channelCount = entry["ChannelCount"].get<int>();
            info.hoppingConfigurable = entry["HoppingConfigurable"].get<bool>();
            info.channelSelectable = entry["ChannelSelectable"].get<bool>();
            regionMap[info.name] = info;
        }
        return true;
    }

    // Returns pointer to info or nullptr if not found
    const RegionJsonInfo* find(const std::wstring& regionNameW) const {
        std::string regionName(regionNameW.begin(), regionNameW.end());
        auto it = regionMap.find(regionName);
        if (it != regionMap.end()) return &it->second;
        return nullptr;
    }
};
