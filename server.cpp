#include "server.h"

std::string readFile(const std::string& path) {
    std::ifstream f(path, std::ios::binary);
    std::stringstream ss;
    ss << f.rdbuf();
    return ss.str();
}

void Server::start(){

    int server = socket(AF_INET, SOCK_STREAM, 0);
    
    int yes = 1;

    setsockopt(server, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes));

    sockaddr_in addr{};

    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
    addr.sin_port = htons(8080);

    if(bind(server, (sockaddr*)&addr, sizeof(addr)) > 0){
        std::cout << "failed to bind server" << std::endl;
    }

    listen(server, 16);

    std::cout << "listening on http://localhost:8080\n";

    while(true){
        int client = accept(server, nullptr, nullptr);
        if(client < 0){
            continue;
        }

        char buf[4096];
        ssize_t n = read(client, buf, sizeof(buf) - 1);
        
        if(n > 0){
            buf[n] = '\0';
            std::string req(buf, n);
            int start = req.find('/');
            int end = req.find(' ', start);

            std::string requestedData = req.substr(start, end - start);

            std::cout << "request: " << req << std::endl;
            std::cout << "type: " << requestedData << std::endl;
            
            std::string response;

            if(requestedData == "/"){

            std::string body = readFile("./front/index.html");
            
            response =
                "HTTP/1.1 200 OK\r\n"
                "Content-Type: text/html\r\n"
                "Content-Length: " + std::to_string(body.size()) + "\r\n"
                "Connection: close\r\n"
                "\r\n" + body;

            } else if (requestedData == "/style.css"){
                std::string body = readFile("./front/style.css");
                response =
                    "HTTP/1.1 200 OK\r\n"
                    "Content-Type: text/css\r\n"
                    "Content-Length: " + std::to_string(body.size()) + "\r\n"
                    "Connection: close\r\n"
                    "\r\n" + body;                
            } else if (requestedData == "/app.js"){
                std::string body = readFile("./front/dist/app.js");
                response =
                    "HTTP/1.1 200 OK\r\n"
                    "Content-Type: text/javascript\r\n"
                    "Content-Length: " + std::to_string(body.size()) + "\r\n"
                    "Connection: close\r\n"
                    "\r\n" + body;                
            }

            write(client, response.data(), response.size());
        }
        close(client);
    }
}
