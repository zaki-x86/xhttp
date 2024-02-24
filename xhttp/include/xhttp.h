#ifndef __XHTTP_H__
#define __XHTTP_H__


#include <stdint.h>
#include <functional>
#include <string>
#include <string_view>
#include <memory>

namespace xhttp {


    namespace Errno
    {
    enum : int32_t {
        ERROR = -1,
        OK = 0,
        INTERNAL = 1,
        STRICT = 2,
        CR_EXPECTED = 25,
        LF_EXPECTED = 3,
        UNEXPECTED_CONTENT_LENGTH = 4,
        UNEXPECTED_SPACE = 30,
        CLOSED_CONNECTION = 5,
        INVALID_METHOD = 6,
        INVALID_URL = 7,
        INVALID_CONSTANT = 8,
        INVALID_VERSION = 9,
        INVALID_HEADER_TOKEN = 10,
        INVALID_CONTENT_LENGTH = 11,
        INVALID_CHUNK_SIZE = 12,
        INVALID_STATUS = 13,
        INVALID_EOF_STATE = 14,
        INVALID_TRANSFER_ENCODING = 15,
        CB_MESSAGE_BEGIN = 16,
        CB_HEADERS_COMPLETE = 17,
        CB_MESSAGE_COMPLETE = 18,
        CB_CHUNK_HEADER = 19,
        CB_CHUNK_COMPLETE = 20,
        PAUSED = 21,
        PAUSED_UPGRADE = 22,
        PAUSED_H2_UPGRADE = 23,
        USER = 24,
        CB_URL_COMPLETE = 26,
        CB_STATUS_COMPLETE = 27,
        CB_METHOD_COMPLETE = 32,
        CB_VERSION_COMPLETE = 33,
        CB_HEADER_FIELD_COMPLETE = 28,
        CB_HEADER_VALUE_COMPLETE = 29,
        CB_CHUNK_EXTENSION_NAME_COMPLETE = 34,
        CB_CHUNK_EXTENSION_VALUE_COMPLETE = 35,
        CB_RESET = 31
    };
        
    }

    namespace Flags {
    enum : uint16_t {
        F_CONNECTION_KEEP_ALIVE = 0x1,
        F_CONNECTION_CLOSE = 0x2,
        F_CONNECTION_UPGRADE = 0x4,
        F_CHUNKED = 0x8,
        F_UPGRADE = 0x10,
        F_CONTENT_LENGTH = 0x20,
        F_SKIPBODY = 0x40,
        F_TRAILING = 0x80,
        F_TRANSFER_ENCODING = 0x200
    };
    }

    namespace LenientFlags {
    enum : uint8_t {
        LENIENT_HEADERS = 0x1,
        LENIENT_CHUNKED_LENGTH = 0x2,
        LENIENT_KEEP_ALIVE = 0x4,
        LENIENT_TRANSFER_ENCODING = 0x8,
        LENIENT_VERSION = 0x10
    };
    }

    namespace Type {
    enum : uint8_t {
        BOTH = 0,
        REQUEST = 1,
        RESPONSE = 2
    };
    }

    namespace Finish {
    enum : uint8_t {
        HTTP_FINISH_SAFE = 0,
        HTTP_FINISH_SAFE_WITH_CB = 1,
        HTTP_FINISH_UNSAFE = 2
    };
    }

    namespace Method {
    enum : uint8_t {
        DELETE = 0,
        GET = 1,
        HEAD = 2,
        POST = 3,
        PUT = 4,
        CONNECT = 5,
        OPTIONS = 6,
        TRACE = 7,
        COPY = 8,
        LOCK = 9,
        MKCOL = 10,
        MOVE = 11,
        PROPFIND = 12,
        PROPPATCH = 13,
        SEARCH = 14,
        UNLOCK = 15,
        BIND = 16,
        REBIND = 17,
        UNBIND = 18,
        ACL = 19,
        REPORT = 20,
        MKACTIVITY = 21,
        CHECKOUT = 22,
        MERGE = 23,
        MSEARCH = 24,
        NOTIFY = 25,
        SUBSCRIBE = 26,
        UNSUBSCRIBE = 27,
        PATCH = 28,
        PURGE = 29,
        MKCALENDAR = 30,
        LINK = 31,
        UNLINK = 32,
        SOURCE = 33,
        PRI = 34,
        DESCRIBE = 35,
        ANNOUNCE = 36,
        SETUP = 37,
        PLAY = 38,
        PAUSE = 39,
        TEARDOWN = 40,
        GET_PARAMETER = 41,
        SET_PARAMETER = 42,
        REDIRECT = 43,
        RECORD = 44,
        FLUSH = 45
    };
    }

