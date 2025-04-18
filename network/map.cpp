#include "map.h"
#include "../shared/logger.h"
#include "../shared/packets.h"

#include <iostream>
#include <winsock2.h>
#include <cstring>

/*
 * Connects to the map server using login + character session info.
 * Sends 0x0072 map login packet and a few startup client packets.
 */
bool connect_map_server(const LoginSession& login, const CharSession& character) {
    SOCKET mapSock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    log("*** Connecting to Map Server...");

    if (connect(mapSock, (sockaddr*)&character.map_server, sizeof(sockaddr_in)) != 0) {
        log("[!] Could not connect to map server.");
        return false;
    }

    // Construct 0x0072 - Enter map
    unsigned char enterMap[19] = { 0 };
    PacketID packetId = PACKET_MAP_LOGIN;
    enterMap[0] = packetId & 0xFF;
    enterMap[1] = (packetId >> 8) & 0xFF;
    memcpy(&enterMap[2], &login.account_id, 4);
    memcpy(&enterMap[6], &character.char_id, 4);
    memcpy(&enterMap[10], &login.login_id1, 4);
    memcpy(&enterMap[14], &login.login_id2, 4);
    enterMap[18] = login.sex;

    send(mapSock, (char*)enterMap, sizeof(enterMap), 0);
    log("-> Sent map login packet (0x0072)");

    // Send client initialization packets (to simulate "ready" state)
    unsigned char pkt1[] = { 0x7D, 0x00 };
    send(mapSock, (char*)pkt1, sizeof(pkt1), 0);

    unsigned char pkt2[] = { 0x60, 0x03, 0x70, 0xC8, 0x4B, 0x00 };
    send(mapSock, (char*)pkt2, sizeof(pkt2), 0);

    unsigned char pkt3[] = { 0xC9, 0x08 };
    send(mapSock, (char*)pkt3, sizeof(pkt3), 0);

    unsigned char pkt4[] = { 0x4F, 0x01, 0x00, 0x00, 0x00, 0x00 };
    send(mapSock, (char*)pkt4, sizeof(pkt4), 0);

    log("*** Map server handshake complete.");
    return true;
