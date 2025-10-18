#include "InfluxDBClient.hpp"

// Constructor to initialize an InfluxDB client.
InfluxDBClient::InfluxDBClient(
    const std::string influxdbHost, // InfluxDB host address.
    const int influxdbPort,         // InfluxDB port number.
    const std::string influxdbUser, // InfluxDB username.
    const std::string influxdbPassword, // InfluxDB password.
    const std::string influxdbDatabase, // InfluxDB database name.
    const int maxConnectionRetries, // Maximum number of connection retries.
    const int connectionretryInterval // Interval between connection retries in seconds.
) : maxConnectionRetries(maxConnectionRetries), 
    connectionretryInterval(connectionretryInterval) {

    // Constructing the connection URL for InfluxDB.
    std::string connection_url = "http://" + influxdbUser + ":" 
                                           + influxdbPassword 
                                           + "@" + influxdbHost 
                                           + ":" + std::to_string(influxdbPort)
                                           + "?db=" 
                                           + influxdbDatabase;

    try {
        // Attempt to connect to InfluxDB using the provided URL.
        influxdb = influxdb::InfluxDBFactory::Get(connection_url);
        // Create the database if it does not exist.
        influxdb->createDatabaseIfNotExists();
        Logger::getInstance().logInfo("Connected to InfluxDB successfully.");
    } catch (const std::exception& e) {
        Logger::getInstance().logError("Error connecting to InfluxDB: " + std::string(e.what()));

        // Retry connection based on the max retries specified.
        for (int i = 0; i < maxConnectionRetries; ++i) {
            // Wait for the specified interval before retrying.
            std::this_thread::sleep_for(std::chrono::seconds(connectionretryInterval));
            try {
                // Retry connection.
                influxdb = influxdb::InfluxDBFactory::Get(connection_url);
                influxdb->createDatabaseIfNotExists();
                Logger::getInstance().logInfo("Reconnected to InfluxDB successfully.");
                break; 
            } catch (const std::exception&) {
                Logger::getInstance().logError("Error reconnecting to InfluxDB: " + std::string(e.what()));
            }
        }
        Logger::getInstance().logError("Failed to connect to InfluxDB despite " 
                                       + std::to_string(maxConnectionRetries) + " attempts.");
        std::exit(EXIT_FAILURE);
    }
}

// Method to write JSON data to InfluxDB.
void InfluxDBClient::writeJsonData(const nlohmann::json& json, const std::string measurement) {
    // Iterate through each data item in the JSON.
    for (const auto& dataItem : json) {
        std::string fieldKey = dataItem["fieldKey"];
        // Process each data item for InfluxDB.
        processParameterDataForInfluxDB(measurement, fieldKey, dataItem);
    }
    Logger::getInstance().logInfo("JSON data written to InfluxDB successfully.");
}

// Helper method to process and write individual data points to InfluxDB.
void InfluxDBClient::processParameterDataForInfluxDB(
    const std::string& measurement,
    const std::string& fieldKey, 
    const nlohmann::json& dataItem
) {
    try {
        // Iterate through each value in the data item.
        for (const auto& valueItem : dataItem["values"]) {
            // Extract and parse the datetime string.
            std::string strDatetime = valueItem["timestamp"];
            auto timePoint = createTimePointFromIso(strDatetime);
            // Extract the value.
            double value = valueItem["value"];

            // Write the data point to InfluxDB.    
            influxdb->write(influxdb::Point{measurement}.addField(fieldKey, value).setTimestamp(timePoint));
        }
    } catch (const std::exception& e) {
        Logger::getInstance().logError("Error processing parameter data for InfluxDB: " + std::string(e.what()));
        // Consider how to handle this error, e.g., retry, ignore, halt etc.
    }
}

// Method to convert an ISO8601 date string to a time_point.
std::chrono::system_clock::time_point InfluxDBClient::createTimePointFromIso(const std::string& dateIso) {
    std::tm tm = {};
    std::istringstream ss(dateIso);
    ss >> std::get_time(&tm, ISO8601_FORMAT);

    if (ss.fail()) {
        throw std::runtime_error("Invalid date/time format in createTimePointFromIso");
    }

    std::time_t t = std::mktime(&tm);
    if (t == -1) {
        throw std::runtime_error("Invalid date/time in createTimePointFromIso");
    }

    return std::chrono::system_clock::from_time_t(t);
}
