//
// Created by Tobey Ragain on 3/3/22.
//
// Each user will have ONE row in the database's
// table to store their configurations
//

#ifndef CREDENTIAL_CAVERN_SERVER_INCLUDE_USERCONFIG_H_
#define CREDENTIAL_CAVERN_SERVER_INCLUDE_USERCONFIG_H_

#include "SqlConnector.h"
#include "User.h"

enum logout_period_e {TWO_MIN, FIVE_MIN, TEN_MIN, FORTY_FIVE_MIN, ONE_DAY, NEVER};
enum theme_color_e {BLUE, GREEN, RED, PURPLE};

typedef struct
{
    bool            _encryption_enabled;
    logout_period_e _logout_time;
    theme_color_e   _account_theme;
} configurations_t;

class UserConfig
{
    public:
        UserConfig() = delete;
        UserConfig(SqlConnector * connection, target_user_t user_table_id) :
           _user_id(user_table_id),
           _server(connection)
           { };

        inline configurations_t pull_configs();
        UserConfig& push_logout_period(logout_period_e period);
        UserConfig& push_theme(theme_color_e theme_color);
        UserConfig& toggle_encryption();

    private:
        target_user_t       _user_id;
        SqlConnector *      _server;

};

#endif //CREDENTIAL_CAVERN_SERVER_INCLUDE_USERCONFIG_H_
