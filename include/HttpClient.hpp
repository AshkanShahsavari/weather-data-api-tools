#pragma once

#include <cpr/cpr.h>
#include <nlohmann/json.hpp>
#include <string>
#include <vector>

/**
 * @class HttpClient
 * @brief Abstract base class for HTTP clients.
 * 
 * Defines the interface for HTTP clients to perform requests and process responses.
 */
class HttpClient {
public:
    /**
     * @brief Virtual destructor.
     */
    virtual ~HttpClient() = default;

    /**
     * @brief Performs an HTTP request.
     * @param startInterval Start time of the data request interval.
     * @param endInterval End time of the data request interval.
     * @param parameters Parameters for the API request.
     * @param locations Locations for the API request.
     * @param format Format of the API response.
     * @return True if the request was successful, False otherwise.
     */
    virtual bool performRequest(
        const std::tm& startInterval,
        const std::tm& endInterval,
        const std::vector<std::string>& parameters,
        const std::string& locations,
        const std::string& format
    ) = 0;

    /**
     * @brief Retrieves the raw response from the last HTTP request.
     * @return The response object.
     */
    virtual const cpr::Response& getResponse() const = 0;

    /**
     * @brief Retrieves the JSON response from the last HTTP request.
     * @return The JSON response object.
     */
    virtual const nlohmann::json& getJsonResponse() const = 0;

    /**
     * @brief Converts the raw response into a format suitable for InfluxDB.
     * @return The standardized JSON response.
     */
    virtual nlohmann::json getStandardizedResponseForInfluxDB() = 0;
};
