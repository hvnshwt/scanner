#include "database.h"
#include <format>

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

void Database::insert_token_id(const char *table, const char *token_id)
{
    string_f query_str = std::format("INSERT INTO {} (token_id) VALUES ('{}');", table, token_id);
    const char *query_constChar = query_str.c_str();
    res = PQexec(conn, query_constChar);
    if(PQresultStatus(res) != PGRES_COMMAND_OK)
        terminate(1);
    clearRes();
}

Database::~Database()
{
    PQfinish(conn);
}
