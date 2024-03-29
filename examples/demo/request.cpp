#include <xhttp>
#include <iostream>

int main(int argc, char const *argv[])
{
    const char* data = "POST / HTTP/1.1\r\nHost: www.example.com\r\nContent-Type: application/x-www-form-urlencoded\r\nContent-Length: 4\r\nConnection: close\r\n\r\nq=42\r\n\r\n";
    xhttp::parser parser;
    
    int state = parser
        .on_msg_begin([](){
            std::cout << "Parsing started!\n";
            return xhttp::Errno::OK;
        })
        .on_method([](const char* at, size_t len){
            std::string_view method(at, len);
            std::cout << "Method: " << method << "\n";
            return xhttp::Errno::OK;
        })
        .on_url([](const char* at, size_t len){
            std::string_view url(at, len);
            std::cout << "Url: " << url << "\n";
            return xhttp::Errno::OK;
        })
        .on_version([](const char* at, size_t len){
            std::string_view version(at, len);
            std::cout << "Version: " << version << "\n";
            return xhttp::Errno::OK;
        })
        .on_header_field([](const char* at, size_t len){
            std::string_view header(at, len);
            std::cout << "Header: " << header << "\n";
            return xhttp::Errno::OK;
        })
        .on_header_value([](const char* at, size_t len){
            std::string_view value(at, len);
            std::cout << "Value: " << value << "\n";
            return xhttp::Errno::OK;
        })
        .on_headers_complete([](){
            std::cout << "Headers complete!\n";
            return xhttp::Errno::OK;
        })
        .on_body([](const char* at, size_t len){
            std::string_view body(at, len);
            std::cout << "Body: " << body << "\n";
            return xhttp::Errno::OK;
        })
        .on_msg_complete([](){
            std::cout << "Message complete!\n";
            return xhttp::Errno::OK;
        })
        .execute(data, strlen(data));

        auto err = parser.errno_name(state);
        std::cout << "Error: " << err << "\n";

    return 0;
}
