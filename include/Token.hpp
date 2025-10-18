#pragma once

#include <string>

/**
 * @class Token
 * @brief Abstract base class for managing access tokens or similar credentials.
 * 
 * Provides an interface for setting and getting tokens, ensuring that derived 
 * classes implement these functionalities.
 */
class Token {
public:
    /**
     * @brief Sets the token value.
     * @param newToken The new token value to be set.
     */
    virtual void setToken(const std::string& newToken) = 0;

    /**
     * @brief Retrieves the current token value.
     * @return The current token as a string.
     */
    virtual std::string getToken() const = 0;

    /**
     * @brief Virtual destructor.
     */
    virtual ~Token() {}
};

