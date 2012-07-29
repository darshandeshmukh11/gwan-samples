/*

Installation: rename to main.c and place in handlers directory
Usage: make sure avahi-daemon is running, then start G-WAN

*/

#include "gwan.h"

#include <stdio.h>
#include <stdlib.h>

#include <avahi-client/client.h>
#include <avahi-client/publish.h>
#include <avahi-common/alternative.h>
#include <avahi-common/simple-watch.h>
#include <avahi-common/malloc.h>
#include <avahi-common/error.h>
#include <avahi-common/timeval.h>

#pragma link "avahi-client"

static char *name = "G-WAN (Global-WAN)";

static void entry_group_callback(AvahiEntryGroup *g, AvahiEntryGroupState state, void *userdata)
{
  *(AvahiEntryGroup **)userdata = g;

  switch (state)
  {
    case AVAHI_ENTRY_GROUP_ESTABLISHED: {
      printf("Service '%s' successfully established.\n", name);
      break;
    }

    case AVAHI_ENTRY_GROUP_COLLISION: { break; }
    case AVAHI_ENTRY_GROUP_FAILURE: { break; }
    case AVAHI_ENTRY_GROUP_UNCOMMITED: { break; }
    case AVAHI_ENTRY_GROUP_REGISTERING: { break; }
  }
}

static void client_callback(AvahiClient *c, AvahiClientState state, AVAHI_GCC_UNUSED void * userdata)
{
  switch (state)
  {
    case AVAHI_CLIENT_S_RUNNING: {
      static AvahiEntryGroup *group = NULL;
      
      if (!group) {
        avahi_entry_group_new(c, entry_group_callback, &group);
      }

      if (avahi_entry_group_is_empty(group)) {
        printf("Adding service '%s'\n", name);

        avahi_entry_group_add_service(group, AVAHI_IF_UNSPEC, AVAHI_PROTO_UNSPEC, 0, name, "_http._tcp", NULL, NULL, 8080, NULL);

        avahi_entry_group_commit(group);
      }
      
      break;
    }

    case AVAHI_CLIENT_FAILURE: { break; }
    case AVAHI_CLIENT_S_COLLISION: { break; }
    case AVAHI_CLIENT_S_REGISTERING: { break; }
    case AVAHI_CLIENT_CONNECTING: { break; }
  }
}

static AvahiSimplePoll *simple_poll = NULL;
int init(int argc, char *argv[])
{
  puts("Avahi zero-conf handler init.");
  
  simple_poll = avahi_simple_poll_new();
  
  int error = 0;
  avahi_client_new(avahi_simple_poll_get(simple_poll), 0, client_callback, NULL, &error);
  printf("Avahi status: %s\n", avahi_strerror(error));
  
  avahi_simple_poll_loop(simple_poll);
    
  return 255;
}

int main(int argc, char *argv[])
{
  return 255;
}

int clean(int argc, char *argv[])
{
  avahi_simple_poll_quit(simple_poll);
  
  return 255;
}
