#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "xhttp.h"
#include "xhttp_internal.h"                                          

namespace xhttp {
    parser::parser(uint8_t type)
        : m_Impl(new llhttp__internal_t(type))
    {}

    parser::~parser() = default;

    uint8_t parser::type() {
        return Impl()->type;
    }

    uint8_t parser::http_major() {
        return Impl()->http_major;
    }

    uint8_t parser::http_minor() {
        return Impl()->http_minor;
    }

    uint8_t parser::method() {
        return Impl()->method;
    }

    int parser::status_code() {
        return Impl()->status_code;
    }

    uint8_t parser::upgrade() {
        return Impl()->upgrade;
    }

    parser& parser::reset() {
        uint8_t type = Impl()->type;
        parse_settings* settings = Impl()->settings;
        void* data = Impl()->data;
        uint8_t lenient_flags = Impl()->lenient_flags;

        //parser::_internal_init(parser);
        // todo - reset internal
        m_Impl.reset();

        Impl()->type = type;
        Impl()->settings = settings;
        Impl()->data = data;
        Impl()->lenient_flags = lenient_flags;

        return *this;
    }

    int parser::execute(const char* data, size_t len) {
        return Impl()->__internal_execute(data, data + len);
    }

    int parser::finish() {
        int err;

        /* We're in an error state. Don't bother doing anything. */
        if (Impl()->error != Errno::OK) {
            return Errno::OK;
        }

        switch (Impl()->finish) {
            case Finish::HTTP_FINISH_SAFE_WITH_CB:
                err = Impl()->settings->on_message_complete();  
                if (err != Errno::OK) return err;

            /* FALLTHROUGH */
            case Finish::HTTP_FINISH_SAFE:
                return Errno::OK;

            case Finish::HTTP_FINISH_UNSAFE:
                Impl()->reason = "Invalid EOF state";
                return Errno::INVALID_EOF_STATE;
            default:
                abort();
        }
    }

    int parser::message_needs_eof()
    {
        return Impl()->_message_needs_eof();
    }

    int parser::should_keep_alive()
    {
        return Impl()->_should_keep_alive();
    }

    parser& parser::pause() {
        if (Impl()->error != Errno::OK) {
            return *this;
        }

        Impl()->error = Errno::PAUSED;
        Impl()->reason = "Paused";
        return *this;
    }

    parser& parser::resume() {
        if (Impl()->error != Errno::PAUSED) {
            return *this;
        }

        Impl()->error = Errno::OK;
        return *this;
    }

    parser& parser::resume_after_upgrade() {
        if (Impl()->error != Errno::PAUSED_UPGRADE) {
           return *this;
        }

        Impl()->error = Errno::OK;
        return *this;
    }

    int parser::err() {
        return Impl()->error;
    }

    const char* parser::error_reason() {
        return Impl()->reason;
    }


    parser& parser::set_error_reason(const char* reason) {
        Impl()->reason = reason;
        return *this;
    }


    const char* parser::error_pos() {
        return Impl()->error_pos;
    }

    const char* parser::errno_name(int err) {
        #define HTTP_ERRNO_GEN(CODE, NAME, _) case Errno::##NAME: return "" #NAME;
        switch (err) {
            HTTP_ERRNO_MAP(HTTP_ERRNO_GEN)
            default: abort();
        }
        #undef HTTP_ERRNO_GEN
    }

    const char* parser::method_name(uint8_t method) {
        #define HTTP_METHOD_GEN(NUM, NAME, STRING) case Method::##NAME: return #STRING;
        switch (method) {
            HTTP_ALL_METHOD_MAP(HTTP_METHOD_GEN)
            default: abort();
        }
        #undef HTTP_METHOD_GEN
    }

    const char* parser::status_name(uint16_t status) {
        #define HTTP_STATUS_GEN(NUM, NAME, STRING) case Status::HTTP_STATUS_##NAME: return #STRING;
        switch (status) {
            HTTP_STATUS_MAP(HTTP_STATUS_GEN)
            default: abort();
        }
        #undef HTTP_STATUS_GEN
    }

    size_t parser::content_length() const
    {
        return Impl()->content_length;
    }

    parser& parser::set_lenient_headers(int enabled) {
        if (enabled) {
            Impl()->lenient_flags |= LenientFlags::LENIENT_HEADERS;
        } else {
            Impl()->lenient_flags &= ~LenientFlags::LENIENT_HEADERS;
        }
        return *this;
    }

    parser& parser::set_lenient_chunked_length(int enabled) {
        if (enabled) {
            Impl()->lenient_flags |= LenientFlags::LENIENT_CHUNKED_LENGTH;
        } else {
            Impl()->lenient_flags &= ~LenientFlags::LENIENT_CHUNKED_LENGTH;
        }
        return *this;
    }

    parser& parser::set_lenient_keep_alive(int enabled) {
        if (enabled) {
            Impl()->lenient_flags |= LenientFlags::LENIENT_KEEP_ALIVE;
        } else {
            Impl()->lenient_flags &= ~LenientFlags::LENIENT_KEEP_ALIVE;
        }
        return *this;
    }

    parser& parser::set_lenient_transfer_encoding(int enabled) {
        if (enabled) {
            Impl()->lenient_flags |= LenientFlags::LENIENT_TRANSFER_ENCODING;
        } else {
            Impl()->lenient_flags &= ~LenientFlags::LENIENT_TRANSFER_ENCODING;
        }
        return *this;
    }

    parser &parser::on_msg_begin(parser_cb cb)
    {
        Impl()->settings->on_message_begin = cb;
        return *this;
    }

    parser &parser::on_headers_complete(parser_cb cb)
    {
        Impl()->settings->on_headers_complete = cb;
        return *this;
    }

    parser &parser::on_msg_complete(parser_cb cb)
    {
        Impl()->settings->on_message_complete = cb;
        return *this;
    }

    parser& parser::on_method(raw_data_cb cb) {
        Impl()->settings->on_method = cb;
        return *this;
    }
    
    parser& parser::on_url(raw_data_cb cb) {
        Impl()->settings->on_url = cb;
        return *this;
    }

    parser& parser::on_status(raw_data_cb cb) {
        Impl()->settings->on_status = cb;
        return *this;
    }

    parser& parser::on_version(raw_data_cb cb) {
        Impl()->settings->on_version = cb;
        return *this;
    }

    parser &parser::on_body(raw_data_cb cb)
    {
        Impl()->settings->on_body = cb;
        return *this;
    }

    parser &parser::on_header_field(raw_data_cb cb)
    {
        Impl()->settings->on_header_field = cb;
        return *this;
    }

    parser &parser::on_header_value(raw_data_cb cb)
    {
        Impl()->settings->on_header_value = cb;
        return *this;
    }

} // namespace xhttp



/* Private */


// void _debug(llhttp::parser* s, const char* p, const char* endp,
//                    const char* msg) {
//   if (p == endp) {
//     fprintf(stderr, "p=%p type=%d flags=%02x next=null debug=%s\n", s, s->type,
//             s->flags, msg);
//   } else {
//     fprintf(stderr, "p=%p type=%d flags=%02x next=%02x   debug=%s\n", s,
//             s->type, s->flags, *p, msg);
//   }
// }
