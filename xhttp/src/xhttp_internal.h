#ifndef __XHTTP__INTERNAL_T_H__
#define __XHTTP__INTERNAL_T_H__

#include "xhttp.h"

#define CALLBACK_MAYBE(NAME)                                          \
  do {                                                                        \
    const parse_settings* settings;                                        \
    settings = const_cast<const parse_settings*>(this->settings);                 \
    if (!settings || !settings->NAME) {                         \
      err = 0;                                                                \
      break;                                                                  \
    }                                                                         \
    err = settings->NAME();                                                   \
  } while (0)

#define SPAN_CALLBACK_MAYBE(NAME, START, LEN)                         \
  do {                                                                        \
    const parse_settings* settings;                                        \
    settings = const_cast<const parse_settings*>(this->settings);                 \
    if (!settings|| !settings->NAME) {                         \
      err = 0;                                                                \
      break;                                                                  \
    }                                                                         \
    err = settings->NAME(reinterpret_cast<const char*>((START)), (LEN));                           \
    if (err == -1) {                                                          \
      err = Errno::USER;                                                         \
      set_error_reason("Span callback error in " #NAME);     \
    }                                                                         \
  } while (0)

namespace xhttp {
    
struct parser::llhttp__internal_t {

    using llhttp__internal__span_cb = std::function<int(const char*, const char*)>;

    int __internal_init();

    llhttp__internal_t();

    llhttp__internal_t(uint8_t type);

    ~llhttp__internal_t();

    int __internal_execute(const char* p, const char* en_p);

    void set_error_reason(const char*);
    int _message_needs_eof();
    int _should_keep_alive();
    int _before_headers_complete(const char* p, const char* endp);
    int _after_headers_complete(const char* p, const char* endp);
    int _after_message_complete(const char* p, const char* endp);

    int _on_message_begin(const char* p, const char* endp);
    int _on_url(const char* p, const char* endp);
    int _on_url_complete(const char* p, const char* endp);
    int _on_status(const char* p, const char* endp);
    int _on_status_complete(const char* p, const char* endp);
    int _on_method(const char* p, const char* endp);
    int _on_method_complete(const char* p, const char* endp);
    int _on_version(const char* p, const char* endp);
    int _on_version_complete(const char* p, const char* endp);
    int _on_header_field(const char* p, const char* endp);
    int _on_header_field_complete(const char* p, const char* endp);
    int _on_header_value(const char* p, const char* endp);
    int _on_header_value_complete(const char* p, const char* endp);
    int _on_headers_complete(const char* p, const char* endp);
    int _on_message_complete(const char* p, const char* endp);
    int _on_body(const char* p, const char* endp);
    int _on_chunk_header(const char* p, const char* endp);
    int _on_chunk_extension_name(const char* p, const char* endp);
    int _on_chunk_extension_name_complete(const char* p, const char* endp);
    int _on_chunk_extension_value(const char* p, const char* endp);
    int _on_chunk_extension_value_complete(const char* p, const char* endp);
    int _on_chunk_complete(const char* p, const char* endp);
    int _on_reset(const char* p, const char* endp);

    int _before_headers_complete(const unsigned char* p, const unsigned char* endp);
    int _after_headers_complete(const unsigned char* p, const unsigned char* endp);
    int _after_message_complete(const unsigned char* p, const unsigned char* endp);

    int _on_message_begin(const unsigned char* p, const unsigned char* endp);
    int _on_url(const unsigned char* p, const unsigned char* endp);
    int _on_url_complete(const unsigned char* p, const unsigned char* endp);
    int _on_status(const unsigned char* p, const unsigned char* endp);
    int _on_status_complete(const unsigned char* p, const unsigned char* endp);
    int _on_method(const unsigned char* p, const unsigned char* endp);
    int _on_method_complete(const unsigned char* p, const unsigned char* endp);
    int _on_version(const unsigned char* p, const unsigned char* endp);
    int _on_version_complete(const unsigned char* p, const unsigned char* endp);
    int _on_header_field(const unsigned char* p, const unsigned char* endp);
    int _on_header_field_complete(const unsigned char* p, const unsigned char* endp);
    int _on_header_value(const unsigned char* p, const unsigned char* endp);
    int _on_header_value_complete(const unsigned char* p, const unsigned char* endp);
    int _on_headers_complete(const unsigned char* p, const unsigned char* endp);
    int _on_message_complete(const unsigned char* p, const unsigned char* endp);
    int _on_body(const unsigned char* p, const unsigned char* endp);
    int _on_chunk_header(const unsigned char* p, const unsigned char* endp);
    int _on_chunk_extension_name(const unsigned char* p, const unsigned char* endp);
    int _on_chunk_extension_name_complete(const unsigned char* p, const unsigned char* endp);
    int _on_chunk_extension_value(const unsigned char* p, const unsigned char* endp);
    int _on_chunk_extension_value_complete(const unsigned char* p, const unsigned char* endp);
    int _on_chunk_complete(const unsigned char* p, const unsigned char* endp);
    int _on_reset(const unsigned char* p, const unsigned char* endp);

    int32_t _index;
    void* _span_pos0;
    llhttp__internal__span_cb _span_cb0;
    int32_t error;
    const char* reason;
    const char* error_pos;
    void* data;
    void* _current;
    uint64_t content_length;
    uint8_t type;
    uint8_t method;
    uint8_t http_major;
    uint8_t http_minor;
    uint8_t header_state;
    uint8_t lenient_flags;
    uint8_t upgrade;
    uint8_t finish;
    uint16_t flags;
    uint16_t status_code;
    uint8_t initial_message_completed;
    parse_settings* settings;

private:
    enum llparse_match_status_e {
        kMatchComplete,
        kMatchPause,
        kMatchMismatch
    };
    typedef enum llparse_match_status_e llparse_match_status_t;

    struct llparse_match_s {
        llparse_match_status_t status;
        const unsigned char* current;
    };
    typedef struct llparse_match_s llparse_match_t;

    llparse_match_t llparse__match_sequence_to_lower(
        const unsigned char* p,
        const unsigned char* endp,
        const unsigned char* seq, uint32_t seq_len);

    llparse_match_t llparse__match_sequence_to_lower_unsafe(
        const unsigned char* p,
        const unsigned char* endp,
        const unsigned char* seq, uint32_t seq_len);

    llparse_match_t llparse__match_sequence_id(
        const unsigned char* p,
        const unsigned char* endp,
        const unsigned char* seq, uint32_t seq_len);

    enum llparse_state_e {
    s_error,
    s_n_llhttp__internal__n_closed,
    s_n_llhttp__internal__n_invoke_llhttp__after_message_complete,
    s_n_llhttp__internal__n_pause_1,
    s_n_llhttp__internal__n_invoke_is_equal_upgrade,
    s_n_llhttp__internal__n_invoke_llhttp__on_message_complete_2,
    s_n_llhttp__internal__n_chunk_data_almost_done_skip,
    s_n_llhttp__internal__n_chunk_data_almost_done,
    s_n_llhttp__internal__n_consume_content_length,
    s_n_llhttp__internal__n_span_start_llhttp__on_body,
    s_n_llhttp__internal__n_invoke_is_equal_content_length,
    s_n_llhttp__internal__n_chunk_size_almost_done,
    s_n_llhttp__internal__n_invoke_llhttp__on_chunk_extension_name_complete,
    s_n_llhttp__internal__n_invoke_llhttp__on_chunk_extension_name_complete_1,
    s_n_llhttp__internal__n_invoke_llhttp__on_chunk_extension_value_complete,
    s_n_llhttp__internal__n_chunk_extension_quoted_value_done,
    s_n_llhttp__internal__n_invoke_llhttp__on_chunk_extension_value_complete_1,
    s_n_llhttp__internal__n_error_17,
    s_n_llhttp__internal__n_chunk_extension_quoted_value,
    s_n_llhttp__internal__n_invoke_llhttp__on_chunk_extension_value_complete_2,
    s_n_llhttp__internal__n_error_19,
    s_n_llhttp__internal__n_chunk_extension_value,
    s_n_llhttp__internal__n_span_start_llhttp__on_chunk_extension_value,
    s_n_llhttp__internal__n_error_20,
    s_n_llhttp__internal__n_chunk_extension_name,
    s_n_llhttp__internal__n_span_start_llhttp__on_chunk_extension_name,
    s_n_llhttp__internal__n_chunk_extensions,
    s_n_llhttp__internal__n_chunk_size_otherwise,
    s_n_llhttp__internal__n_chunk_size,
    s_n_llhttp__internal__n_chunk_size_digit,
    s_n_llhttp__internal__n_invoke_update_content_length_1,
    s_n_llhttp__internal__n_consume_content_length_1,
    s_n_llhttp__internal__n_span_start_llhttp__on_body_1,
    s_n_llhttp__internal__n_eof,
    s_n_llhttp__internal__n_span_start_llhttp__on_body_2,
    s_n_llhttp__internal__n_invoke_llhttp__after_headers_complete,
    s_n_llhttp__internal__n_headers_almost_done,
    s_n_llhttp__internal__n_header_field_colon_discard_ws,
    s_n_llhttp__internal__n_error_28,
    s_n_llhttp__internal__n_invoke_llhttp__on_header_value_complete,
    s_n_llhttp__internal__n_span_start_llhttp__on_header_value,
    s_n_llhttp__internal__n_header_value_discard_lws,
    s_n_llhttp__internal__n_header_value_discard_ws_almost_done,
    s_n_llhttp__internal__n_header_value_lws,
    s_n_llhttp__internal__n_header_value_almost_done,
    s_n_llhttp__internal__n_header_value_lenient,
    s_n_llhttp__internal__n_error_34,
    s_n_llhttp__internal__n_header_value_otherwise,
    s_n_llhttp__internal__n_header_value_connection_token,
    s_n_llhttp__internal__n_header_value_connection_ws,
    s_n_llhttp__internal__n_header_value_connection_1,
    s_n_llhttp__internal__n_header_value_connection_2,
    s_n_llhttp__internal__n_header_value_connection_3,
    s_n_llhttp__internal__n_header_value_connection,
    s_n_llhttp__internal__n_error_36,
    s_n_llhttp__internal__n_error_37,
    s_n_llhttp__internal__n_header_value_content_length_ws,
    s_n_llhttp__internal__n_header_value_content_length,
    s_n_llhttp__internal__n_error_39,
    s_n_llhttp__internal__n_error_38,
    s_n_llhttp__internal__n_header_value_te_token_ows,
    s_n_llhttp__internal__n_header_value,
    s_n_llhttp__internal__n_header_value_te_token,
    s_n_llhttp__internal__n_header_value_te_chunked_last,
    s_n_llhttp__internal__n_header_value_te_chunked,
    s_n_llhttp__internal__n_span_start_llhttp__on_header_value_1,
    s_n_llhttp__internal__n_header_value_discard_ws,
    s_n_llhttp__internal__n_invoke_llhttp__on_header_field_complete,
    s_n_llhttp__internal__n_header_field_general_otherwise,
    s_n_llhttp__internal__n_header_field_general,
    s_n_llhttp__internal__n_header_field_colon,
    s_n_llhttp__internal__n_header_field_3,
    s_n_llhttp__internal__n_header_field_4,
    s_n_llhttp__internal__n_header_field_2,
    s_n_llhttp__internal__n_header_field_1,
    s_n_llhttp__internal__n_header_field_5,
    s_n_llhttp__internal__n_header_field_6,
    s_n_llhttp__internal__n_header_field_7,
    s_n_llhttp__internal__n_header_field,
    s_n_llhttp__internal__n_span_start_llhttp__on_header_field,
    s_n_llhttp__internal__n_header_field_start,
    s_n_llhttp__internal__n_headers_start,
    s_n_llhttp__internal__n_url_skip_to_http09,
    s_n_llhttp__internal__n_url_skip_lf_to_http09,
    s_n_llhttp__internal__n_req_pri_upgrade,
    s_n_llhttp__internal__n_req_http_complete_1,
    s_n_llhttp__internal__n_req_http_complete,
    s_n_llhttp__internal__n_invoke_load_method_1,
    s_n_llhttp__internal__n_invoke_llhttp__on_version_complete,
    s_n_llhttp__internal__n_error_44,
    s_n_llhttp__internal__n_error_49,
    s_n_llhttp__internal__n_req_http_minor,
    s_n_llhttp__internal__n_error_50,
    s_n_llhttp__internal__n_req_http_dot,
    s_n_llhttp__internal__n_error_51,
    s_n_llhttp__internal__n_req_http_major,
    s_n_llhttp__internal__n_span_start_llhttp__on_version,
    s_n_llhttp__internal__n_req_http_start_1,
    s_n_llhttp__internal__n_req_http_start_2,
    s_n_llhttp__internal__n_req_http_start_3,
    s_n_llhttp__internal__n_req_http_start,
    s_n_llhttp__internal__n_url_skip_to_http,
    s_n_llhttp__internal__n_url_fragment,
    s_n_llhttp__internal__n_span_end_stub_query_3,
    s_n_llhttp__internal__n_url_query,
    s_n_llhttp__internal__n_url_query_or_fragment,
    s_n_llhttp__internal__n_url_path,
    s_n_llhttp__internal__n_span_start_stub_path_2,
    s_n_llhttp__internal__n_span_start_stub_path,
    s_n_llhttp__internal__n_span_start_stub_path_1,
    s_n_llhttp__internal__n_url_server_with_at,
    s_n_llhttp__internal__n_url_server,
    s_n_llhttp__internal__n_url_schema_delim_1,
    s_n_llhttp__internal__n_url_schema_delim,
    s_n_llhttp__internal__n_span_end_stub_schema,
    s_n_llhttp__internal__n_url_schema,
    s_n_llhttp__internal__n_url_start,
    s_n_llhttp__internal__n_span_start_llhttp__on_url_1,
    s_n_llhttp__internal__n_span_start_llhttp__on_url,
    s_n_llhttp__internal__n_req_spaces_before_url,
    s_n_llhttp__internal__n_req_first_space_before_url,
    s_n_llhttp__internal__n_invoke_llhttp__on_method_complete_1,
    s_n_llhttp__internal__n_after_start_req_2,
    s_n_llhttp__internal__n_after_start_req_3,
    s_n_llhttp__internal__n_after_start_req_1,
    s_n_llhttp__internal__n_after_start_req_4,
    s_n_llhttp__internal__n_after_start_req_6,
    s_n_llhttp__internal__n_after_start_req_8,
    s_n_llhttp__internal__n_after_start_req_9,
    s_n_llhttp__internal__n_after_start_req_7,
    s_n_llhttp__internal__n_after_start_req_5,
    s_n_llhttp__internal__n_after_start_req_12,
    s_n_llhttp__internal__n_after_start_req_13,
    s_n_llhttp__internal__n_after_start_req_11,
    s_n_llhttp__internal__n_after_start_req_10,
    s_n_llhttp__internal__n_after_start_req_14,
    s_n_llhttp__internal__n_after_start_req_17,
    s_n_llhttp__internal__n_after_start_req_16,
    s_n_llhttp__internal__n_after_start_req_15,
    s_n_llhttp__internal__n_after_start_req_18,
    s_n_llhttp__internal__n_after_start_req_20,
    s_n_llhttp__internal__n_after_start_req_21,
    s_n_llhttp__internal__n_after_start_req_19,
    s_n_llhttp__internal__n_after_start_req_23,
    s_n_llhttp__internal__n_after_start_req_24,
    s_n_llhttp__internal__n_after_start_req_26,
    s_n_llhttp__internal__n_after_start_req_28,
    s_n_llhttp__internal__n_after_start_req_29,
    s_n_llhttp__internal__n_after_start_req_27,
    s_n_llhttp__internal__n_after_start_req_25,
    s_n_llhttp__internal__n_after_start_req_30,
    s_n_llhttp__internal__n_after_start_req_22,
    s_n_llhttp__internal__n_after_start_req_31,
    s_n_llhttp__internal__n_after_start_req_32,
    s_n_llhttp__internal__n_after_start_req_35,
    s_n_llhttp__internal__n_after_start_req_36,
    s_n_llhttp__internal__n_after_start_req_34,
    s_n_llhttp__internal__n_after_start_req_37,
    s_n_llhttp__internal__n_after_start_req_38,
    s_n_llhttp__internal__n_after_start_req_42,
    s_n_llhttp__internal__n_after_start_req_43,
    s_n_llhttp__internal__n_after_start_req_41,
    s_n_llhttp__internal__n_after_start_req_40,
    s_n_llhttp__internal__n_after_start_req_39,
    s_n_llhttp__internal__n_after_start_req_45,
    s_n_llhttp__internal__n_after_start_req_44,
    s_n_llhttp__internal__n_after_start_req_33,
    s_n_llhttp__internal__n_after_start_req_48,
    s_n_llhttp__internal__n_after_start_req_49,
    s_n_llhttp__internal__n_after_start_req_50,
    s_n_llhttp__internal__n_after_start_req_51,
    s_n_llhttp__internal__n_after_start_req_47,
    s_n_llhttp__internal__n_after_start_req_46,
    s_n_llhttp__internal__n_after_start_req_54,
    s_n_llhttp__internal__n_after_start_req_56,
    s_n_llhttp__internal__n_after_start_req_57,
    s_n_llhttp__internal__n_after_start_req_55,
    s_n_llhttp__internal__n_after_start_req_53,
    s_n_llhttp__internal__n_after_start_req_58,
    s_n_llhttp__internal__n_after_start_req_59,
    s_n_llhttp__internal__n_after_start_req_52,
    s_n_llhttp__internal__n_after_start_req_61,
    s_n_llhttp__internal__n_after_start_req_62,
    s_n_llhttp__internal__n_after_start_req_60,
    s_n_llhttp__internal__n_after_start_req_65,
    s_n_llhttp__internal__n_after_start_req_67,
    s_n_llhttp__internal__n_after_start_req_68,
    s_n_llhttp__internal__n_after_start_req_66,
    s_n_llhttp__internal__n_after_start_req_69,
    s_n_llhttp__internal__n_after_start_req_64,
    s_n_llhttp__internal__n_after_start_req_63,
    s_n_llhttp__internal__n_after_start_req,
    s_n_llhttp__internal__n_span_start_llhttp__on_method_1,
    s_n_llhttp__internal__n_invoke_llhttp__on_status_complete,
    s_n_llhttp__internal__n_res_line_almost_done,
    s_n_llhttp__internal__n_res_status,
    s_n_llhttp__internal__n_span_start_llhttp__on_status,
    s_n_llhttp__internal__n_res_status_start,
    s_n_llhttp__internal__n_res_status_code_otherwise,
    s_n_llhttp__internal__n_res_status_code,
    s_n_llhttp__internal__n_res_after_version,
    s_n_llhttp__internal__n_invoke_llhttp__on_version_complete_1,
    s_n_llhttp__internal__n_error_66,
    s_n_llhttp__internal__n_error_72,
    s_n_llhttp__internal__n_res_http_minor,
    s_n_llhttp__internal__n_error_73,
    s_n_llhttp__internal__n_res_http_dot,
    s_n_llhttp__internal__n_error_74,
    s_n_llhttp__internal__n_res_http_major,
    s_n_llhttp__internal__n_span_start_llhttp__on_version_1,
    s_n_llhttp__internal__n_start_res,
    s_n_llhttp__internal__n_invoke_llhttp__on_method_complete,
    s_n_llhttp__internal__n_req_or_res_method_2,
    s_n_llhttp__internal__n_invoke_update_type_1,
    s_n_llhttp__internal__n_req_or_res_method_3,
    s_n_llhttp__internal__n_req_or_res_method_1,
    s_n_llhttp__internal__n_req_or_res_method,
    s_n_llhttp__internal__n_span_start_llhttp__on_method,
    s_n_llhttp__internal__n_start_req_or_res,
    s_n_llhttp__internal__n_invoke_load_type,
    s_n_llhttp__internal__n_invoke_update_finish,
    s_n_llhttp__internal__n_start,
    };
    typedef enum llparse_state_e llparse_state_t;

    llparse_state_t __internal__run(
        const unsigned char* p,
        const unsigned char* endp);

    int __internal__c_load_initial_message_completed(
        const unsigned char* p,
        const unsigned char* endp);

    int __internal__c_update_finish(
        const unsigned char* p,
        const unsigned char* endp);

    int __internal__c_load_type(
        const unsigned char* p,
        const unsigned char* endp);

    int __internal__c_store_method(
        const unsigned char* p,
        const unsigned char* endp,
        int match);

    int __internal__c_is_equal_method(
        const unsigned char* p,
        const unsigned char* endp);

    int __internal__c_update_http_major(
        const unsigned char* p,
        const unsigned char* endp);

    int __internal__c_update_http_minor(
        const unsigned char* p,
        const unsigned char* endp);

    int __internal__c_test_lenient_flags(
        const unsigned char* p,
        const unsigned char* endp);

    int __internal__c_test_flags(
        const unsigned char* p,
        const unsigned char* endp);

    int __internal__c_is_equal_upgrade(
        const unsigned char* p,
        const unsigned char* endp);

    int __internal__c_update_content_length(
        const unsigned char* p,
        const unsigned char* endp);

    int __internal__c_update_initial_message_completed(
        const unsigned char* p,
        const unsigned char* endp);

    int __internal__c_update_finish_1(
        const unsigned char* p,
        const unsigned char* endp);

    int __internal__c_test_lenient_flags_1(
        const unsigned char* p,
        const unsigned char* endp);

    int __internal__c_test_flags_1(
        const unsigned char* p,
        const unsigned char* endp);

    int __internal__c_test_lenient_flags_2(
        const unsigned char* p,
        const unsigned char* endp);

    int __internal__c_mul_add_content_length(
        const unsigned char* p,
        const unsigned char* endp,
        int match);

    int __internal__c_is_equal_content_length(
        const unsigned char* p,
        const unsigned char* endp);

    int __internal__c_or_flags(
        const unsigned char* p,
        const unsigned char* endp);

    int __internal__c_update_finish_3(
        const unsigned char* p,
        const unsigned char* endp);

    int __internal__c_or_flags_1(
        const unsigned char* p,
        const unsigned char* endp);

    int __internal__c_update_upgrade(
        const unsigned char* p,
        const unsigned char* endp);

    int __internal__c_store_header_state(
        const unsigned char* p,
        const unsigned char* endp,
        int match);

    int __internal__c_load_header_state(
        const unsigned char* p,
        const unsigned char* endp);

    int __internal__c_or_flags_3(
        const unsigned char* p,
        const unsigned char* endp);

    int __internal__c_update_header_state(
        const unsigned char* p,
        const unsigned char* endp);

    int __internal__c_or_flags_4(
        const unsigned char* p,
        const unsigned char* endp);

    int __internal__c_or_flags_5(
        const unsigned char* p,
        const unsigned char* endp);

    int __internal__c_or_flags_6(
        const unsigned char* p,
        const unsigned char* endp);

    int __internal__c_update_header_state_3(
        const unsigned char* p,
        const unsigned char* endp);

    int __internal__c_update_header_state_1(
        const unsigned char* p,
        const unsigned char* endp);

    int __internal__c_update_header_state_6(
        const unsigned char* p,
        const unsigned char* endp);

    int __internal__c_update_header_state_7(
        const unsigned char* p,
        const unsigned char* endp);

    int __internal__c_test_flags_2(
        const unsigned char* p,
        const unsigned char* endp);

    int __internal__c_mul_add_content_length_1(
        const unsigned char* p,
        const unsigned char* endp,
        int match);

    int __internal__c_or_flags_15(
        const unsigned char* p,
        const unsigned char* endp);

    int __internal__c_test_flags_3(
        const unsigned char* p,
        const unsigned char* endp);

    int __internal__c_test_lenient_flags_6(
        const unsigned char* p,
        const unsigned char* endp);

    int __internal__c_or_flags_16(
        const unsigned char* p,
        const unsigned char* endp);

    int __internal__c_and_flags(
        const unsigned char* p,
        const unsigned char* endp);

    int __internal__c_update_header_state_8(
        const unsigned char* p,
        const unsigned char* endp);

    int __internal__c_or_flags_18(
        const unsigned char* p,
        const unsigned char* endp);

    int __internal__c_load_method(
        const unsigned char* p,
        const unsigned char* endp);

    int __internal__c_store_http_major(
        const unsigned char* p,
        const unsigned char* endp,
        int match);

    int __internal__c_store_http_minor(
        const unsigned char* p,
        const unsigned char* endp,
        int match);

    int __internal__c_test_lenient_flags_8(
        const unsigned char* p,
        const unsigned char* endp);

    int __internal__c_load_http_major(
        const unsigned char* p,
        const unsigned char* endp);

    int __internal__c_load_http_minor(
        const unsigned char* p,
        const unsigned char* endp);

    int __internal__c_update_status_code(
        const unsigned char* p,
        const unsigned char* endp);

    int __internal__c_mul_add_status_code(
        const unsigned char* p,
        const unsigned char* endp,
        int match);
    
    int __internal__c_update_type(
        const unsigned char* p,
        const unsigned char* endp);

    int __internal__c_update_type_1(
        const unsigned char* p,
        const unsigned char* endp);

};




} // namespace xhttp


#endif // _XHTTP__INTERNAL_T_H__