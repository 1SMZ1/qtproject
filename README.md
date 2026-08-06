# Qt 聊天应用

基于 Qt 5 开发的局域网聊天应用，采用客户端-服务器架构，支持用户注册登录、好友管理、实时聊天等功能。

## 功能特性

- 用户注册与登录
- 好友搜索与添加（需对方同意）
- 好友列表刷新与管理
- 实时文字聊天
- 删除好友
- 自定义 TCP 通信协议（PDU）

## 项目结构

```
2507/
├── Client/                 # 客户端模块（GUI 应用）
│   ├── main.cpp            # 程序入口
│   ├── loginwindow.cpp/h   # 登录窗口
│   ├── index.cpp/h         # 主界面（好友列表）
│   ├── chat.cpp/h          # 聊天窗口
│   ├── fire.cpp/h          # 好友请求处理
│   ├── friendwindow.cpp/h  # 好友窗口
│   ├── client.cpp/h        # 网络客户端（TCP 连接管理）
│   ├── protocol.cpp/h      # 通信协议定义（PDU）
│   ├── reshandler.cpp/h    # 资源处理
│   ├── config.ini          # 配置文件（服务器地址、端口）
│   ├── config.qrc          # Qt 资源文件
│   └── Client.pro          # Qt 项目文件
│
├── Server/                 # 服务端模块（控制台应用）
│   ├── main.cpp            # 程序入口
│   ├── server.cpp/h        # 服务器核心（单例模式）
│   ├── tcpserver.cpp/h     # TCP 服务器
│   ├── tcpsocket.cpp/h     # TCP 套接字处理
│   ├── msghandler.cpp/h    # 消息处理
│   ├── operatedb.cpp/h     # 数据库操作（单例模式）
│   ├── protocol.cpp/h      # 通信协议定义（PDU）
│   ├── config.ini          # 配置文件
│   ├── config.qrc          # Qt 资源文件
│   └── Server.pro          # Qt 项目文件
│
└── .gitignore
```

## 技术栈

| 技术 | 说明 |
|------|------|
| **Qt 5.14.2** | 应用框架 |
| **MinGW 64-bit** | C++ 编译器 |
| **Qt Network** | TCP 网络通信 |
| **Qt SQL** | 数据库操作（服务端） |
| **Qt Widgets** | GUI 界面（客户端） |
| **C++11** | 语言标准 |

## 通信协议

客户端与服务端通过自定义的 PDU（Protocol Data Unit）进行通信：

```c
struct PDU {
    unsigned int uintTotalLen;  // 协议总长度
    unsigned int uintMsgLen;    // 柔性数组长度
    unsigned int uintType;      // 协议类型
    char caData[64];            // 固定长度数据
    char caMsg[];               // 柔性消息体
};
```

支持的消息类型包括：登录、注册、查找用户、添加好友、同意好友、刷新好友列表、删除好友、聊天等。

## 编译与运行

### 环境要求

- Qt 5.14.2（或兼容版本）
- MinGW 或 MSVC 编译器
- Qt Creator（推荐）

### 编译服务端

1. 使用 Qt Creator 打开 `Server/Server.pro`
2. 选择编译套件（如 Desktop Qt 5.14.2 MinGW 64-bit）
3. 点击 **构建** → **执行 qmake**，然后 **构建**

或使用命令行：

```bash
cd Server
qmake Server.pro
make        # Linux/macOS
mingw32-make  # Windows MinGW
```

### 编译客户端

1. 使用 Qt Creator 打开 `Client/Client.pro`
2. 选择编译套件
3. 点击 **构建** → **执行 qmake**，然后 **构建**

或使用命令行：

```bash
cd Client
qmake Client.pro
make        # Linux/macOS
mingw32-make  # Windows MinGW
```

### 运行

1. **先启动服务端**：运行 Server 可执行文件，服务端会自动连接数据库并监听端口（默认 `4570`，可在 `config.ini` 中修改）
2. **再启动客户端**：运行 Client 可执行文件，在登录窗口中注册/登录账号

> 注意：`config.ini` 中配置了服务器地址和端口，确保客户端配置指向正确的服务器地址。

## 许可证

本项目仅供学习参考使用。
