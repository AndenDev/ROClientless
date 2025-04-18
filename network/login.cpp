#include "login.h"
#include "../config.h"
#include "../shared/logger.h"
#include "../shared/packets.h"
#include <iostream>
#include <cstring>
#include <ws2tcpip.h>

/*
 * Populates the login packet (0x0064) with credentials and version info.
 */
void fill_login_packet(unsigned char* packet) {
    memset(packet, 0, 55);

    // Packet ID: 0x0064
    PacketID packetId = PACKET_LOGIN_REQUEST;
    packet[0] = packetId & 0xFF;
    packet[1] = (packetId >> 8) & 0xFF;

    // Client version
    packet[2] = RO_PACKET_VER & 0xFF;
    packet[3] = (RO_PACKET_VER >> 8) & 0xFF;
    packet[4] = (RO_PACKET_VER >> 16) & 0xFF;
    packet[5] = (RO_PACKET_VER >> 24) & 0xFF;

    // Credentials
    snprintf((char*)&packet[6], 24, "%s", RO_USERNAME);
    snprintf((char*)&packet[30], 24, "%s", RO_PASSWORD);

    // Client type flag
    packet[54] = 0x01;
}

/*
 * Connects to the login server and performs authentication.
 * On success, fills the LoginSession structure.
 */
bool connect_login_server(LoginSession& session) {
    session.socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (session.socket == INVALID_SOCKET) {
        log("[!] Failed to create login socket.");
        return false;
    }

    sockaddr_in addr{};
    addr.sin_family = AF_INET;
    addr.sin_port = htons(RO_LOGIN_PORT);
    inet_pton(AF_INET, RO_SERVER_IP, &addr.sin_addr);

    log("*** Connecting to Login Server...");
    if (connect(session.socket, (sockaddr*)&addr, sizeof(addr)) != 0) {
        log("[!] Could not connect to login server.");
        return false;
    }

    log("-> Connected. Sending login packet (0x0064)...");
    unsigned char packet[55];
    fill_login_packet(packet);
    send(session.socket, (char*)packet, sizeof(packet), 0);

    // Receive response
    unsigned char buffer[1024];
    int len = recv(session.socket, (char*)buffer, sizeof(buffer), 0);
    if (len <= 0) {
        log("[!] No response from login server.");
        return false;
    }

    // Check login result
    PacketID packet_id = *(PacketID*)buffer;
    if (packet_id != PACKET_LOGIN_SUCCESS) {
        log("[!] Login failed. Unexpected packet ID.");
        return false;
    }

    if (len < sizeof(LoginSuccessPacket)) {
        log("[!] Login response packet too short.");
        return false;
    }

    // Extract login info
    auto* login = reinterpret_cast<LoginSuccessPacket*>(buffer);
    login->char_port = (*((uint8_t*)&login->char_port) << 8) | *(((uint8_t*)&login->char_port) + 1);

    session.account_id = login->account_id;
    session.login_id1 = login->login_id1;
    session.login_id2 = login->login_id2;
    session.sex = static_cast<uint8_t>(login->sex);
    memcpy(session.session_key, ((uint8_t*)login) + 0x10, 4);

    char ip_str[INET_ADDRSTRLEN];
    inet_ntop(AF_INET, login->char_ip, ip_str, sizeof(ip_str));
    session.char_server.sin_family = AF_INET;
    session.char_server.sin_port = login->char_port;
    memcpy(&session.char_server.sin_addr, login->char_ip, 4);

    // Log
    log("-> Login successful!");
    log("-> Char Server IP: " + std::string(ip_str) + ":" + std::to_string(ntohs(login->char_port)));

    return true;
}
