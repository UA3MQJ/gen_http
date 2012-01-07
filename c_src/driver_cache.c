#include "gen_http.h"

static ErlDrvBinary *cached_reply_bin = 0;

void init_cache() {
  // char reply[] = "Hello world\n";
  char reply[] = "123456789\n";
  int len = strlen(reply);
  int count = 1000;
  // count = 1;
  char headers_fmt[] = "HTTP/1.1 200 OK\r\nContent-Type: text/plain\r\nConnection: keep-alive\r\nContent-Length: %d\r\n\r\n";
  char headers[1024];
  snprintf(headers, sizeof(headers), headers_fmt, len*count);
  int headers_len = strlen(headers);
  ErlDrvBinary *bin = driver_alloc_binary(len*count+headers_len);
  int i;
  memcpy(bin->orig_bytes, headers, headers_len);
  char *ptr = bin->orig_bytes + headers_len;
  for(i = 0; i < count; i++) {
    memcpy(ptr, reply, len);
    ptr += len;
  }
  cached_reply_bin = bin;  
}

int cached_reply(HTTP *d) {
  // return 0;
  
  // This is just a temporary stub code to test in-driver cache
  
  if(memcmp(d->url->orig_bytes, "/dvb/2/manifest.f4m", d->url->orig_size)) {
    return 0;
  }
  
  driver_enq_bin(d->port, cached_reply_bin, 0, cached_reply_bin->orig_size);
  activate_write(d);
  activate_read(d);
  
  return 1;
}