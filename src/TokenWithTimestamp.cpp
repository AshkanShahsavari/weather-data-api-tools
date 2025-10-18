#include "TokenWithTimestamp.hpp"

TokenWithTimestamp::TokenWithTimestamp() {
    token = ""; 
    updateTimestamp(); 
}

void TokenWithTimestamp::updateTimestamp() {
    lastUpdated = std::time(nullptr);
}

void TokenWithTimestamp::setToken(const std::string& newToken) {
    token = newToken;
    updateTimestamp();
}

std::string TokenWithTimestamp::getToken() const {
    return token;
}

std::string TokenWithTimestamp::getLastUpdatedTime() const {
    return ctime(&lastUpdated);
}

bool TokenWithTimestamp::isUpdatedWithinLastHours(const int hours) const {
    time_t now = std::time(nullptr);
    double seconds = static_cast<double>(hours) * 60 * 60;
    
    return difftime(now, lastUpdated) < seconds;
}

TokenWithTimestamp::~TokenWithTimestamp() {
    // Destructor logic if necessary
}
