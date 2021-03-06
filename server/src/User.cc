//
// Created by Tobey Ragain on 2/20/22.
//

#include "../include/User.h"

/*
 * Creates a new user entry in the User table.
 */
creation_status_e User::create()
{
    std::string query ("SELECT usern FROM Users WHERE usern= '" + _entry_usern + "'");

    if(mysql_query(_server->connection(), query.c_str()))
    {
        _server->log("SERVER ERROR: Selecting from User table failed");
        return SERVER_FAULT;
    }
    // Check if entered username already exists
    MYSQL_RES * mysqlResult = mysql_store_result(_server->connection());
    if(mysql_num_rows(mysqlResult) != 0)
    {
        _server->log("FAILED: Unsuccessful creation of user: "
                     "username already exists in this database");
        mysql_free_result(mysqlResult);
        return BAD_USERN;
    }
    else
    {

        // Add a user row with the provided entries
        query = "INSERT INTO Users (usern, passph) VALUES "
                "('" + _entry_usern + "', '" + _entry_passph + "')";
        if(mysql_query(_server->connection(), query.c_str()))
        {
            _server->log("SERVER ERROR: Adding a new user has failed");
            mysql_free_result(mysqlResult);
            return SERVER_FAULT;
        }
        _server->log("Created new user: " + _entry_usern);
        mysql_free_result(mysqlResult);
        return SUCCESS;
    }
}

/*
 * With the supplied username and passphrase this functions attempts
 * to login into a user account. If successful, login() will update
 * the _u_table_id data member with the table id of the user account.
 * If authentication is unsuccessful the value will be left at zero.
 */
User& User::login()
{
    std::string query ("SELECT * FROM Users WHERE usern= '" + _entry_usern +
        "' AND passph= '" + _entry_passph + "'");

    // Check if usern and passph exists
    // User creation verifies that no repeat usern are added
    if(mysql_query(_server->connection(), query.c_str()))
    {
        _server->log("SERVER ERROR: Selecting passph User table failed");
        return *this;
    }

    MYSQL_RES * mysqlResult = mysql_store_result(_server->connection());
    if(mysql_num_rows(mysqlResult) == 0)
    {
        _server->log("FAILED: No user found with provided username or password");
        mysql_free_result(mysqlResult);
        return *this;
    }

    // Get the id of the user account
    MYSQL_ROW row = mysql_fetch_row(mysqlResult);
    _u_table_id = row[ID];
    _server->log("Logged in to account: " + _entry_usern);
    mysql_free_result(mysqlResult);
    return *this;
}



