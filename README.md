# `xhttp`

Reimplementation of [nodejs/llhttp](https://github.com/nodejs/llhttp) with Clean C++ API

> Project is under active development - but it is usable

## Usage

### Parsing an incoming http request message

```cpp
#include <xhttp>
#include <iostream>

int main(int argc, char const *argv[])
{
    const char* data = "POST / HTTP/1.1\r\nHost: www.example.com\r\nContent-Type: application/x-www-form-urlencoded\r\nContent-Length: 4\r\nConnection: close\r\n\r\nq=42\r\n\r\n";
    xhttp::parser request_parser(xhttp::Type::REQUEST);
    
    request_parser
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

    return 0;
}

```

Output:

```
Parsing started!
Method: POST
Url: /
Version: 1.1
Header: Host
Value: www.example.com
Header: Content-Type
Value: application/x-www-form-urlencoded
Header: Content-Length
Value: 4
Header: Connection
Value: close
Headers complete!
Body: q=42
Message complete!
Error: OK
```

### Parsing an incoming http response message

```cpp
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
```

Output:

```
Version: 1.1
Status code: 301
Status: Moved Permanently
Header: Location
Value: http://www.google.com/
Header: Content-Type
Value: text/html; charset=UTF-8
Header: Content-Security-Policy-Report-Only
Value: object-src 'none';base-uri 'self';script-src 'nonce-elPYGZ6nFeqSYbouLhM5fw' 'strict-dynamic' 'report-sample' 'unsafe-eval' 'unsafe-inline' https: http:;report-uri https://csp.withgoogle.com/csp/gws/other-hp       
Header: Date
Value: Sat, 24 Feb 2024 10:42:00 GMT
Header: Expires
Value: Mon, 25 Mar 2024 10:42:00 GMT
Header: Cache-Control
Value: public, max-age=2592000
Header: Server
Value: gws
Header: Content-Length
Value: 219
Header: X-XSS-Protection
Value: 0
Header: X-Frame-Options
Value: SAMEORIGIN
Headers Complete
Content-Length: 219
Body: <HTML>
<HEAD>
<meta http-equiv="content-type" content="text/html;charset=utf-8">
<TITLE>301 Moved</TITLE>
</HEAD>
<BODY>
<H1>301 Moved</H1>
The document has moved<A HREF="http://www.google.com/">here</A>.
</BODY>
</HTML
Message Complete
```
