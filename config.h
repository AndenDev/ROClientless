#pragma once
#include <cstdint>


#define RO_SERVER_IP     "127.0.0.1"   // Private server IP (adjust as needed)
#define RO_LOGIN_PORT    6900          // Login server port

#define RO_USERNAME      "player"    // Ragnarok username
#define RO_PASSWORD      "123456"    // Ragnarok password
#define RO_PACKET_VER    0x37      // Matches client version/server packet_ver

#define RO_SELECTED_CHAR_INDEX 0       // Index of character to use (0 = first)
#define RO_SEX                 0       // 0 = female, 1 = male (required in some packets)