    namespace Status {
    enum : uint16_t {
        HTTP_STATUS_CONTINUE = 100,
        HTTP_STATUS_SWITCHING_PROTOCOLS = 101,
        HTTP_STATUS_PROCESSING = 102,
        HTTP_STATUS_EARLY_HINTS = 103,
        HTTP_STATUS_RESPONSE_IS_STALE = 110,
        HTTP_STATUS_REVALIDATION_FAILED = 111,
        HTTP_STATUS_DISCONNECTED_OPERATION = 112,
        HTTP_STATUS_HEURISTIC_EXPIRATION = 113,
        HTTP_STATUS_MISCELLANEOUS_WARNING = 199,
        HTTP_STATUS_OK = 200,
        HTTP_STATUS_CREATED = 201,
        HTTP_STATUS_ACCEPTED = 202,
        HTTP_STATUS_NON_AUTHORITATIVE_INFORMATION = 203,
        HTTP_STATUS_NO_CONTENT = 204,
        HTTP_STATUS_RESET_CONTENT = 205,
        HTTP_STATUS_PARTIAL_CONTENT = 206,
        HTTP_STATUS_MULTI_STATUS = 207,
        HTTP_STATUS_ALREADY_REPORTED = 208,
        HTTP_STATUS_TRANSFORMATION_APPLIED = 214,
        HTTP_STATUS_IM_USED = 226,
        HTTP_STATUS_MISCELLANEOUS_PERSISTENT_WARNING = 299,
        HTTP_STATUS_MULTIPLE_CHOICES = 300,
        HTTP_STATUS_MOVED_PERMANENTLY = 301,
        HTTP_STATUS_FOUND = 302,
        HTTP_STATUS_SEE_OTHER = 303,
        HTTP_STATUS_NOT_MODIFIED = 304,
        HTTP_STATUS_USE_PROXY = 305,
        HTTP_STATUS_SWITCH_PROXY = 306,
        HTTP_STATUS_TEMPORARY_REDIRECT = 307,
        HTTP_STATUS_PERMANENT_REDIRECT = 308,
        HTTP_STATUS_BAD_REQUEST = 400,
        HTTP_STATUS_UNAUTHORIZED = 401,
        HTTP_STATUS_PAYMENT_REQUIRED = 402,
        HTTP_STATUS_FORBIDDEN = 403,
        HTTP_STATUS_NOT_FOUND = 404,
        HTTP_STATUS_METHOD_NOT_ALLOWED = 405,
        HTTP_STATUS_NOT_ACCEPTABLE = 406,
        HTTP_STATUS_PROXY_AUTHENTICATION_REQUIRED = 407,
        HTTP_STATUS_REQUEST_TIMEOUT = 408,
        HTTP_STATUS_CONFLICT = 409,
        HTTP_STATUS_GONE = 410,
        HTTP_STATUS_LENGTH_REQUIRED = 411,
        HTTP_STATUS_PRECONDITION_FAILED = 412,
        HTTP_STATUS_PAYLOAD_TOO_LARGE = 413,
        HTTP_STATUS_URI_TOO_LONG = 414,
        HTTP_STATUS_UNSUPPORTED_MEDIA_TYPE = 415,
        HTTP_STATUS_RANGE_NOT_SATISFIABLE = 416,
        HTTP_STATUS_EXPECTATION_FAILED = 417,
        HTTP_STATUS_IM_A_TEAPOT = 418,
        HTTP_STATUS_PAGE_EXPIRED = 419,
        HTTP_STATUS_ENHANCE_YOUR_CALM = 420,
        HTTP_STATUS_MISDIRECTED_REQUEST = 421,
        HTTP_STATUS_UNPROCESSABLE_ENTITY = 422,
        HTTP_STATUS_LOCKED = 423,
        HTTP_STATUS_FAILED_DEPENDENCY = 424,
        HTTP_STATUS_TOO_EARLY = 425,
        HTTP_STATUS_UPGRADE_REQUIRED = 426,
        HTTP_STATUS_PRECONDITION_REQUIRED = 428,
        HTTP_STATUS_TOO_MANY_REQUESTS = 429,
        HTTP_STATUS_REQUEST_HEADER_FIELDS_TOO_LARGE_UNOFFICIAL = 430,
        HTTP_STATUS_REQUEST_HEADER_FIELDS_TOO_LARGE = 431,
        HTTP_STATUS_LOGIN_TIMEOUT = 440,
        HTTP_STATUS_NO_RESPONSE = 444,
        HTTP_STATUS_RETRY_WITH = 449,
        HTTP_STATUS_BLOCKED_BY_PARENTAL_CONTROL = 450,
        HTTP_STATUS_UNAVAILABLE_FOR_LEGAL_REASONS = 451,
        HTTP_STATUS_CLIENT_CLOSED_LOAD_BALANCED_REQUEST = 460,
        HTTP_STATUS_INVALID_X_FORWARDED_FOR = 463,
        HTTP_STATUS_REQUEST_HEADER_TOO_LARGE = 494,
        HTTP_STATUS_SSL_CERTIFICATE_ERROR = 495,
        HTTP_STATUS_SSL_CERTIFICATE_REQUIRED = 496,
        HTTP_STATUS_HTTP_REQUEST_SENT_TO_HTTPS_PORT = 497,
        HTTP_STATUS_INVALID_TOKEN = 498,
        HTTP_STATUS_CLIENT_CLOSED_REQUEST = 499,
        HTTP_STATUS_INTERNAL_SERVER_ERROR = 500,
        HTTP_STATUS_NOT_IMPLEMENTED = 501,
        HTTP_STATUS_BAD_GATEWAY = 502,
        HTTP_STATUS_SERVICE_UNAVAILABLE = 503,
        HTTP_STATUS_GATEWAY_TIMEOUT = 504,
        HTTP_STATUS_HTTP_VERSION_NOT_SUPPORTED = 505,
        HTTP_STATUS_VARIANT_ALSO_NEGOTIATES = 506,
        HTTP_STATUS_INSUFFICIENT_STORAGE = 507,
        HTTP_STATUS_LOOP_DETECTED = 508,
        HTTP_STATUS_BANDWIDTH_LIMIT_EXCEEDED = 509,
        HTTP_STATUS_NOT_EXTENDED = 510,
        HTTP_STATUS_NETWORK_AUTHENTICATION_REQUIRED = 511,
        HTTP_STATUS_WEB_SERVER_UNKNOWN_ERROR = 520,
        HTTP_STATUS_WEB_SERVER_IS_DOWN = 521,
        HTTP_STATUS_CONNECTION_TIMEOUT = 522,
        HTTP_STATUS_ORIGIN_IS_UNREACHABLE = 523,
        HTTP_STATUS_TIMEOUT_OCCURED = 524,
        HTTP_STATUS_SSL_HANDSHAKE_FAILED = 525,
        HTTP_STATUS_INVALID_SSL_CERTIFICATE = 526,
        HTTP_STATUS_RAILGUN_ERROR = 527,
        HTTP_STATUS_SITE_IS_OVERLOADED = 529,
        HTTP_STATUS_SITE_IS_FROZEN = 530,
        HTTP_STATUS_IDENTITY_PROVIDER_AUTHENTICATION_ERROR = 561,
        HTTP_STATUS_NETWORK_READ_TIMEOUT = 598,
        HTTP_STATUS_NETWORK_CONNECT_TIMEOUT = 599
    };
    }

#define HTTP_ERRNO_MAP(XX) \
  XX(0, OK, OK) \
  XX(1, INTERNAL, INTERNAL) \
  XX(2, STRICT, STRICT) \
  XX(25, CR_EXPECTED, CR_EXPECTED) \
  XX(3, LF_EXPECTED, LF_EXPECTED) \
  XX(4, UNEXPECTED_CONTENT_LENGTH, UNEXPECTED_CONTENT_LENGTH) \
  XX(30, UNEXPECTED_SPACE, UNEXPECTED_SPACE) \
  XX(5, CLOSED_CONNECTION, CLOSED_CONNECTION) \
  XX(6, INVALID_METHOD, INVALID_METHOD) \
  XX(7, INVALID_URL, INVALID_URL) \
  XX(8, INVALID_CONSTANT, INVALID_CONSTANT) \
  XX(9, INVALID_VERSION, INVALID_VERSION) \
  XX(10, INVALID_HEADER_TOKEN, INVALID_HEADER_TOKEN) \
  XX(11, INVALID_CONTENT_LENGTH, INVALID_CONTENT_LENGTH) \
  XX(12, INVALID_CHUNK_SIZE, INVALID_CHUNK_SIZE) \
  XX(13, INVALID_STATUS, INVALID_STATUS) \
  XX(14, INVALID_EOF_STATE, INVALID_EOF_STATE) \
  XX(15, INVALID_TRANSFER_ENCODING, INVALID_TRANSFER_ENCODING) \
  XX(16, CB_MESSAGE_BEGIN, CB_MESSAGE_BEGIN) \
  XX(17, CB_HEADERS_COMPLETE, CB_HEADERS_COMPLETE) \
  XX(18, CB_MESSAGE_COMPLETE, CB_MESSAGE_COMPLETE) \
  XX(19, CB_CHUNK_HEADER, CB_CHUNK_HEADER) \
  XX(20, CB_CHUNK_COMPLETE, CB_CHUNK_COMPLETE) \
  XX(21, PAUSED, PAUSED) \
  XX(22, PAUSED_UPGRADE, PAUSED_UPGRADE) \
  XX(23, PAUSED_H2_UPGRADE, PAUSED_H2_UPGRADE) \
  XX(24, USER, USER) \
  XX(26, CB_URL_COMPLETE, CB_URL_COMPLETE) \
  XX(27, CB_STATUS_COMPLETE, CB_STATUS_COMPLETE) \
  XX(32, CB_METHOD_COMPLETE, CB_METHOD_COMPLETE) \
  XX(33, CB_VERSION_COMPLETE, CB_VERSION_COMPLETE) \
  XX(28, CB_HEADER_FIELD_COMPLETE, CB_HEADER_FIELD_COMPLETE) \
  XX(29, CB_HEADER_VALUE_COMPLETE, CB_HEADER_VALUE_COMPLETE) \
  XX(34, CB_CHUNK_EXTENSION_NAME_COMPLETE, CB_CHUNK_EXTENSION_NAME_COMPLETE) \
  XX(35, CB_CHUNK_EXTENSION_VALUE_COMPLETE, CB_CHUNK_EXTENSION_VALUE_COMPLETE) \
  XX(31, CB_RESET, CB_RESET) \


#define HTTP_METHOD_MAP(XX) \
  XX(0, DELETE, DELETE) \
  XX(1, GET, GET) \
  XX(2, HEAD, HEAD) \
  XX(3, POST, POST) \
  XX(4, PUT, PUT) \
  XX(5, CONNECT, CONNECT) \
  XX(6, OPTIONS, OPTIONS) \
  XX(7, TRACE, TRACE) \
  XX(8, COPY, COPY) \
  XX(9, LOCK, LOCK) \
  XX(10, MKCOL, MKCOL) \
  XX(11, MOVE, MOVE) \
  XX(12, PROPFIND, PROPFIND) \
  XX(13, PROPPATCH, PROPPATCH) \
  XX(14, SEARCH, SEARCH) \
  XX(15, UNLOCK, UNLOCK) \
  XX(16, BIND, BIND) \
  XX(17, REBIND, REBIND) \
  XX(18, UNBIND, UNBIND) \
  XX(19, ACL, ACL) \
  XX(20, REPORT, REPORT) \
  XX(21, MKACTIVITY, MKACTIVITY) \
  XX(22, CHECKOUT, CHECKOUT) \
  XX(23, MERGE, MERGE) \
  XX(24, MSEARCH, M-SEARCH) \
  XX(25, NOTIFY, NOTIFY) \
  XX(26, SUBSCRIBE, SUBSCRIBE) \
  XX(27, UNSUBSCRIBE, UNSUBSCRIBE) \
  XX(28, PATCH, PATCH) \
  XX(29, PURGE, PURGE) \
  XX(30, MKCALENDAR, MKCALENDAR) \
  XX(31, LINK, LINK) \
  XX(32, UNLINK, UNLINK) \
  XX(33, SOURCE, SOURCE) \


#define RTSP_METHOD_MAP(XX) \
  XX(1, GET, GET) \
  XX(3, POST, POST) \
  XX(6, OPTIONS, OPTIONS) \
  XX(35, DESCRIBE, DESCRIBE) \
  XX(36, ANNOUNCE, ANNOUNCE) \
  XX(37, SETUP, SETUP) \
  XX(38, PLAY, PLAY) \
  XX(39, PAUSE, PAUSE) \
  XX(40, TEARDOWN, TEARDOWN) \
  XX(41, GET_PARAMETER, GET_PARAMETER) \
  XX(42, SET_PARAMETER, SET_PARAMETER) \
  XX(43, REDIRECT, REDIRECT) \
  XX(44, RECORD, RECORD) \
  XX(45, FLUSH, FLUSH) \


#define HTTP_ALL_METHOD_MAP(XX) \
  XX(0, DELETE, DELETE) \
  XX(1, GET, GET) \
  XX(2, HEAD, HEAD) \
  XX(3, POST, POST) \
  XX(4, PUT, PUT) \
  XX(5, CONNECT, CONNECT) \
  XX(6, OPTIONS, OPTIONS) \
  XX(7, TRACE, TRACE) \
  XX(8, COPY, COPY) \
  XX(9, LOCK, LOCK) \
  XX(10, MKCOL, MKCOL) \
  XX(11, MOVE, MOVE) \
  XX(12, PROPFIND, PROPFIND) \
  XX(13, PROPPATCH, PROPPATCH) \
  XX(14, SEARCH, SEARCH) \
  XX(15, UNLOCK, UNLOCK) \
  XX(16, BIND, BIND) \
  XX(17, REBIND, REBIND) \
  XX(18, UNBIND, UNBIND) \
  XX(19, ACL, ACL) \
  XX(20, REPORT, REPORT) \
  XX(21, MKACTIVITY, MKACTIVITY) \
  XX(22, CHECKOUT, CHECKOUT) \
  XX(23, MERGE, MERGE) \
  XX(24, MSEARCH, M-SEARCH) \
  XX(25, NOTIFY, NOTIFY) \
  XX(26, SUBSCRIBE, SUBSCRIBE) \
  XX(27, UNSUBSCRIBE, UNSUBSCRIBE) \
  XX(28, PATCH, PATCH) \
  XX(29, PURGE, PURGE) \
  XX(30, MKCALENDAR, MKCALENDAR) \
  XX(31, LINK, LINK) \
  XX(32, UNLINK, UNLINK) \
  XX(33, SOURCE, SOURCE) \
  XX(34, PRI, PRI) \
  XX(35, DESCRIBE, DESCRIBE) \
  XX(36, ANNOUNCE, ANNOUNCE) \
  XX(37, SETUP, SETUP) \
  XX(38, PLAY, PLAY) \
  XX(39, PAUSE, PAUSE) \
  XX(40, TEARDOWN, TEARDOWN) \
  XX(41, GET_PARAMETER, GET_PARAMETER) \
  XX(42, SET_PARAMETER, SET_PARAMETER) \
  XX(43, REDIRECT, REDIRECT) \
  XX(44, RECORD, RECORD) \
  XX(45, FLUSH, FLUSH) \


#define HTTP_STATUS_MAP(XX) \
  XX(100, CONTINUE, CONTINUE) \
  XX(101, SWITCHING_PROTOCOLS, SWITCHING_PROTOCOLS) \
  XX(102, PROCESSING, PROCESSING) \
  XX(103, EARLY_HINTS, EARLY_HINTS) \
  XX(110, RESPONSE_IS_STALE, RESPONSE_IS_STALE) \
  XX(111, REVALIDATION_FAILED, REVALIDATION_FAILED) \
  XX(112, DISCONNECTED_OPERATION, DISCONNECTED_OPERATION) \
  XX(113, HEURISTIC_EXPIRATION, HEURISTIC_EXPIRATION) \
  XX(199, MISCELLANEOUS_WARNING, MISCELLANEOUS_WARNING) \
  XX(200, OK, OK) \
  XX(201, CREATED, CREATED) \
  XX(202, ACCEPTED, ACCEPTED) \
  XX(203, NON_AUTHORITATIVE_INFORMATION, NON_AUTHORITATIVE_INFORMATION) \
  XX(204, NO_CONTENT, NO_CONTENT) \
  XX(205, RESET_CONTENT, RESET_CONTENT) \
  XX(206, PARTIAL_CONTENT, PARTIAL_CONTENT) \
  XX(207, MULTI_STATUS, MULTI_STATUS) \
  XX(208, ALREADY_REPORTED, ALREADY_REPORTED) \
  XX(214, TRANSFORMATION_APPLIED, TRANSFORMATION_APPLIED) \
  XX(226, IM_USED, IM_USED) \
  XX(299, MISCELLANEOUS_PERSISTENT_WARNING, MISCELLANEOUS_PERSISTENT_WARNING) \
  XX(300, MULTIPLE_CHOICES, MULTIPLE_CHOICES) \
  XX(301, MOVED_PERMANENTLY, MOVED_PERMANENTLY) \
  XX(302, FOUND, FOUND) \
  XX(303, SEE_OTHER, SEE_OTHER) \
  XX(304, NOT_MODIFIED, NOT_MODIFIED) \
  XX(305, USE_PROXY, USE_PROXY) \
  XX(306, SWITCH_PROXY, SWITCH_PROXY) \
  XX(307, TEMPORARY_REDIRECT, TEMPORARY_REDIRECT) \
  XX(308, PERMANENT_REDIRECT, PERMANENT_REDIRECT) \
  XX(400, BAD_REQUEST, BAD_REQUEST) \
  XX(401, UNAUTHORIZED, UNAUTHORIZED) \
  XX(402, PAYMENT_REQUIRED, PAYMENT_REQUIRED) \
  XX(403, FORBIDDEN, FORBIDDEN) \
  XX(404, NOT_FOUND, NOT_FOUND) \
  XX(405, METHOD_NOT_ALLOWED, METHOD_NOT_ALLOWED) \
  XX(406, NOT_ACCEPTABLE, NOT_ACCEPTABLE) \
  XX(407, PROXY_AUTHENTICATION_REQUIRED, PROXY_AUTHENTICATION_REQUIRED) \
  XX(408, REQUEST_TIMEOUT, REQUEST_TIMEOUT) \
  XX(409, CONFLICT, CONFLICT) \
  XX(410, GONE, GONE) \
  XX(411, LENGTH_REQUIRED, LENGTH_REQUIRED) \
  XX(412, PRECONDITION_FAILED, PRECONDITION_FAILED) \
  XX(413, PAYLOAD_TOO_LARGE, PAYLOAD_TOO_LARGE) \
  XX(414, URI_TOO_LONG, URI_TOO_LONG) \
  XX(415, UNSUPPORTED_MEDIA_TYPE, UNSUPPORTED_MEDIA_TYPE) \
  XX(416, RANGE_NOT_SATISFIABLE, RANGE_NOT_SATISFIABLE) \
  XX(417, EXPECTATION_FAILED, EXPECTATION_FAILED) \
  XX(418, IM_A_TEAPOT, IM_A_TEAPOT) \
  XX(419, PAGE_EXPIRED, PAGE_EXPIRED) \
  XX(420, ENHANCE_YOUR_CALM, ENHANCE_YOUR_CALM) \
  XX(421, MISDIRECTED_REQUEST, MISDIRECTED_REQUEST) \
  XX(422, UNPROCESSABLE_ENTITY, UNPROCESSABLE_ENTITY) \
  XX(423, LOCKED, LOCKED) \
  XX(424, FAILED_DEPENDENCY, FAILED_DEPENDENCY) \
  XX(425, TOO_EARLY, TOO_EARLY) \
  XX(426, UPGRADE_REQUIRED, UPGRADE_REQUIRED) \
  XX(428, PRECONDITION_REQUIRED, PRECONDITION_REQUIRED) \
  XX(429, TOO_MANY_REQUESTS, TOO_MANY_REQUESTS) \
  XX(430, REQUEST_HEADER_FIELDS_TOO_LARGE_UNOFFICIAL, REQUEST_HEADER_FIELDS_TOO_LARGE_UNOFFICIAL) \
  XX(431, REQUEST_HEADER_FIELDS_TOO_LARGE, REQUEST_HEADER_FIELDS_TOO_LARGE) \
  XX(440, LOGIN_TIMEOUT, LOGIN_TIMEOUT) \
  XX(444, NO_RESPONSE, NO_RESPONSE) \
  XX(449, RETRY_WITH, RETRY_WITH) \
  XX(450, BLOCKED_BY_PARENTAL_CONTROL, BLOCKED_BY_PARENTAL_CONTROL) \
  XX(451, UNAVAILABLE_FOR_LEGAL_REASONS, UNAVAILABLE_FOR_LEGAL_REASONS) \
  XX(460, CLIENT_CLOSED_LOAD_BALANCED_REQUEST, CLIENT_CLOSED_LOAD_BALANCED_REQUEST) \
  XX(463, INVALID_X_FORWARDED_FOR, INVALID_X_FORWARDED_FOR) \
  XX(494, REQUEST_HEADER_TOO_LARGE, REQUEST_HEADER_TOO_LARGE) \
  XX(495, SSL_CERTIFICATE_ERROR, SSL_CERTIFICATE_ERROR) \
  XX(496, SSL_CERTIFICATE_REQUIRED, SSL_CERTIFICATE_REQUIRED) \
  XX(497, HTTP_REQUEST_SENT_TO_HTTPS_PORT, HTTP_REQUEST_SENT_TO_HTTPS_PORT) \
  XX(498, INVALID_TOKEN, INVALID_TOKEN) \
  XX(499, CLIENT_CLOSED_REQUEST, CLIENT_CLOSED_REQUEST) \
  XX(500, INTERNAL_SERVER_ERROR, INTERNAL_SERVER_ERROR) \
  XX(501, NOT_IMPLEMENTED, NOT_IMPLEMENTED) \
  XX(502, BAD_GATEWAY, BAD_GATEWAY) \
  XX(503, SERVICE_UNAVAILABLE, SERVICE_UNAVAILABLE) \
  XX(504, GATEWAY_TIMEOUT, GATEWAY_TIMEOUT) \
  XX(505, HTTP_VERSION_NOT_SUPPORTED, HTTP_VERSION_NOT_SUPPORTED) \
  XX(506, VARIANT_ALSO_NEGOTIATES, VARIANT_ALSO_NEGOTIATES) \
  XX(507, INSUFFICIENT_STORAGE, INSUFFICIENT_STORAGE) \
  XX(508, LOOP_DETECTED, LOOP_DETECTED) \
  XX(509, BANDWIDTH_LIMIT_EXCEEDED, BANDWIDTH_LIMIT_EXCEEDED) \
  XX(510, NOT_EXTENDED, NOT_EXTENDED) \
  XX(511, NETWORK_AUTHENTICATION_REQUIRED, NETWORK_AUTHENTICATION_REQUIRED) \
  XX(520, WEB_SERVER_UNKNOWN_ERROR, WEB_SERVER_UNKNOWN_ERROR) \
  XX(521, WEB_SERVER_IS_DOWN, WEB_SERVER_IS_DOWN) \
  XX(522, CONNECTION_TIMEOUT, CONNECTION_TIMEOUT) \
  XX(523, ORIGIN_IS_UNREACHABLE, ORIGIN_IS_UNREACHABLE) \
  XX(524, TIMEOUT_OCCURED, TIMEOUT_OCCURED) \
  XX(525, SSL_HANDSHAKE_FAILED, SSL_HANDSHAKE_FAILED) \
  XX(526, INVALID_SSL_CERTIFICATE, INVALID_SSL_CERTIFICATE) \
  XX(527, RAILGUN_ERROR, RAILGUN_ERROR) \
  XX(529, SITE_IS_OVERLOADED, SITE_IS_OVERLOADED) \
  XX(530, SITE_IS_FROZEN, SITE_IS_FROZEN) \
  XX(561, IDENTITY_PROVIDER_AUTHENTICATION_ERROR, IDENTITY_PROVIDER_AUTHENTICATION_ERROR) \
  XX(598, NETWORK_READ_TIMEOUT, NETWORK_READ_TIMEOUT) \
  XX(599, NETWORK_CONNECT_TIMEOUT, NETWORK_CONNECT_TIMEOUT) \

