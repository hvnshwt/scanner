#include <libpq-fe.h>
#include <memory>

class Database
{
private:
    PGconn* conn = NULL;
    PGresult* res = NULL;

private:
    void terminate(int code);
    static void processNotice(void *arg, const char *message);
    void clearRes();

public:
    // Constructor
    Database(const char* user, const char* password, const char* host, const char* dbname);

    // Inserting a row into a table, row(token_id, tag, price)
    void insert_token_id(const char* table, const char* token_id);
    void insert_token_info(const char* table, const char* token_id, const char* tag, const double price) const;
    void insert_price(const char* table, const char* token_id, const double price) const;

    // Destructor
    ~Database();
};
