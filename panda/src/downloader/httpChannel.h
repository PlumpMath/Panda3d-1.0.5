// Filename: httpChannel.h
// Created by:  drose (24Sep02)
//
////////////////////////////////////////////////////////////////////
//
// PANDA 3D SOFTWARE
// Copyright (c) 2001 - 2004, Disney Enterprises, Inc.  All rights reserved
//
// All use of this software is subject to the terms of the Panda 3d
// Software license.  You should have received a copy of this license
// along with this source code; you will also find a current copy of
// the license at http://etc.cmu.edu/panda3d/docs/license/ .
//
// To contact the maintainers of this program write to
// panda3d-general@lists.sourceforge.net .
//
////////////////////////////////////////////////////////////////////

#ifndef HTTPCHANNEL_H
#define HTTPCHANNEL_H

#include "pandabase.h"

// This module requires OpenSSL to compile, even if you do not intend
// to use this to establish https connections; this is because it uses
// the OpenSSL library to portably handle all of the socket
// communications.

#ifdef HAVE_SSL
#define OPENSSL_NO_KRB5

#include "httpClient.h"
#include "httpEnum.h"
#include "urlSpec.h"
#include "documentSpec.h"
#include "virtualFile.h"
#include "bioPtr.h"
#include "bioStreamPtr.h"
#include "pmap.h"
#include "pvector.h"
#include "pointerTo.h"
#include "config_downloader.h"
#include "filename.h"
#include <openssl/ssl.h>

class Ramfile;
class HTTPClient;

////////////////////////////////////////////////////////////////////
//       Class : HTTPChannel
// Description : A single channel of communication from an HTTPClient.
//               This is similar to the concept of a 'connection',
//               except that HTTP is technically connectionless; in
//               fact, a channel may represent one unbroken connection
//               or it may transparently close and reopen a new
//               connection with each request.
//
//               A channel is conceptually a single thread of I/O.
//               One document at a time may be requested using a
//               channel; a new document may (in general) not be
//               requested from the same HTTPChannel until the first
//               document has been fully retrieved.
////////////////////////////////////////////////////////////////////
class EXPCL_PANDAEXPRESS HTTPChannel : public VirtualFile {
private:
  HTTPChannel(HTTPClient *client);

public:
  virtual ~HTTPChannel();

  virtual VirtualFileSystem *get_file_system() const;
  virtual Filename get_filename() const;

  virtual bool is_regular_file() const;
  virtual istream *open_read_file() const;
  void close_read_file(istream *stream) const;

  bool will_close_connection() const;

PUBLISHED:
  // get_status_code() will either return an HTTP-style status code >=
  // 100 (e.g. 404), or one of the following values.  In general,
  // these are ordered from less-successful to more-successful.
  enum StatusCode {
    SC_incomplete = 0,
    SC_internal_error,
    SC_no_connection,
    SC_timeout,
    SC_lost_connection,
    SC_non_http_response,
    SC_invalid_http,
    SC_socks_invalid_version,
    SC_socks_no_acceptable_login_method,
    SC_socks_refused,
    SC_socks_no_connection,
    SC_ssl_internal_failure,
    SC_ssl_no_handshake,

    // No one returns this code, but StatusCode values higher than
    // this are deemed more successful than any generic HTTP response.
    SC_http_error_watermark,

    SC_ssl_invalid_server_certificate,
    SC_ssl_unexpected_server,
    SC_download_write_error,
  };

  INLINE bool is_valid() const;
  INLINE bool is_connection_ready() const;
  INLINE const URLSpec &get_url() const;
  INLINE const DocumentSpec &get_document_spec() const;
  INLINE HTTPEnum::HTTPVersion get_http_version() const;
  INLINE const string &get_http_version_string() const;
  INLINE int get_status_code() const;
  string get_status_string() const;
  INLINE const string &get_www_realm() const;
  INLINE const string &get_proxy_realm() const;
  INLINE const URLSpec &get_redirect() const;
  string get_header_value(const string &key) const;

  INLINE int get_num_redirect_trail() const;
  INLINE const URLSpec &get_redirect_trail(int n) const;

  INLINE void set_persistent_connection(bool persistent_connection);
  INLINE bool get_persistent_connection() const;

  INLINE void set_allow_proxy(bool allow_proxy);
  INLINE bool get_allow_proxy() const;
  INLINE void set_proxy_tunnel(bool proxy_tunnel);
  INLINE bool get_proxy_tunnel() const;

  INLINE void set_connect_timeout(double timeout_seconds);
  INLINE double get_connect_timeout() const;
  INLINE void set_blocking_connect(bool blocking_connect);
  INLINE bool get_blocking_connect() const;

