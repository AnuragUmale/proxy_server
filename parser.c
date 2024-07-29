#include "parser.h"

#define NUMBER_OF_HEADERS 8
#define MAX_REQUEST_LENGTH 65535
#define MIN_REQUEST_LENGTH 4

static const char* ABSOLUTE_PATH = "/";

Request* create_request(){
    Request* req = (Request*)malloc(sizeof(Request));
    if(req){
        create_header(req);
        req->buffer = NULL;
        req->method = NULL;
        req->protocol = NULL;
        req->host = NULL;
        req->path = NULL;
        req->version = NULL;
        req->buffer_length = 0;
    }
    return req;
}

int parse_request(Request* req, const char* buffer, size_t buffer_length);

void destroy_request(Request *req){
    if (req){   
        if(req->buffer){
            free(req->buffer);
        }
        if(req->path){
            free(req->path);
        }
        if(req->headers_length > 0){
            destroy_header(req->headers);
        }
        free(req);
        req = NULL;
    }
    

}

int unparse_request(Request* req, char* buffer, size_t buffer_len);
int unparse_headers(Request* req, char* buffer, size_t buffer_length);
size_t request_total_length(Request* req);
size_t request_header_length(Request* req);

int set_header(Request *req, const char * key, const char * value){
    Header* head;
    remove_header(req, key);
    if(req->headers_length <= req->headers_used + 1){
        req->headers_length = 2 * req->headers_length;
        req->headers = (Header*)realloc(req->headers, req->headers_length * sizeof(Header));
        if(!req->headers){
            return -1;
        }
    }
    head = req->headers + req->headers_length;
    req->headers_used += 1;
    head->key = (char *)malloc(strlen(key) + 1);
    memcpy(head->key, key, strlen(key));
    head->key[strlen(key)] = '\0';

    head->value = (char *)malloc(strlen(value) + 1);
    memcpy(head->value, value, strlen(value));
    head->value[strlen(value)] = '\0';

    head->key_length = strlen(key);
    head->value_length = strlen(value);

    return 0;
}

Request* get_header(Request* *req, const char * key);
int remove_header(Request* req, const char* key);
int print_request(Request* req, char* buffer, size_t buffer_length, size_t temp_variable);
size_t request_length(Request* req);

void create_header(Request* req){
    req->headers = (Header*)malloc(sizeof(Header) * NUMBER_OF_HEADERS);
    req->headers_length = NUMBER_OF_HEADERS;
    req->headers_used = 0;
}

void destroy_header(Header* head){
    if(head){
        if(head->key){
            free(head->key);
            head->key = NULL;
        }
        if(head->value){
            free(head->value);
            head->value = NULL;
        }
        head->key_length = 0;
        head->value_length = 0;
        free(head);
        head = NULL;
    }
}

void debug(const char * format, ...) {
     va_list args;
     if (DEBUG) {
	  va_start(args, format);
	  vfprintf(stderr, format, args);
	  va_end(args);
     }
}