
#include <iostream>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <iomanip>
#include <chrono>
#include <ctime>
#include "config.h"
#include "shared/logger.h"
#include "shared/packets.h"
#include "network/login.h"
#include "network/char.h"
#include "network/map.h"

#pragma comment(lib, "ws2_32.lib")

int main() {
    log("--------------------------------------------");
    log(">> Ragnarok Online Clientless Bot Started");
    log(">> Flow: Login -> Character -> Map");
    log("--------------------------------------------");

    // Initialize Winsock
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        log("Failed to initialize Winsock.");
        return 1;
    }
    log("Winsock initialized.\n");

    // Step 1: Connect to Login Server
    LoginSession loginSession;
    log("Connecting to Login Server...");
    if (!connect_login_server(loginSession)) {
        log("Login server connection failed.");
        WSACleanup();
        return 1;
    }
    log("Login successful.\n");

    // Step 2: Connect to Character Server
    CharSession charSession;
    log("Connecting to Character Server...");
    if (!connect_char_server(loginSession, charSession)) {
        log("Character server connection failed.");
        WSACleanup();
        return 1;
    }
    log("Character selected and map server info received.\n");

    // Step 3: Connect to Map Server
    log("Connecting to Map Server...");
    if (!connect_map_server(loginSession, charSession)) {
        log("Map server connection failed.");
        WSACleanup();
        return 1;
    }
    log("Entered map server successfully.\n");

    // Bot is now active
    log(">>> Bot initialized. Standing by...");
    log(">>> Press Enter to exit.\n");

    getchar();

    // Cleanup
    WSACleanup();
    log("Winsock cleaned up. Exiting.");

    return 0;
}
