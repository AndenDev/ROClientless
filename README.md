# Ragnarok Online Clientless Bot

A lightweight, clientless bot for **Ragnarok Online** written in C++.  
This project demonstrates how to connect to a RO server using raw packets:  
**Login Server → Character Server → Map Server**, without using the official game client.

---

## 🚀 Features

- Connects to RO login server and authenticates using username/password
- Retrieves and selects a character from the character server
- Connects to the map server and sends initial handshake packets
- Designed to be expandable into a full clientless bot
- Structured with modular code for login, character, and map logic

---

## 🧩 Architecture

```
main.cpp
├── network/
│   ├── login.h / login.cpp  → Handles login server connection
│   ├── char.h  / char.cpp   → Handles character selection
│   └── map.h   / map.cpp    → Handles map server handshake
├── shared/
│   ├── packets.h → Packet structs and constants
│   └── logger.h  → Simple logging utility
├── config.h       → Configuration: IP, port, credentials
```

---

## ⚙️ Configuration

Edit the `config.h` file to match your RO server:

```cpp
#define RO_SERVER_IP     "127.0.0.1"
#define RO_LOGIN_PORT    6900
#define RO_USERNAME      "your_username"
#define RO_PASSWORD      "your_password"
#define RO_SELECTED_CHAR_INDEX 0
#define RO_PACKET_VER    20220404 // Use the version your server expects
```

---

## 🛠️ Build Instructions (Windows)

1. **Requirements**
   - Visual Studio or any C++ compiler supporting C++11+
   - Winsock2 (usually included with Windows SDK)

2. **Build Steps**
   - Open project folder in Visual Studio
   - Include all `.cpp` and `.h` files in your project
   - Build and run `main.cpp`

3. **CLI (Optional)**

If you're using a CLI or Makefile setup:

```bash
cl /EHsc /Fe:ro_bot.exe main.cpp network\login.cpp network\char.cpp network\map.cpp /I. /Ishared /link ws2_32.lib
```

---

## 🖥️ Example Output

```
--------------------------------------------
>> Ragnarok Online Clientless Bot Started
>> Flow: Login -> Character -> Map
--------------------------------------------
[✓] Winsock initialized.
[>] Connecting to Login Server...
[✓] Login successful.
[>] Connecting to Character Server...
[✓] Character selected and map server info received.
[>] Connecting to Map Server...
[✓] Entered map server successfully.
>>> Bot initialized. Standing by...
```

---

## 📌 Notes

- This is a **learning tool** and not meant for abuse.
- It assumes the RO server uses the standard packet flow (0x0064, 0x0065, 0x0066, 0x0072, etc.).
- You can expand this bot by parsing map packets, moving the character, attacking, etc.

---

## 🔐 Legal

This project is for educational and reverse engineering research purposes.  
It is **not affiliated with Gravity, WarpPortal, or any Ragnarok Online publisher**.  
Use responsibly and respect private servers' rules.

---

## 🧠 TODO / Ideas

- [ ] Packet parser & sniffer integration
- [ ] Movement and command system
- [ ] Auto-login/character reconnect



---

## 🙏 Credits

Inspired by OpenKore’s architecture and packet analysis from the RO community.
