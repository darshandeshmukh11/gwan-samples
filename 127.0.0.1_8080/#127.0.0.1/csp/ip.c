#include "gwan.h"

int main(int argc, char *argv[])
{
  xbuf_xcat(get_reply(argv), "%s", get_env(argv, REMOTE_ADDR));

  return 200;
}