    using raw_data_cb = std::function<int(const char*, size_t)>;
    using string_v_data_cb = std::function<int(std::string_view)>;
    using string_data_cb = std::function<int(std::string)>;
    using parser_cb = std::function<int(void)>;

    struct parse_settings {
        parse_settings()
            : on_message_begin(nullptr)
            , on_url(nullptr)
            , on_status(nullptr)
            , on_method(nullptr)
            , on_version(nullptr)
            , on_header_field(nullptr)
            , on_header_value(nullptr)
            , on_chunk_extension_name(nullptr)
            , on_chunk_extension_value(nullptr)
            , on_headers_complete(nullptr)
            , on_body(nullptr)
            , on_message_complete(nullptr)
            , on_url_complete(nullptr)
            , on_status_complete(nullptr)
            , on_method_complete(nullptr)
            , on_version_complete(nullptr)
            , on_header_field_complete(nullptr)
            , on_header_value_complete(nullptr)
            , on_chunk_extension_name_complete(nullptr)
            , on_chunk_extension_value_complete(nullptr)
            , on_chunk_header(nullptr)
            , on_chunk_complete(nullptr)
            , on_reset(nullptr)
        {}
        
        /* Possible return values 0, -1, `HPE_PAUSED` */
        parser_cb        on_message_begin;

