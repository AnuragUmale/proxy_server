#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <errno.h>
#include <ctype.h>

#ifndef PARSER
#define PARSER

#define DEBUG 0

typedef struct Request Request;
typedef struct Header Header;

struct Request {
    char *method; 
    char *protocol; 
    char *host; 
    char *port; 
    char *path;
    char *version;
    char *buffer;
    size_t buffer_length;
    Header *headers;
    size_t headers_used;
    size_t headers_length;
};

struct Header {
    char * key;
    size_t key_length;
    char * value;
    size_t value_length;
};


Request* create_request();
int parse_request(Request* req, const char* buffer, size_t buffer_length);
void destroy_request(Request *req);
int unparse_request(Request* req, char* buffer, size_t buffer_len);
int unparse_headers(Request* req, char* buffer, size_t buffer_length);
size_t request_total_length(Request* req);
size_t request_header_length(Request* req);
int set_header(Request *req, const char * key, const char * value);
int remove_header(Request* req, const char* key);
int print_request(Request* req, char* buffer, size_t buffer_length, size_t temp_variable);
size_t request_length(Request* req);
void create_header(Request* req);
void destroy_header(Header* req);
size_t header_line_length(Header * head);
int print_headers(Request *req, char * buffer, size_t length);
void debug(const char * format, ...);


#endif