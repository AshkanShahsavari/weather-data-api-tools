#include "WeatherApiHttpClient.hpp"

// Constructor to initialize a WeatherApiHttpClient HTTP client.
WeatherApiHttpClient::WeatherApiHttpClient(
    const std::string apiUrl,           // API base URL for weather.
    const std::string tokenUrl,         // URL to fetch the access token.
    const int tokenLifetime,            // Lifetime of the access token in hours.
    const std::string username,         // Username for authentication.
    const std::string password          // Password for authentication.
) : apiUrl(apiUrl),
    tokenUrl(tokenUrl),
    tokenLifetime(tokenLifetime),
    username(username),
    password(password),
    accessToken() {
    Logger::getInstance().logInfo("WeatherApiHttpClient initialized");
}

// Method to perform a request to the weather API.
bool WeatherApiHttpClient::performRequest(
    const std::tm& startInterval,
    const std::tm& endInterval,
    const std::vector<std::string>& parameters,
    const std::string& locations,
    const std::string& format
) {
    // Check and refresh the access token if necessary.
    if (!accessToken.isUpdatedWithinLastHours(tokenLifetime) || accessToken.getToken().empty()) {
        if (!fetchAccessToken()) {
            Logger::getInstance().logError("Failed to fetch access token for weather API");
            return false;
        }
    }

    std::string strParameters;
    for (const auto& parameter : parameters) {
        if (!strParameters.empty()) {
            strParameters += ",";
        }
        strParameters += parameter;
    }

    // Construct the request URL with all parameters.
    std::string url = apiUrl + "/" 
                      + createTimeRangeString(startInterval, endInterval) + "/" 
                      + strParameters + "/" 
                      + locations + "/" 
                      + format + "?access_token=" + accessToken.getToken();
    
    // Perform the GET request.
    apiResponse = cpr::Get(cpr::Url{url});

    // Handle the response.
    if (apiResponse.status_code == 200) {
        apiJsonResponse = nlohmann::json::parse(apiResponse.text);
        Logger::getInstance().logInfo("Weather API request successful");
        return true;
    } else {
        Logger::getInstance().logError("Weather API request failed, status code: " 
                                       + std::to_string(apiResponse.status_code));
        return false;
    }
}

// Method to get the raw API response.
const cpr::Response& WeatherApiHttpClient::getResponse() const {
    return apiResponse;
}

// Method to get the API response in JSON format.
const nlohmann::json& WeatherApiHttpClient::getJsonResponse() const {
    return apiJsonResponse;
}

// Method to fetch the access token required for API requests.
bool WeatherApiHttpClient::fetchAccessToken() {
    try {
        // Perform an HTTP GET request to the token URL with basic authentication.
        cpr::Response response = cpr::Get(cpr::Url{tokenUrl},
                                          cpr::Authentication{username, password, cpr::AuthMode::BASIC},
                                          cpr::Header{{"Content-Type", "application/json"}});

        if (response.status_code == 200) {
            // Parse the response text to JSON and extract the access token.
            auto jsonResponse = nlohmann::json::parse(response.text);
            accessToken.setToken(jsonResponse["access_token"]);
            Logger::getInstance().logInfo("Access token fetched successfully for weather API");
            return true;
        } else {
            // Handle unsuccessful token fetch attempts.
            Logger::getInstance().logError("Failed to fetch access token, status code: " 
                                           + std::to_string(response.status_code));
            return false;
        }
    } catch (const std::exception& e) {
        Logger::getInstance().logError("Exception caught in fetchAccessToken: " + std::string(e.what()));
        return false;
    }
}

// Method to convert the API response to a format suitable for InfluxDB.
nlohmann::json WeatherApiHttpClient::getStandardizedResponseForInfluxDB() {
    nlohmann::json standardizedJson;

    for (const auto& dataItem : apiJsonResponse["data"]) {
        nlohmann::json measurement; // Initialize an empty JSON object.
        measurement["fieldKey"] = dataItem["parameter"];

        std::vector<nlohmann::json> valuesArray;
        // Iterate over each data item in the API response.
        for (const auto& dateItem : dataItem["coordinates"][0]["dates"]) {
            nlohmann::json value;
            value["timestamp"] = dateItem["date"];
            value["value"] = dateItem["value"];
            valuesArray.push_back(value);
        }

        measurement["values"] = valuesArray;
        standardizedJson.push_back(measurement);
    }

    Logger::getInstance().logInfo("Standardized response for InfluxDB generated");
    return standardizedJson;
}

// Helper method to create a time range string in ISO8601 format.
std::string WeatherApiHttpClient::createTimeRangeString(const std::tm& start, const std::tm& end) {
    std::ostringstream ss;
    ss << std::put_time(&start, ISO8601_FORMAT);
    ss << "--";
    ss << std::put_time(&end, ISO8601_FORMAT);
    return ss.str();
}
