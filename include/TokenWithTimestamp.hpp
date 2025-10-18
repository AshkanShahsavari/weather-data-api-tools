#pragma once

#include "Token.hpp"
#include <ctime>
#include <string>

/**
 * @class TokenWithTimestamp
 * @brief A token class that also keeps track of the last update timestamp.
 * 
 * Extends the Token base class to include functionality for timestamp management.
 */
class TokenWithTimestamp : public Token {
private:
    std::string token; ///< The token value.
    time_t lastUpdated; ///< Timestamp of the last token update.

    /**
     * @brief Updates the last updated timestamp to the current time.
     */
    void updateTimestamp();

public:
    /**
     * @brief Constructor for TokenWithTimestamp.
     */
    TokenWithTimestamp(); 

    /**
     * @brief Sets the token value and updates the timestamp.
     * @param newToken The new token value to be set.
     */
    void setToken(const std::string& newToken) override;

    /**
     * @brief Retrieves the current token value.
     * @return The current token as a string.
     */
    std::string getToken() const override;

    /**
     * @brief Retrieves the last updated time as a string.
     * @return The last updated time.
     */
    std::string getLastUpdatedTime() const;

    /**
     * @brief Checks if the token was updated within the last specified number of hours.
     * @param hours The number of hours to check against for the last update.
     * @return True if the token was updated within the specified hours, False otherwise.
     */
    bool isUpdatedWithinLastHours(const int hours) const;

    /**
     * @brief Destructor for TokenWithTimestamp.
     */
    virtual ~TokenWithTimestamp();
};
