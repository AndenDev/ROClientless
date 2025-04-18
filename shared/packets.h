#pragma once
#include <cstdint>

#pragma pack(push, 1)


enum PacketID : uint16_t {
    // Login stage
    PACKET_LOGIN_REQUEST = 0x0064,
    PACKET_LOGIN_SUCCESS = 0x0AC4,

    // Character select
    PACKET_CHAR_SERVER_REQ = 0x0065,
    PACKET_CHAR_LIST_RESPONSE = 0x006B,
    PACKET_ENTER_MAP = 0x0066,
    PACKET_ENTER_MAP_ACK = 0x0AC5,

    // Map login (after char select)
    PACKET_MAP_LOGIN = 0x0436,
    PACKET_MAP_LOGIN_SUCCESS = 0x0073,
};


// Login success packet from server (0x0AC4)
#pragma pack(push, 1)
struct LoginSuccessPacket {
    PacketID packet_id;     // 0x0AC4
    char padding_001[2];// Align to where login_id1 starts
    uint32_t login_id1;
    uint32_t account_id;
    uint32_t login_id2;
    char padding_002[30];
    uint8_t sex;
    char padding_003[17];
    uint8_t char_ip[4];
    uint16_t char_port;
};
#pragma pack(pop)

#pragma pack(push, 1)
struct MapACKPacket {
    PacketID packet_id;      // 0x0AC5
    uint32_t char_id;
    char map_name[16];       // e.g., "prontera.gat"
    uint8_t map_ip[4];
    uint16_t map_port;
};
#pragma pack(pop)