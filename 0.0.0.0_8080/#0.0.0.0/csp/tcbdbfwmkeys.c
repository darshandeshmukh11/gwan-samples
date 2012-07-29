//in response to http://forum.gwan.com/index.php?p=/discussion/comment/4100/#Comment_4100

#include "gwan.h"
#include "stdio.h"

int add_key(const kv_item *item, const void *keys_)
{
  char ***keys = (char***)keys_;
  
  **keys = item->key;
  (*keys)++;
  
  return 1;
}

int main(int argc, char *argv[])
{
  kv_t store;
  kv_init(&store, "store", 1024, 0, 0, 0);
  
  kv_add(&store,
    &(kv_item) {
      .key = "key1",
      .val = "value",
      .flags = 0,
    }
  );

  kv_add(&store,
    &(kv_item) {
      .key = "key2",
      .val = "value",
      .flags = 0,
    }
  );
  
  char *keys[1024];
  char **keys_ptr = keys;
  
  //search by key prefix "k"
  kv_do(&store, "k", sizeof("k") - 1, add_key, &keys_ptr);
  
  printf("Key: %s\n", keys[0]); //Key: key1
  printf("Key: %s\n", keys[1]); //Key: key2
  
  kv_free(&store);
  
  return 204;
}
