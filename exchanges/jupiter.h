#pragma once

#include "exchange.h"
#include <nlohmann/json.hpp>

class Jupiter : public Exchange
{
private:
    /* data */
public:
    double getPrice(const std::string &tag);
    nlohmann::json getAllTokens();
    Jupiter();
    ~Jupiter();
};


