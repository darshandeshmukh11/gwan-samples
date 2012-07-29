#ifdef HAVE_CONFIG_H
  #include <config.h>
#endif

#include <time.h>
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

static AvahiEntryGroup *group = NULL;
static AvahiSimplePoll *simple_poll = NULL;
static char *name = "G-WAN (Global-WAN)";

static void create_services(AvahiClient *c);

static void entry_group_callback(AvahiEntryGroup *g, AvahiEntryGroupState state, AVAHI_GCC_UNUSED void *userdata)
{
  group = g;

  switch (state)
  {
      case AVAHI_ENTRY_GROUP_ESTABLISHED: {
        fprintf(stderr, "Service '%s' successfully established.\n", name);
        break;
      }
      
      case AVAHI_ENTRY_GROUP_COLLISION: {
        char *n;

        n = avahi_alternative_service_name(name);
        avahi_free(name);
        name = n;

        fprintf(stderr, "Service name collision, renaming service to '%s'\n", name);

        create_services(avahi_entry_group_get_client(g));
        break;
      }

      case AVAHI_ENTRY_GROUP_FAILURE: {
          fprintf(stderr, "Entry group failure: %s\n", avahi_strerror(avahi_client_errno(avahi_entry_group_get_client(g))));

          avahi_simple_poll_quit(simple_poll);
          break;
      }
      case AVAHI_ENTRY_GROUP_UNCOMMITED: {}
      case AVAHI_ENTRY_GROUP_REGISTERING: {}
  }
}

static void create_services(AvahiClient *c)
{
  if (!group) {
    group = avahi_entry_group_new(c, entry_group_callback, NULL);
  }

  if (avahi_entry_group_is_empty(group)) {
      fprintf(stderr, "Adding service '%s'\n", name);

      avahi_entry_group_add_service(group, AVAHI_IF_UNSPEC, AVAHI_PROTO_UNSPEC, 0, name, "_http._tcp", NULL, NULL, 8080, NULL);

      /* Tell the server to register the service */
      avahi_entry_group_commit(group);
  }
}

static void client_callback(AvahiClient *c, AvahiClientState state, AVAHI_GCC_UNUSED void * userdata) {

    /* Called whenever the client or server state changes */

    switch (state)
    {
        case AVAHI_CLIENT_S_RUNNING: {
          /* The server has startup successfully and registered its host
           * name on the network, so it's time to create our services */
          create_services(c);
          break;
        }

        case AVAHI_CLIENT_FAILURE: {
          fprintf(stderr, "Client failure: %s\n", avahi_strerror(avahi_client_errno(c)));
          avahi_simple_poll_quit(simple_poll);

          break;
        }

        case AVAHI_CLIENT_S_COLLISION: {}
        case AVAHI_CLIENT_S_REGISTERING: {
          /* The server records are now being established. This
           * might be caused by a host name change. We need to wait
           * for our own records to register until the host name is
           * properly esatblished. */

          if (group)
              avahi_entry_group_reset(group);
        }
            break;

        case AVAHI_CLIENT_CONNECTING: {}
    }
}

int init(int argc, char *argv[])
{
  simple_poll = avahi_simple_poll_new();

  avahi_client_new(avahi_simple_poll_get(simple_poll), 0, client_callback, NULL, NULL);

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
