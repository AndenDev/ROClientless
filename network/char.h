#pragma once
#include <winsock2.h>
#include "login.h"

struct CharSession {
    SOCKET socket;
    uint32_t char_id;
    sockaddr_in map_server;
};

bool connect_char_server(const LoginSession& login, CharSession& character);
