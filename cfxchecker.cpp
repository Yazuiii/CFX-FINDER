#include <iostream>
#include <string>
#include <windows.h>
#include <winhttp.h>

void setDarkPurple() {
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_INTENSITY);
}


void getServerIP(const std::string& cfxLink) {
    setDarkPurple();
    
    std::string request;
    std::string displayUrl;
    
    if (cfxLink.find("cfx.re/join/") == 0) {
        request = "https://" + cfxLink;
        displayUrl = cfxLink;
    } else if (cfxLink.find("https://cfx.re/join/") == 0) {
        request = cfxLink;
        displayUrl = cfxLink.substr(8);
    } else {
        request = "https://cfx.re/join/" + cfxLink;
        displayUrl = "cfx.re/join/" + cfxLink;
    }
    
    HINTERNET hSession = WinHttpOpen(L"FiveM/1.0",
                                   WINHTTP_ACCESS_TYPE_DEFAULT_PROXY,
                                   WINHTTP_NO_PROXY_NAME,
                                   WINHTTP_NO_PROXY_BYPASS,
                                   0);

    if (!hSession) {
        setDarkPurple();
        std::cout << "\nServer wurde nicht gefunden! Überprüfe die CFX.re IP-Adresse oder der Server ist möglicherweise offline!\n" << std::endl;
        return;
    }

    std::wstring wideUrl(request.begin(), request.end());
    WCHAR hostname[256] = L"cfx.re";
    WCHAR urlPath[256];
    URL_COMPONENTS urlComp = { sizeof(URL_COMPONENTS) };
    urlComp.lpszHostName = hostname;
    urlComp.dwHostNameLength = sizeof(hostname) / sizeof(WCHAR);
    urlComp.lpszUrlPath = urlPath;
    urlComp.dwUrlPathLength = sizeof(urlPath) / sizeof(WCHAR);
    WinHttpCrackUrl(wideUrl.c_str(), 0, 0, &urlComp);

    HINTERNET hConnect = WinHttpConnect(hSession, hostname, INTERNET_DEFAULT_HTTPS_PORT, 0);
    if (!hConnect) {
        WinHttpCloseHandle(hSession);
        setDarkPurple();
        std::cout << "\nServer wurde nicht gefunden! Überprüfe die CFX.re IP-Adresse oder der Server ist möglicherweise offline!\n" << std::endl;
        return;
    }

    HINTERNET hRequest = WinHttpOpenRequest(hConnect, L"GET", urlPath,
                                          NULL, WINHTTP_NO_REFERER,
                                          WINHTTP_DEFAULT_ACCEPT_TYPES,
                                          WINHTTP_FLAG_SECURE);

    if (!hRequest) {
        WinHttpCloseHandle(hConnect);
        WinHttpCloseHandle(hSession);
        setDarkPurple();
        std::cout << "\nServer wurde nicht gefunden! Überprüfe die CFX.re IP-Adresse oder der Server ist möglicherweise offline!\n" << std::endl;
        return;
    }

    if (!WinHttpSendRequest(hRequest, WINHTTP_NO_ADDITIONAL_HEADERS, 0,
                           WINHTTP_NO_REQUEST_DATA, 0, 0, 0) ||
        !WinHttpReceiveResponse(hRequest, NULL)) {
        WinHttpCloseHandle(hRequest);
        WinHttpCloseHandle(hConnect);
        WinHttpCloseHandle(hSession);
        setDarkPurple();
        std::cout << "\nServer wurde nicht gefunden! Überprüfe die CFX.re IP-Adresse oder der Server ist möglicherweise offline!\n" << std::endl;
        return;
    }

    WCHAR headerBuffer[512];
    DWORD bufferLength = sizeof(headerBuffer);
    if (WinHttpQueryHeaders(hRequest,
                           WINHTTP_QUERY_CUSTOM,
                           L"X-CitizenFX-Url",
                           headerBuffer,
                           &bufferLength,
                           WINHTTP_NO_HEADER_INDEX)) {
        
        std::wstring response(headerBuffer);
        std::string serverIP(response.begin(), response.end());
        
        serverIP = serverIP.substr(7); 
        if (serverIP.back() == '/') {
            serverIP.pop_back();
        }

        setDarkPurple();
        std::cout << "\nCFX.re IP: " << displayUrl << std::endl;
        std::cout << "IP address: " << serverIP << std::endl << std::endl;
        std::cout << "Hinweis: Denken Sie daran, dass einige Server ihre IP verbergen, aber es gibt einen anderen Weg, sie zu erhalten.\n";
        std::cout << "Stellen Sie auch sicher, dass der Server online ist!\n" << std::endl;
    } else {
        setDarkPurple();
        std::cout << "\nServer wurde nicht gefunden! Überprüfe die CFX.re IP-Adresse oder der Server ist möglicherweise offline!\n" << std::endl;
    }

    WinHttpCloseHandle(hRequest);
    WinHttpCloseHandle(hConnect);
    WinHttpCloseHandle(hSession);
}

int main() {
    setDarkPurple();
    
    std::cout << "   _____ ________   __              _____ _____     ______ _           _           \n";
    std::cout << "  / ____|  ____\\ \\ / /             |_   _|  __ \\   |  ____(_)         | |          \n";
    std::cout << " | |    | |__   \\ V /   _ __ ___     | | | |__) |  | |__   _ _ __   __| | ___ _ __ \n";
    std::cout << " | |    |  __|   > <   | '__/ _ \\    | | |  ___/   |  __| | | '_ \\ / _` |/ _ \\ '__|\n";
    std::cout << " | |____| |     / . \\ _| | |  __/   _| |_| |       | |    | | | | | (_| |  __/ |   \n";
    std::cout << "  \\_____|_|    /_/ \\_(_)_|  \\___|  |_____|_|       |_|    |_|_| |_|\\__,_|\\___|_|   \n";
    std::cout << "\n";
    std::cout << "Made by .mascn             \n\n";

    while (true) {
        std::cout << "Bitte gib die CFX.re-Server-URL ein (cfx.re/join/XXXXXX oder XXXXXX): ";
        std::string url;
        std::cin >> url;
        
        getServerIP(url);
    }

    return 0;
}
