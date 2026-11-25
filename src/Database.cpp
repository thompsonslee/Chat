#include "Database.h"
#include "config.h"
#include <optional>
#include <stdexcept>
#include <iostream>


Database::Database(){
    int errCode = sqlite3_open_v2(
            config::dbPath,
            &dbConnection,
            SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE
            ,nullptr
        );

    if(errCode != 0){
        
        std::string errStr(std::string("failed to open db. errorCode: ") + sqlite3_errstr(errCode));
        throw std::runtime_error(errStr);
    };
    
    std::string createUserTableSQLString = 
R"(CREATE TABLE IF NOT EXISTS users(
    id          INTEGER PRIMARY KEY,
    username    TEXT NOT NULL UNIQUE,
    hash        BLOB NOT NULL,
    salt        BLOB NOT NULL 
    )
)";
    DatabaseQuery dq(dbConnection, createUserTableSQLString);


    int stepErrCode = dq.step();
    //Todo: check err code
}

void Database::execQuery(std::string &SQL){

}

void Database::test(){
    
}

DatabaseQuery::DatabaseQuery(sqlite3 *dbConnection, const std::string &SQL){
    if(dbConnection == nullptr){
        throw new std::runtime_error("databaseQuery was provided a null pointer for *dbConnection");
    }

    this->dbConnection = dbConnection;

    int errCode = sqlite3_prepare_v3(
            dbConnection, 
            SQL.data(), 
            SQL.size(), 
            0,
            &statement,
            NULL);


    if(errCode != 0){
        std::string err = "failed to prepare SQL Statement. error: ";
        std::string sqlErrCodestr (sqlite3_errstr(errCode));

        throw std::runtime_error(err + sqlErrCodestr);
    }
}

int DatabaseQuery::step(){
    return sqlite3_step(this->statement);
}


void DatabaseQuery::bind(int index, const std::string &text){
    if(sqlite3_bind_text(
                this->statement, 
                index, 
                text.data(),
                text.size(),
                SQLITE_TRANSIENT
                )
       != 0){
        throw new std::runtime_error("failed to bind text to SQL statement");
      }
}

void DatabaseQuery::bind(int index, int num){
    if(sqlite3_bind_int(
                this->statement, 
                index, 
                num
                )
       != 0){
        throw new std::runtime_error("failed to bind number to SQL statement");
      }
}

void DatabaseQuery::bindblob(int index, const Hash *hash, int size){
    DatabaseQuery::bindblob(
            index,
            reinterpret_cast<const unsigned char*>(hash),
            size
            );
}

void DatabaseQuery::bindblob(int index, const Salt *salt, int size){
    DatabaseQuery::bindblob(
            index,
            reinterpret_cast<const unsigned char*>(salt),
            size
            );
}

void DatabaseQuery::bindblob(int index, const unsigned char *blob, int size){
    if(sqlite3_bind_blob(
                this->statement, 
                index, 
                blob, 
                size, 
                SQLITE_TRANSIENT
                )
            != 0){

        throw new std::runtime_error("failed to bind blob to SQL statement");
    }
}
std::string DatabaseQuery::getText(int rowCol){
    const unsigned char * dbresult = sqlite3_column_text(this->statement, rowCol);
    int colBytes = sqlite3_column_bytes(this->statement, rowCol);
    if(colBytes != 32){

    }
    return dbresult
        ? std::string(reinterpret_cast<const char *>(dbresult))
        : std::string();
}

int DatabaseQuery::getNum(int rowCol){
    return sqlite3_column_int(this->statement, rowCol);
}

std::optional<Hash> DatabaseQuery::getHash(int rowCol){
    const void *dbresult = sqlite3_column_blob(this->statement, rowCol);
    int size = sqlite3_column_bytes(this->statement, rowCol);

    if(size == 0){
        return std::nullopt;
    }
    if(size != 32){
        throw std::runtime_error("non zero bytes hash was not the expected 32 amount");
    }

    Hash hash;
    std::memcpy(&hash, dbresult, 32);
    return hash;
}

std::optional<Salt> DatabaseQuery::getSalt(int rowCol){
    const void *dbresult = sqlite3_column_blob(this->statement, rowCol);
    int size = sqlite3_column_bytes(this->statement, rowCol);

    if(size == 0){
        return std::nullopt;
    }
    if(size != 32){
        throw std::runtime_error("non zero bytes salt was not the expected 32 amount");
    }

    Salt salt;
    std::memcpy(&salt, dbresult, 32);
    return salt;
}

int DatabaseQuery::getResultsCount(){
    return sqlite3_column_count(this->statement);
};
