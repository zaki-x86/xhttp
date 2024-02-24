#include <xhttp>
#include <iostream>

int main() {
    xhttp::parser client_parser(xhttp::Type::RESPONSE);
    
    const char* data = "HTTP/1.1 301 Moved Permanently\r\nLocation: http://www.google.com/\r\nContent-Type: text/html; charset=UTF-8\r\nContent-Security-Policy-Report-Only: object-src 'none';base-uri 'self';script-src 'nonce-elPYGZ6nFeqSYbouLhM5fw' 'strict-dynamic' 'report-sample' 'unsafe-eval' 'unsafe-inline' https: http:;report-uri https://csp.withgoogle.com/csp/gws/other-hp\r\nDate: Sat, 24 Feb 2024 10:42:00 GMT\r\nExpires: Mon, 25 Mar 2024 10:42:00 GMT\r\nCache-Control: public, max-age=2592000\r\nServer: gws\r\nContent-Length: 219\r\nX-XSS-Protection: 0\r\nX-Frame-Options: SAMEORIGIN\r\n\r\n<HTML>\n<HEAD>\n<meta http-equiv=\"content-type\" content=\"text/html;charset=utf-8\">\n<TITLE>301 Moved</TITLE>\n</HEAD>\n<BODY>\n<H1>301 Moved</H1>\nThe document has moved<A HREF=\"http://www.google.com/\">here</A>.\n</BODY>\n</HTML>\r\n\r\n";

    client_parser
        .on_version([](const char* at, size_t len){
            std::string_view version(at, len);
            std::cout << "Version: " << version << std::endl;
            return 0;
        })
        .on_status([&client_parser](const char* at, size_t len){
            std::string_view status(at, len);
            std::cout << "Status code: " << client_parser.status_code() << std::endl;
            std::cout << "Status: " << status << std::endl;
            return 0;
        })
        .on_header_field([](const char* at, size_t len){
            std::string_view header(at, len);
            std::cout << "Header: " << header << std::endl;
            return 0;
        })
        .on_header_value([](const char* at, size_t len){
            std::string_view value(at, len);
            std::cout << "Value: " << value << std::endl;
            return 0;
        })
        .on_headers_complete([&client_parser](){
            std::cout << "Headers Complete" << std::endl;
            std::cout << "Content-Length: " << client_parser.content_length() << std::endl;
            // based on the content length, you may choose to parse the body or not
            // by returning 0, you will parse the body, and by returning 1, you will skip parsing the body
            return 0;
        })
        .on_body([](const char* at, size_t len){
            std::string_view body(at, len);
            std::cout << "Body: " << body << std::endl;
            return 0;
        })
        .on_msg_complete([](){
            std::cout << "Message Complete" << std::endl;
            return 0;
        })
        .execute(data, strlen(data));
}
