#pragma once
#include <winsock2.h>
#include <cstdint>

struct LoginSession {
    SOCKET socket;
    uint32_t account_id;
    uint32_t login_id1;
    uint32_t login_id2;
    uint8_t session_key[4];
    uint8_t sex;
    sockaddr_in char_server;
};

bool connect_login_server(LoginSession& session);
