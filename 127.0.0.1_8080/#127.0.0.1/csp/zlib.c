//in response to http://forum.gwan.com/index.php?p=/discussion/514/zlib_cmp-customizing-logs/p1

#include "gwan.h"

int main(int argc, char *argv[])
{
  xbuf_t data;
  xbuf_init(&data);
  xbuf_cat(&data,
    "Lorem ipsum dolor sit amet, consectetur adipiscing elit. Morbi sit amet "
    "quam purus, vitae fermentum turpis. Nam in augue mi. Donec suscipit moles"
    "tie felis, eget rhoncus risus pharetra in. Sed id diam id felis fringilla"
    " adipiscing non vitae odio. Etiam vulputate tristique elit, nec eleifend"
    " mauris scelerisque eu. Vestibulum luctus, enim a luctus posuere, mauris"
    " mauris rutrum mi, eget fermentum massa tortor id enim. Nulla feugiat "
    "porta urna quis laoreet. Morbi metus ante, commodo quis dictum vitae, "
    "rhoncus a libero. Cras viverra feugiat orci id interdum. Duis pulvinar "
    "neque id erat adipiscing facilisis. Maecenas vitae urna risus, euismod"
    " sollicitudin risus."
  );
  
  char gzipped[4096];
  
  u32 len = zlib_cmp(data.ptr, 0, data.len, gzipped, sizeof(gzipped), 1);
  
  printf("Original length: %i\n", data.len);
  printf("Gzipped length:  %i\n\n", len);
  
  xbuf_t *reply = get_reply(argv);
  
  xbuf_xcat(reply,
    "HTTP/1.1 200 OK\r\n"
    "Content-Type: text/plain\r\n"
    "Content-Length: %i\r\n"
    "Content-Encoding: gzip\r\n\r\n",
    len
  );
  
  xbuf_ncat(reply, gzipped, len);
  
  xbuf_free(&data);

  return -1; //custom headers
}
