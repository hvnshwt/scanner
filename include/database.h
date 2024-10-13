#include <libpq-fe.h>
#include <memory>
#include <string>
#include <nlohmann/json.hpp>
#include <vector>

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
    void insert_token_id(const std::string table, const std::string token_id);
    void multi_insert_token_id(const std::string table, const nlohmann::json objJson);
    void insert_token_info(const char* table, const char* token_id, const char* tag, const double price) const;
    void insert_price(const char* table, const char* token_id, const double price) const;

    std::vector<std::string> get_tags(const std::string table);

    // Destructor
    ~Database();
};
