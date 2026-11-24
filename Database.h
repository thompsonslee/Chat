#pragma once
#include <sqlite3.h>
#include <string>
#include"./models/Hasher.h"

class Database{
    public: 
        Database() ;
        void execQuery(std::string &SQL) ;
        void test();
        sqlite3* getDbConnection(){return dbConnection;}
    private:
        sqlite3 *dbConnection;
};

class DatabaseQuery{
    public:
        DatabaseQuery(sqlite3 *dbConnection, const std::string &SQL);
        int step() ;
        void bind(int index, const std::string &text);
        void bind(int index, int num);
        void bindblob(int index, const unsigned char *blob, int size);
        void bindblob(int index, const Hash *hash, int size);
        void bindblob(int index, const Salt *Salt, int size);
        int getResultsCount();
        std::string getText(int rowCol);
        int getNum(int rowCol);
        std::optional<Hash> getHash(int rowCol);
        std::optional<Salt> getSalt(int rowCol);
    private:
        sqlite3 *dbConnection;
        sqlite3_stmt *statement;

};
