#include "exchange.h"

class Jupiter : public Exchange
{
private:
    /* data */
public:
    double getPrice(const char* id);
    Jupiter();
    ~Jupiter();
};


