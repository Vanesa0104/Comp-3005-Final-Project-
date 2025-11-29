#ifndef DB_HPP
#define DB_HPP

#include <libpq-fe.h>
#include <string>
#include <stdexcept>
#include <iostream>

using namespace std;

class Database {
private:
    PGconn* conn;

public:

    // Constructor
    Database(const string& conninfo) {
        conn = PQconnectdb(conninfo.c_str());
        if (PQstatus(conn) != CONNECTION_OK) {
            throw runtime_error(PQerrorMessage(conn));
        }
    }

    // If an error occurs for SQL stuff
    PGresult* exec(const string& q) {
        PGresult* res = PQexec(conn, q.c_str());
        ExecStatusType status = PQresultStatus(res);

        if (status != PGRES_TUPLES_OK && status != PGRES_COMMAND_OK) {
            cerr << "SQL Error: " << PQerrorMessage(conn) << endl;
            PQclear(res);
            return nullptr;
        }

        return res;
    }

    // Deconstructor
    ~Database() {
        PQfinish(conn);
    }
};

#endif