        /* Possible return values 0, -1, HPE_USER */
        raw_data_cb      on_url;
        raw_data_cb      on_status;
        raw_data_cb      on_method;
        raw_data_cb      on_version;
        raw_data_cb      on_header_field;
        raw_data_cb      on_header_value;
        raw_data_cb      on_chunk_extension_name;
        raw_data_cb      on_chunk_extension_value;

        /* Possible return values:
        * 0  - Proceed normally
        * 1  - Assume that request/response has no body, and proceed to parsing the
        *      next message
        * 2  - Assume absence of body (as above) and make `llhttp_execute()` return
        *      `HPE_PAUSED_UPGRADE`
        * -1 - Error
        * `HPE_PAUSED`
        */
        parser_cb      on_headers_complete;

        /* Possible return values 0, -1, HPE_USER */
        raw_data_cb on_body;

        /* Possible return values 0, -1, `HPE_PAUSED` */
        parser_cb      on_message_complete;
        parser_cb      on_url_complete;
        parser_cb      on_status_complete;
        parser_cb      on_method_complete;
        parser_cb      on_version_complete;
        parser_cb      on_header_field_complete;
        parser_cb      on_header_value_complete;
        parser_cb      on_chunk_extension_name_complete;
        parser_cb      on_chunk_extension_value_complete;

