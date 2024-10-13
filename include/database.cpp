#include "database.h"
#include <format>
#include <iostream>

#define UNUSED(x) (void)(x)

typedef std::string string_f;

void Database::terminate(int code)
{
    if(code != 0)
        fprintf(stderr, "%s\n", PQerrorMessage(conn));

    if(res != NULL)
        PQclear(res);

    if(conn != NULL)
        PQfinish(conn);

    exit(code);
}

void Database::processNotice(void *arg, const char *message)
{
    UNUSED(arg);
    UNUSED(message);

    // do nothing
}

void Database::clearRes()
{
    PQclear(res);
    res = NULL;
}

Database::Database(const char *user, const char *password, const char *host, const char *dbname)
{
    string_f str = std::format("user={} password={} host={} dbname={}", user, password, host, dbname);
    conn = PQconnectdb(str.c_str());
    if(PQstatus(conn) != CONNECTION_OK)
        terminate(1);
    PQsetNoticeProcessor(conn, processNotice, NULL);
}

void Database::insert_token_id(const std::string table, const std::string token_id)
{
    string_f query_str = std::format("INSERT INTO {} (token_id) VALUES ('{}');", table, token_id);
    const char *query_constChar = query_str.c_str();
    res = PQexec(conn, query_constChar);
    if(PQresultStatus(res) != PGRES_COMMAND_OK)
        terminate(1);
    clearRes();
}

void Database::multi_insert_token_id(const std::string table, const nlohmann::json objJson)
{
    const int range = objJson.size();

    std::string multi_row_query;

    // составление строки вида "('abc'), ('abc'), ('abc'), ('abc');"
    for (int i = 0; i < range; i++)
    {
      multi_row_query.append("('" + objJson[i].dump().substr(1, objJson[i].dump().size() -2) + "')");
      if (i != range - 1)
        multi_row_query.append(",");
    }

    string_f query_str = std::format("INSERT INTO {} (token_id) VALUES {};", table, multi_row_query);
    res = PQexec(conn, query_str.c_str());

    if(PQresultStatus(res) != PGRES_COMMAND_OK)
        terminate(1);
    clearRes();
}

std::vector<std::string> Database::get_tags(const std::string table)
{
    std::vector<std::string> tagsVector;
    res = PQexec(conn, "SELECT tag FROM exchanges;");
    int nrows = PQntuples(res);
    for(int i = 0; i < nrows; i++)
    {
        char* tag = PQgetvalue(res, i, 0);
        tagsVector.push_back(tag);
    }
    return tagsVector;
}

void Database::update_prices(const std::string table, std::vector<std::string> tags, std::vector<double> prices)
{
    res = PQexec(conn, "BEGIN;");
    for (int i = 0; i < prices.size(); i++){
        string_f query_str = std::format("UPDATE {} SET price = {} WHERE tag = '{}';", table, prices[i], tags[i]);
        res = PQexec(conn, query_str.c_str());
    }
    res = PQexec(conn, "COMMIT;");
}

Database::~Database()
{
    PQclear(res);
    PQfinish(conn);
}