  INLINE void set_http_timeout(double timeout_seconds);
  INLINE double get_http_timeout() const;

  INLINE void set_download_throttle(bool download_throttle);
  INLINE bool get_download_throttle() const;

  INLINE void set_max_bytes_per_second(double max_bytes_per_second);
  INLINE double get_max_bytes_per_second() const;

  INLINE void set_max_updates_per_second(double max_updates_per_second);
  INLINE double get_max_updates_per_second() const;

  INLINE void set_expected_file_size(size_t file_size);
  size_t get_file_size() const;
  INLINE bool is_file_size_known() const;

  void write_headers(ostream &out) const;

  INLINE void reset();
  INLINE void preserve_status();

  INLINE void clear_extra_headers();
  INLINE void send_extra_header(const string &key, const string &value);

  INLINE bool get_document(const DocumentSpec &url);
  INLINE bool get_subdocument(const DocumentSpec &url, 
                              size_t first_byte, size_t last_byte);
  INLINE bool get_header(const DocumentSpec &url);
  INLINE bool post_form(const DocumentSpec &url, const string &body);
  INLINE bool put_document(const DocumentSpec &url, const string &body);
  INLINE bool delete_document(const DocumentSpec &url);
  INLINE bool get_trace(const DocumentSpec &url);
  INLINE bool connect_to(const DocumentSpec &url);
  INLINE bool get_options(const DocumentSpec &url);

  INLINE void begin_get_document(const DocumentSpec &url);
  INLINE void begin_get_subdocument(const DocumentSpec &url, 
                                    size_t first_byte, size_t last_byte);
  INLINE void begin_get_header(const DocumentSpec &url);
  INLINE void begin_post_form(const DocumentSpec &url, const string &body);
  bool run();
  INLINE void begin_connect_to(const DocumentSpec &url);

  ISocketStream *read_body();
  bool download_to_file(const Filename &filename, bool subdocument_resumes = true);
  bool download_to_ram(Ramfile *ramfile, bool subdocument_resumes = true);
  SocketStream *get_connection();

  INLINE size_t get_bytes_downloaded() const;
  INLINE size_t get_bytes_requested() const;
  INLINE bool is_download_complete() const;

public:
  static string downcase(const string &s);

private:
  bool reached_done_state();
  bool run_try_next_proxy();
  bool run_connecting();
  bool run_connecting_wait();
  bool run_http_proxy_ready();
  bool run_http_proxy_request_sent();
  bool run_http_proxy_reading_header();
  bool run_socks_proxy_greet();
  bool run_socks_proxy_greet_reply();
  bool run_socks_proxy_connect();
  bool run_socks_proxy_connect_reply();
  bool run_setup_ssl();
  bool run_ssl_handshake();
  bool run_ready();
  bool run_request_sent();
  bool run_reading_header();
  bool run_read_header();
  bool run_begin_body();
  bool run_reading_body();
  bool run_read_body();
  bool run_read_trailer();

  bool run_download_to_file();
  bool run_download_to_ram();

  void begin_request(HTTPEnum::Method method, const DocumentSpec &url, 
                     const string &body, bool nonblocking,
                     size_t first_byte, size_t last_byte);
  void reconsider_proxy();
  void reset_for_new_request();

  void finished_body(bool has_trailer);
  bool reset_download_position(size_t first_byte);

  bool server_getline(string &str);
  bool server_getline_failsafe(string &str);
  bool server_get(string &str, size_t num_bytes);
  bool server_get_failsafe(string &str, size_t num_bytes);
  bool server_send(const string &str, bool secret);
  bool parse_http_response(const string &line);
  bool parse_http_header();
  bool parse_content_range(const string &content_range);

  void check_socket();
  bool verify_server(X509_NAME *subject) const;

  static string get_x509_name_component(X509_NAME *name, int nid);
  static bool x509_name_subset(X509_NAME *name_a, X509_NAME *name_b);

  void make_header();
  void make_proxy_request_text();
  void make_request_text();

  void reset_url(const URLSpec &old_url, const URLSpec &new_url);
  void store_header_field(const string &field_name, const string &field_value);

#ifndef NDEBUG
  static void show_send(const string &message);
#endif

  void reset_download_to();
  void reset_to_new();
  void close_connection();