        /* When on_chunk_header is called, the current chunk length is stored
        * in parser->content_length.
        * Possible return values 0, -1, `HPE_PAUSED`
        */
        parser_cb      on_chunk_header;
        parser_cb      on_chunk_complete;
        parser_cb      on_reset;
    };

    class parser {
    public:
        parser(uint8_t type = Type::BOTH);

        ~parser();
    
        uint8_t type();

        uint8_t http_major();

        uint8_t http_minor();

        uint8_t method();

        int status_code();

        uint8_t upgrade();

        /* Reset an already initialized parser back to the start state, preserving the
        * existing parser type, callback settings, user data, and lenient flags.
        */
        parser& reset();

        /* Initialize the settings object */
        parser& init_settings(parse_settings& settings);

        /* Parse full or partial request/response, invoking user callbacks along the
        * way.
        *
        * If any of `raw_data_cb` returns errno not equal to `HPE_OK` - the parsing
        * interrupts, and such errno is returned from `execute()`. If
        * `HPE_PAUSED` was used as a errno, the execution can be resumed with
        * `resume()` call.
        *
        * In a special case of CONNECT/Upgrade request/response `HPE_PAUSED_UPGRADE`
        * is returned after fully parsing the request/response. If the user wishes to
        * continue parsing, they need to invoke `resume_after_upgrade()`.
        *
        * NOTE: if this function ever returns a non-pause type error, it will continue
        * to return the same error upon each successive call up until the parser is destroyed or reset
        */
        int execute(const char* data, size_t len);

