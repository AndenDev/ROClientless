#include "char.h"
#include "../config.h"
#include "../shared/logger.h"
#include "../shared/packets.h"
#include <iostream>
#include <cstring>
#include <ws2tcpip.h>

/*
 * Connects to the character server using login session info.
 * Requests the character list and selects the configured character index.
 * Fills out the CharSession with map server info on success.
 */
bool connect_char_server(const LoginSession& login, CharSession& character) {
    character.socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    log("*** Connecting to Character Server...");

    // Attempt connection
    if (connect(character.socket, (sockaddr*)&login.char_server, sizeof(sockaddr_in)) != 0) {
        log("[!] Could not connect to character server.");
        return false;
    }

    // Construct and send 0x0065 - Character List Request
    unsigned char req[17] = { 0 };
    PacketID packetId = PACKET_CHAR_SERVER_REQ;
    req[0] = packetId & 0xFF;
    req[1] = (packetId >> 8) & 0xFF;
    memcpy(&req[2], &login.account_id, 4);
    memcpy(&req[6], &login.login_id1, 4);
    memcpy(&req[10], &login.login_id2, 4);
    req[16] = login.sex;

    send(character.socket, (char*)req, sizeof(req), 0);
    log("-> Sent request for character list (0x0065)");

    // Wait for 0x006B - Character List Response
    unsigned char temp_buf[1024];
    int temp_len = 0;
    bool isFound = false;

    while ((temp_len = recv(character.socket, (char*)temp_buf, sizeof(temp_buf), 0)) > 0) {
        for (int i = 0; i < temp_len - 1; ++i) {
            if (temp_buf[i] == 0x6B && temp_buf[i + 1] == 0x00) {
                isFound = true;
                break;
            }
        }
        if (isFound) break;
    }

    if (!isFound) {
        log("[!] Expected 0x006B response, but did not receive it.");
        return false;
    }

    log("<- Received character list (0x006B)");

    // Construct and send 0x0066 - Select character
    unsigned char select_req[3] = { 0 };
    PacketID select_packet_id = PACKET_ENTER_MAP;

    select_req[0] = select_packet_id & 0xFF;
    select_req[1] = (select_packet_id >> 8) & 0xFF;
    select_req[2] = static_cast<uint8_t>(RO_SELECTED_CHAR_INDEX);

    send(character.socket, (char*)select_req, sizeof(select_req), 0);
    log("-> Selected character index " + std::to_string(RO_SELECTED_CHAR_INDEX) + " (0x0066)");

    // Receive 0x0AC5 - Map server connection info
    unsigned char map_buf[1024];
    int map_len = recv(character.socket, (char*)map_buf, sizeof(map_buf), 0);
    if (map_len <= 0) {
        log("[!] No response after character selection.");
        return false;
    }

    auto* resp = reinterpret_cast<MapACKPacket*>(map_buf);
    if (resp->packet_id != PACKET_ENTER_MAP_ACK) {
        log("[!] Unexpected packet received, expected 0x0AC5.");
        return false;
    }

    // Convert port from little endian to host byte order
    resp->map_port = (*((uint8_t*)&resp->map_port) << 8) | *(((uint8_t*)&resp->map_port) + 1);

    // Fill character session with map server data
    character.char_id = resp->char_id;
    character.map_server.sin_family = AF_INET;
    character.map_server.sin_port = resp->map_port;
    memcpy(&character.map_server.sin_addr, resp->map_ip, 4);

    // Convert IP to string for display
    char ip_str[INET_ADDRSTRLEN];
    inet_ntop(AF_INET, resp->map_ip, ip_str, sizeof(ip_str));

    // Display info
    log("-> Character ID: " + std::to_string(resp->char_id));
    log("-> Map Name    : " + std::string(resp->map_name));
    log("-> Map Server  : " + std::string(ip_str) + ":" + std::to_string(ntohs(resp->map_port)));

    return true;
}
