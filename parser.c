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

size_t header_line_length(Header * head){
    if(head->key){
        return strlen(head->key) + strlen(head->value);
    }
    return 0;
}

size_t request_header_length(Request* req){
    if(req){
        size_t length = 0;
        for(int i = 0; req->headers_used > i; i++){
            length += header_line_length(req->headers + i);
        }
        length += 2;
        return length;
    }
    return 0;
}

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

Header* get_header(Request *req, const char * key){
    size_t counter = 0;
    Header *temp_header;
    while(req->headers_used > counter){
        temp_header = req->headers + counter;
        if(temp_header->key && key && !strcmp(temp_header->key, key)){
            return temp_header;
        }
        counter++;
    }
    return NULL;
}

int remove_header(Request* req, const char* key){
    Header *temp_header;
    temp_header = get_header(req,key);
    if(temp_header == NULL){
        return -1;
    }
    req->headers_used--;
    free(temp_header->key);
    free(temp_header->value);
    temp_header->key = NULL;
    temp_header->value = NULL;
    return 0;
}
int print_request(Request* req, char* buffer, size_t buffer_length, size_t temp_variable);
size_t request_length(Request* req);

int print_headers(Request *req, char * buffer, size_t length){
    char *current = buffer;
    Header* head;
    size_t i = 0;
    if(length < header_line_length(req->headers)){
        debug("Headers are too small to print\r\n\r\n");
    }
    while(req->headers_used > i){
        head = req->headers + i;
        if(req->headers){
            memcpy(current, head->key, strlen(head->key));
            memcpy(current+strlen(head->key), ": ", 2);
	        memcpy(current+strlen(head->key) +2 , head->value, strlen(head->value));
	        memcpy(current+strlen(head->key) +2+strlen(head->value) , "\r\n", 2);
	       current += strlen(head->key)+strlen(head->value)+4;
        }
        i++;
    }
    memcpy(current, "\r\n",2);
    // current += 2;
    return 0;

}

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