        /* This method should be called when the other side has no further bytes to
        * send (e.g. shutdown of readable side of the TCP connection.)
        *
        * Requests without `Content-Length` and other messages might require treating
        * all incoming bytes as the part of the body, up to the last byte of the
        * connection. This method will invoke `on_message_complete()` callback if the
        * request was terminated safely. Otherwise a error code would be returned.
        */
        int finish();

        /* Returns `1` if the incoming message is parsed until the last byte, and has
        * to be completed by calling `finish()` on EOF
        */
        int message_needs_eof();

        /* Returns `1` if there might be any other messages following the last that was
        * successfully parsed.
        */
        int should_keep_alive();

        /* Make further calls of `execute()` return `HPE_PAUSED` and set
        * appropriate error reason.
        *
        * Important: do not call this from user callbacks! User callbacks must return
        * `HPE_PAUSED` if pausing is required.
        */
        parser& pause();

        /* Might be called to resume the execution after the pause in user's callback.
        * See `execute()` above for details.
        *
        * Call this only if `execute()` returns `HPE_PAUSED`.
        */
        parser& resume();

        /* Might be called to resume the execution after the pause in user's callback.
        * See `execute()` above for details.
        *
        * Call this only if `execute()` returns `HPE_PAUSED_UPGRADE`
        */
        parser& resume_after_upgrade();

