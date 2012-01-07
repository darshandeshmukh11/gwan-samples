#include "gwan.h"

int main(int argc, char *argv[])
{
  xbuf_xcat(get_reply(argv), "%s", get_env(argv, REMOTE_ADDR, 0));

  return 200;
}
