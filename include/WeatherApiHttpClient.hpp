#pragma once

#include <cpr/cpr.h>
#include <iostream>
#include <nlohmann/json.hpp>
#include <string>
#include "ConfigManager.hpp"
#include "HttpClient.hpp"
#include "TokenWithTimestamp.hpp"
#include "Logger.hpp"

/**
 * @class WeatherApiHttpClient
 * @brief Client for interacting with the weather API.
 * 
 * Inherits from HttpClient and adds specific functionality for handling weather API requests.
 */
class WeatherApiHttpClient : public HttpClient {
public:
    /**
     * @brief Constructs a WeatherApiHttpClient.
     * @param apiUrl Base URL of the weather API.
     * @param tokenUrl URL to fetch the access token.
     * @param tokenLifetime The token lifetime in hours.
     * @param username Username for weather API.
     * @param password Password for weather API.
     */
    WeatherApiHttpClient(
        const std::string apiUrl,
        const std::string tokenUrl,
        const int tokenLifetime,
        const std::string username,
        const std::string password
    );

    /**
     * @brief Performs a request to the weather API.
     * @param startInterval Start time of the data request interval.
     * @param endInterval End time of the data request interval.
     * @param parameters Parameters for the API request.
     * @param locations Locations for the API request.
     * @param format Format of the API response.
     * @return True if the request was successful, False otherwise.
     */
    bool performRequest(
        const std::tm& startInterval,
        const std::tm& endInterval,
        const std::vector<std::string>& parameters,
        const std::string& locations,
        const std::string& format
    );

    const cpr::Response& getResponse() const; ///< Returns the raw API response.
    const nlohmann::json& getJsonResponse() const; ///< Returns the JSON API response.

    /**
     * @brief Fetches an access token from the token URL.
     * @return True if the token was successfully fetched, False otherwise.
     */
    bool fetchAccessToken();

    /**
     * @brief Converts the API response into a standardized format for InfluxDB.
     * @return JSON object in a standardized format.
     */
    nlohmann::json getStandardizedResponseForInfluxDB();

private:
    static constexpr const char* ISO8601_FORMAT = "%Y-%m-%dT%H:%M:%SZ"; ///< ISO 8601 date format.
    const std::string apiUrl;
    const std::string tokenUrl;
    const int tokenLifetime;
    const std::string username;
    const std::string password;
    cpr::Response apiResponse; ///< Stores the last API response.
    nlohmann::json apiJsonResponse; ///< Stores the last JSON API response.
    TokenWithTimestamp accessToken; ///< Stores the access token.

    /**
     * @brief Creates a time range string in ISO 8601 format.
     * @param start Start time of the interval.
     * @param end End time of the interval.
     * @return String representing the time range.
     */
    std::string createTimeRangeString(const std::tm& start, const std::tm& end);
};