  static bool more_useful_status_code(int a, int b);

public:
  // This is declared public solely so we can make an ostream operator
  // for it.
  enum State {
    S_new,
    S_try_next_proxy,
    S_connecting,
    S_connecting_wait,
    S_http_proxy_ready,
    S_http_proxy_request_sent,
    S_http_proxy_reading_header,
    S_socks_proxy_greet,
    S_socks_proxy_greet_reply,
    S_socks_proxy_connect,
    S_socks_proxy_connect_reply,
    S_setup_ssl,
    S_ssl_handshake,
    S_ready,
    S_request_sent,
    S_reading_header,
    S_read_header,
    S_begin_body,
    S_reading_body,
    S_read_body,
    S_read_trailer,
    S_failure
  };

private:
  class StatusEntry {
  public:
    INLINE StatusEntry();
    int _status_code;
    string _status_string;
  };
  typedef pvector<URLSpec> Proxies;
  typedef pvector<StatusEntry> StatusList;

  HTTPClient *_client;
  Proxies _proxies;
  size_t _proxy_next_index;
  StatusList _status_list;
  URLSpec _proxy;
  PT(BioPtr) _bio;
  PT(BioStreamPtr) _source;
  bool _persistent_connection;
  bool _allow_proxy;
  bool _proxy_tunnel;
  double _connect_timeout;
  double _http_timeout;
  bool _blocking_connect;
  bool _download_throttle;
  double _max_bytes_per_second;
  double _max_updates_per_second;
  double _seconds_per_update;
  int _bytes_per_update;
  bool _nonblocking;
  string _send_extra_headers;

  DocumentSpec _document_spec;
  DocumentSpec _request;
  HTTPEnum::Method _method;
  string request_path;
  string _header;
  string _body;
  bool _want_ssl;
  bool _proxy_serves_document;
  bool _proxy_tunnel_now;
  bool _server_response_has_no_body;
  size_t _first_byte_requested;
  size_t _last_byte_requested;
  size_t _first_byte_delivered;
  size_t _last_byte_delivered;
  int _connect_count;

  enum DownloadDest {
    DD_none,
    DD_file,
    DD_ram,
  };
  DownloadDest _download_dest;
  bool _subdocument_resumes;
  Filename _download_to_filename;
  ofstream _download_to_file;
  Ramfile *_download_to_ramfile;

  int _read_index;

  HTTPEnum::HTTPVersion _http_version;
  string _http_version_string;
  StatusEntry _status_entry;
  URLSpec _redirect;

  string _proxy_realm;
  string _proxy_username;
  PT(HTTPAuthorization) _proxy_auth;

  string _www_realm;
  string _www_username;
  PT(HTTPAuthorization) _www_auth;

  // What type of response do we get to our HTTP request?
  enum ResponseType {
    RT_none,
    RT_hangup,       // immediately lost connection 
    RT_non_http,     // something that wasn't an expected HTTP response
    RT_http_hangup,  // the start of an HTTP response, then a lost connection
    RT_http_complete // a valid HTTP response completed
  };
  ResponseType _response_type;
  
  // Not a phash_map, to maintain sorted order.
  typedef pmap<string, string> Headers;
  Headers _headers;

  size_t _expected_file_size;
  size_t _file_size;
  size_t _transfer_file_size;
  size_t _bytes_downloaded;
  size_t _bytes_requested;
  bool _got_expected_file_size;
  bool _got_file_size;
  bool _got_transfer_file_size;

  // These members are used to maintain the current state while
  // communicating with the server.  We need to store everything in
  // the class object instead of using local variables because in the
  // case of nonblocking I/O we have to be able to return to the
  // caller after any I/O operation and resume later where we left
  // off.
  State _state;
  State _done_state;
  double _started_connecting_time;
  double _sent_request_time;
  bool _started_download;
  string _proxy_header;
  string _proxy_request_text;
  string _request_text;
  string _working_get;
  size_t _sent_so_far;
  string _current_field_name;
  string _current_field_value;
  ISocketStream *_body_stream;
  BIO *_sbio;
  pvector<URLSpec> _redirect_trail;
  int _last_status_code;
  double _last_run_time;

public:
  virtual TypeHandle get_type() const {
    return get_class_type();
  }
  virtual TypeHandle force_init_type() {init_type(); return get_class_type();}
  static TypeHandle get_class_type() {
    return _type_handle;
  }
  static void init_type() {
    VirtualFile::init_type();
    register_type(_type_handle, "HTTPChannel",
                  VirtualFile::get_class_type());
  }

private:
  static TypeHandle _type_handle;
  friend class ChunkedStreamBuf;
  friend class IdentityStreamBuf;
  friend class HTTPClient;
};

ostream &operator << (ostream &out, HTTPChannel::State state);

#include "httpChannel.I"

#endif  // HAVE_SSL

#endif


