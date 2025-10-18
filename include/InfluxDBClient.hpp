#pragma once

#include <nlohmann/json.hpp>
#include <InfluxDBFactory.h>
#include <string>
#include <unordered_set>
#include <memory>
#include <chrono>
#include <sstream>
#include <iomanip>
#include <stdexcept>
#include <iostream>
#include <thread>

#include "Logger.hpp"

/**
 * @class InfluxDBClient
 * @brief A client for interfacing with InfluxDB.
 */
class InfluxDBClient {
public:
    /**
     * @brief Constructor for InfluxDBClient.
     * @param influxdbHost Hostname of the InfluxDB server.
     * @param influxdbPort Port of the InfluxDB server.
     * @param influxdbUser Username for InfluxDB authentication.
     * @param influxdbPassword Password for InfluxDB authentication.
     * @param influxdbDatabase Database name to connect in InfluxDB.
     * @param maxConnectionRetries Maximum number of retries for connecting to InfluxDB.
     * @param connectionretryInterval Interval in seconds between connection retries.
     */
    InfluxDBClient(
        const std::string influxdbHost, 
        const int influxdbPort,
        const std::string influxdbUser, 
        const std::string influxdbPassword,
        const std::string influxdbDatabase,
        const int maxConnectionRetries,
        const int connectionretryInterval
    );

    /**
     * @brief Writes JSON data to InfluxDB.
     * @param json JSON data to be written.
     * @param measurement The measurement name in InfluxDB.
     */
    void writeJsonData(const nlohmann::json& json, const std::string measurement);

private:
    int maxConnectionRetries;
    std::chrono::seconds connectionretryInterval;
    static constexpr const char* ISO8601_FORMAT = "%Y-%m-%dT%H:%M:%SZ";
    std::unique_ptr<influxdb::InfluxDB> influxdb;

    /**
     * @brief Processes and writes a parameter data to InfluxDB.
     * @param measurement The measurement name in InfluxDB.
     * @param fieldKey The field key in InfluxDB.
     * @param dataItem JSON item containing data to be written.
     */
    void processParameterDataForInfluxDB(
        const std::string& measurement,
        const std::string& fieldKey, 
        const nlohmann::json& dataItem
    );

    /**
     * @brief Creates a time_point from an ISO 8601 date string.
     * @param dateIso ISO 8601 date string.
     * @return A time_point representing the given date and time.
     */
    std::chrono::system_clock::time_point createTimePointFromIso(const std::string& date_iso);
};