        /* Returns the latest return error */
        int err();

        /* Returns the verbal explanation of the latest returned error.
        *
        * Note: User callback should set error reason when returning the error. See
        * `set_error_reason()` for details.
        */
        const char* error_reason();

        /* Assign verbal description to the returned error. Must be called in user
        * callbacks right before returning the errno.
        *
        * Note: `HPE_USER` error code might be useful in user callbacks.
        */
        parser& set_error_reason(const char* reason);

        /* Returns the pointer to the last parsed byte before the returned error. The
        * pointer is relative to the `data` argument of `execute()`.
        *
        * Note: this method might be useful for counting the number of parsed bytes.
        */
        const char* error_pos();

        /* Returns textual name of error code */
        const char* errno_name(int err);

        /* Returns textual name of HTTP method */
        const char* method_name(uint8_t method);

        /* Returns textual name of HTTP status */
        const char* status_name(uint16_t status);

        size_t content_length() const;

        /* Enables/disables lenient header value parsing (disabled by default).
        *
        * Lenient parsing disables header value token checks, extending llhttp's
        * protocol support to highly non-compliant clients/server. No
        * `HPE_INVALID_HEADER_TOKEN` will be raised for incorrect header values when
        * lenient parsing is "on".
        *
        * **(USE AT YOUR OWN RISK)**
        */
        parser& set_lenient_headers(int enabled);


