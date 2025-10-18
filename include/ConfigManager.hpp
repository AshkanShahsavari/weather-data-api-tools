#pragma once

#include <nlohmann/json.hpp>
#include <mutex>
#include <string>
#include <unordered_map>
#include <iostream>
#include <fstream>

#include "Logger.hpp"

/**
 * @class ConfigManager
 * @brief Singleton class to manage configuration settings.
 * 
 * Loads and provides access to environment variables and configuration file settings.
 */
class ConfigManager {
private:
    std::unordered_map<std::string, std::string> envValues; ///< Map of environment variable values
    nlohmann::json configFileValues; ///< JSON object containing configuration file values
    static ConfigManager* instance; ///< Singleton instance of ConfigManager
    static std::mutex mutex; ///< Mutex for thread-safe singleton instantiation

    /**
     * @brief Constructor for ConfigManager.
     * 
     * Loads environment variables and configuration file.
     */
    ConfigManager();  

    // Preventing copy and assignment.
    ConfigManager(const ConfigManager&) = delete;
    ConfigManager& operator=(const ConfigManager&) = delete;

    void loadEnvVariables(); ///< Loads environment variables into envValues
    void loadConfigFile(const std::string& filename); ///< Loads configuration file settings into configFileValues

public:
    /**
     * @brief Retrieves the singleton instance of ConfigManager.
     * @return Reference to the singleton ConfigManager instance.
     */
    static ConfigManager& getInstance();

    /**
     * @brief Retrieves the value of an environment variable.
     * @param key The name of the environment variable.
     * @return The value of the environment variable if found, or an empty string.
     */
    std::string getEnv(const std::string& key) const;

    /**
     * @brief Retrieves a configuration value from the config file.
     * @param key The key for the desired configuration setting.
     * @return JSON object representing the configuration value.
     */
    nlohmann::json getConfig(const std::string& key) const;
};
