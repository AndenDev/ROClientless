#pragma once
#include <winsock2.h>
#include <cstdint>

// Session info from login
struct LoginSession {
    SOCKET socket;
    uint32_t account_id;
    uint32_t login_id1;
    uint32_t login_id2;
    uint8_t sex;
    sockaddr_in char_server;
};

// Session info for selected character
struct CharSession {
    SOCKET socket;
    uint32_t char_id;
    sockaddr_in map_server;
};