        /* Enables/disables lenient handling of conflicting `Transfer-Encoding` and
        * `Content-Length` headers (disabled by default).
        *
        * Normally `llhttp` would error when `Transfer-Encoding` is present in
        * conjunction with `Content-Length`. This error is important to prevent HTTP
        * request smuggling, but may be less desirable for small number of cases
        * involving legacy servers.
        *
        * **(USE AT YOUR OWN RISK)**
        */
        parser& set_lenient_chunked_length(int enabled);


        /* Enables/disables lenient handling of `Connection: close` and HTTP/1.0
        * requests responses.
        *
        * Normally `llhttp` would error on (in strict mode) or discard (in loose mode)
        * the HTTP request/response after the request/response with `Connection: close`
        * and `Content-Length`. This is important to prevent cache poisoning attacks,
        * but might interact badly with outdated and insecure clients. With this flag
        * the extra request/response will be parsed normally.
        *
        * **(USE AT YOUR OWN RISK)**
        */
        parser& set_lenient_keep_alive(int enabled);

        /* Enables/disables lenient handling of `Transfer-Encoding` header.
        *
        * Normally `llhttp` would error when a `Transfer-Encoding` has `chunked` value
        * and another value after it (either in a single header or in multiple
        * headers whose value are internally joined using `, `).
        * This is mandated by the spec to reliably determine request body size and thus
        * avoid request smuggling.
        * With this flag the extra value will be parsed normally.
        *
        * **(USE AT YOUR OWN RISK)**
        */
        parser& set_lenient_transfer_encoding(int enabled);

        /**
         * Callbacks
         * 
         */

        parser&        on_msg_begin(parser_cb cb);

        /* Possible return values 0, -1, HPE_USER */
        parser&      on_url(raw_data_cb cb);
        parser&      on_status(raw_data_cb cb);
        parser&      on_method(raw_data_cb cb);
        parser&      on_version(raw_data_cb cb);
        parser&      on_header_field(raw_data_cb cb);
        parser&      on_header_value(raw_data_cb cb);
        parser&      on_chunk_extension_name(raw_data_cb cb);
        parser&      on_chunk_extension_value(raw_data_cb cb);

        /* Possible return values:
        * 0  - Proceed normally
        * 1  - Assume that request/response has no body, and proceed to parsing the
        *      next message
        * 2  - Assume absence of body (as above) and make `execute()` return
        *      `HPE_PAUSED_UPGRADE`
        * -1 - Error
        * `HPE_PAUSED`
        */
        parser&      on_headers_complete(parser_cb cb);

        /* Possible return values 0, -1, HPE_USER */
        parser& on_body(raw_data_cb cb);

        /* Possible return values 0, -1, `HPE_PAUSED` */
        parser&      on_msg_complete(parser_cb cb);
        parser&      on_url_complete(parser_cb cb);
        parser&      on_status_complete(parser_cb cb);
        parser&      on_method_complete(parser_cb cb);
        parser&      on_version_complete(parser_cb cb);
        parser&      on_header_field_complete(parser_cb cb);
        parser&      on_header_value_complete(parser_cb cb);
        parser&      on_chunk_extension_name_complete(parser_cb cb);
        parser&      on_chunk_extension_value_complete(parser_cb cb);

        /* When on_chunk_header is called, the current chunk length is stored
        * in parser->content_length.
        * Possible return values 0, -1, `HPE_PAUSED`
        */
        parser&      on_chunk_header(parser_cb cb);
        parser&      on_chunk_complete(parser_cb cb);
        parser&      on_reset(parser_cb cb);

    private:
        struct llhttp__internal_t;
        std::unique_ptr<llhttp__internal_t> m_Impl;

        const llhttp__internal_t* Impl() const { return m_Impl.get(); }
        llhttp__internal_t* Impl() { return m_Impl.get(); }
    };
    
    
} // namespace xhttp


#endif // __XHTTP_H__