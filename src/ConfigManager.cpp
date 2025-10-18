#include "ConfigManager.hpp"

// Static variables for the singleton instance and mutex.
ConfigManager* ConfigManager::instance = nullptr;
std::mutex ConfigManager::mutex;

// Constructor of ConfigManager.
ConfigManager::ConfigManager() {
    loadEnvVariables(); // Load environment variables.
    loadConfigFile("config/config.json"); // Load configuration from a JSON file.
    Logger::getInstance().logInfo("ConfigManager initialized"); // Log initialization.
}

// Method to load environment variables.
void ConfigManager::loadEnvVariables() {
    const char* env_vars[] = {"PROJECT", 
                              "API_USERNAME", 
                              "API_PASSWORD", 
                              "INFLUXDB_USER", 
                              "INFLUXDB_PASSWORD", 
                              /* ... other variables ... */ nullptr};
    for (const char** var = env_vars; *var != nullptr; ++var) {
        const char* value = std::getenv(*var);
        if (value) {
            envValues[*var] = value;
        }
    }
    Logger::getInstance().logInfo("Environment variables loaded");
}

// Method to load configuration from a JSON file.
void ConfigManager::loadConfigFile(const std::string& filename) {
    std::ifstream file(filename);
    if (file) {
        file >> configFileValues;
        Logger::getInstance().logInfo("Config file loaded successfully");
    } else {
        Logger::getInstance().logError("Failed to open config file: " + filename);
    }
}

// Static method to get the singleton instance of ConfigManager.
ConfigManager& ConfigManager::getInstance() {
    std::lock_guard<std::mutex> lock(mutex);
    if (instance == nullptr) {
        instance = new ConfigManager();
    }
    return *instance;
}

// Method to get a value from environment variables.
std::string ConfigManager::getEnv(const std::string& key) const {
    auto it = envValues.find(key);
    if (it != envValues.end()) {
        return it->second;
    }
    return "";
}

// Method to get a configuration value from the JSON file.
nlohmann::json ConfigManager::getConfig(const std::string& key) const {
    return configFileValues.value(key, nlohmann::json{});
}
