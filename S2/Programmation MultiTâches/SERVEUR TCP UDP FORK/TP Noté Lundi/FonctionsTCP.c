int sendTCP(int connectedSocket, char *message, size_t messageSize) {
    ssize_t res = send(connectedSocket, &messageSize, sizeof(messageSize), 0);
    if (res == ERROR || res == 0) {
        return res;
    }
    size_t totalSent = 0;
    while (totalSent < messageSize) {
        ssize_t res = send(connectedSocket, message + totalSent, messageSize - totalSent, 0);
        if (res == ERROR || res == 0) {
            return res;
        }
        totalSent += res;
    }
    return totalSent;
}

int receiveTCP(int connectedSocket, char *message, size_t maxMessageSize) {
    size_t messageSize;
    ssize_t res = recv(connectedSocket, &messageSize, sizeof(messageSize), 0);
    if (res == ERROR || res == 0) {
        return res;
    }
    size_t totalReceived = 0;
    while (totalReceived < messageSize) {
        ssize_t res = recv(connectedSocket, message + totalReceived, messageSize - totalReceived, 0);
        if (res == ERROR || res == 0) {
            return res;
        }
        totalReceived += res;
    }
    return totalReceived;
}
