//in response to http://forum.gwan.com/index.php?p=/discussion/402/key-value-store-with-in-place-editing-and-appending-for-huge-value/#Item_1

#include "gwan.h"
#include "stdio.h"
#include "stdlib.h"

typedef struct {
  u32 id,
      amount;
} Payment;

typedef struct {
  u32 id;
  kv_t *payments;
} User;

static void user_free(void *value)
{
  User *u = value;
  
  kv_free(u->payments);
  free(u->payments);
  free(u);
}

Payment* payment_add(kv_t *store, u32 amount)
{
  Payment *payment = malloc(sizeof(*payment));
  payment->id = store->nbr_items + 1;
  payment->amount = amount;
  
  kv_add(store, &(kv_item){
    .key = (void*)&payment->id,
    .klen = sizeof(payment->id),
    .val = (void*)payment,
    .flags = 0,
  });
  
  return payment;
}

int main(int argc, char *argv[])
{
  kv_t users_store;
  kv_init(&users_store, "users_store", 50, 0, user_free, 0);

  for (u32 i = 1; i <= 50; i++) //50 users
  {
    kv_t *payments_store = malloc(sizeof(*payments_store));
    kv_init(payments_store, "payments_store", 3, 0, free, 0);
    
    User *user = malloc(sizeof(User));
    user->id = i;
    user->payments = payments_store;
    
    kv_add(&users_store, &(kv_item){
      .key = (void*)&user->id,
      .klen = sizeof(user->id),
      .val = (void*)user,
      .flags = 0,
    });
    
    for (u32 j = 0; j != 3; j++) //3 payments per user
    {      
      payment_add(payments_store, i * 10 + j);
    }
  }
  
  //"...appending one more payment to existing array of payment"
  //search for user id 45
  User *user = (void*)kv_get(&users_store, (void*)&(u32){45}, sizeof(u32));
  
  Payment *payment = payment_add(user->payments, 100);
  
  //"...some small change in the existing array of payment"
  u32 payment_search = 1; //search for payment id 1
  payment = (void*)kv_get(user->payments, (void*)&payment_search, sizeof(payment_search));
  payment->amount = 999;
  
  //list users and payments
  u32 userid;
  for (userid = 1; userid <= users_store.nbr_items; userid++)
  {
    printf("User %i's payments:\n", userid);
    
    User *user = (void*)kv_get(&users_store, (void*)&userid, sizeof(userid));
    
    for (int paymentid = 1; paymentid <= user->payments->nbr_items; paymentid++)
    {
      Payment *payment = (void*)kv_get(user->payments, (void*)&paymentid, sizeof(paymentid));
      printf("\tPayment %i = %i\n", paymentid, payment->amount);
    }
  }
  
  kv_free(&users_store);
  
  return 204; //no content
}
