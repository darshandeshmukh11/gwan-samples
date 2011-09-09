//in response to http://forum.gwan.com/index.php?p=/discussion/402/key-value-store-with-in-place-editing-and-appending-for-huge-value/#Item_1

//Might have a memory leak. Proceed with caution.

#include "gwan.h"
#include "stdlib.h"

typedef struct {
  u32 id;
  u32 amount;
} Payment;

typedef struct {
  u32 id;
  kv_t *payments;
} User;

static void user_free(void *value)
{  
  kv_free(((User*)value)->payments);
  free(value);
}

Payment* payment_add(kv_t *store, u32 amount)
{
  Payment *payment = (Payment*)malloc(sizeof(Payment));
  payment->id = store->nbr_items + 1;
  payment->amount = amount;
  
  kv_item item = {
    .key = &payment->id,
    .klen = sizeof(item.key),
    .val = payment,
    .flags = 0,
  };
  
  kv_add(store, &item);
  
  return payment;
}

int main(int argc, char *argv[])
{
  kv_t users_store;
  kv_init(&users_store, "users_store", 50, 0, &user_free, 0);

  u32 i;
  for (i = 1; i <= 50; i++) //50 users
  {
    kv_t *payments_store = (kv_t*)malloc(sizeof(kv_t));
    kv_init(payments_store, "payments_store", 3, 0, &free, 0);
    
    User *user = (User*)malloc(sizeof(User));
    user->id = i;
    user->payments = payments_store;
    
    kv_item item = {
      .key = &user->id,
      .klen = sizeof(item.key),
      .val = user,
      .flags = 0,
    };
    
    kv_add(&users_store, &item);
    
    u32 j;
    for (j = 0; j != 3; j++) //3 payments per user
    {      
      payment_add(payments_store, i * 10 + j);
    }
  }
  
  //"...appending one more payment to existing array of payment"
  u32 user_search = 45; //search for user id 45
  
  User *user = (User*)kv_get(&users_store, &user_search, sizeof(user_search));
  Payment *payment = payment_add(user->payments, 100);
  
  //"...some small change in the existing array of payment"
  u32 payment_search = 1; //search for payment id 1
  payment = (Payment*)kv_get(user->payments, &payment_search, sizeof(payment_search));
  payment->amount = 999;
  
  //list users and payments
  u32 userid;
  for (userid = 1; userid <= users_store.nbr_items; userid++)
  {
    printf("User %i's payments:\n", userid);
    
    User *user = (User*)kv_get(&users_store, &userid, sizeof(userid));
    
    int paymentid;
    for (paymentid = 1; paymentid <= user->payments->nbr_items; paymentid++)
    {
      Payment *payment = (Payment*)kv_get(user->payments, &paymentid, sizeof(paymentid));
      printf("\tPayment %i = %i\n", paymentid, payment->amount);
    }
  }
  
  kv_free(&users_store);
  
  return 200;
}
