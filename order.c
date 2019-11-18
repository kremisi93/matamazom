//
// Created by kremisi on 16/11/2019.
//
#include "set.h"
#include <stdio.h>
#include "order.h"
#include <stdlib.h>
struct Order_t{
    unsigned int orderId;
    Set proudctToOrder;
};

typedef struct ProudctToOrder_t{
    unsigned int productId;
    double amountForOrder;
}*ProudctToOrder;


static SetElement CopyProudctToOrder(SetElement proudctToOrder)
{
    if(proudctToOrder == NULL){
        return NULL;
    }
    ProudctToOrder newProudctToOrder = malloc(sizeof(ProudctToOrder));
    if(newProudctToOrder == NULL){
        return NULL;
    }
    newProudctToOrder->productId = ((ProudctToOrder)proudctToOrder)->productId;
    newProudctToOrder->amountForOrder = ((ProudctToOrder)proudctToOrder)->amountForOrder;
    return newProudctToOrder;
}
static void FreeProudctToOrder(SetElement proudctToOrder)
{
    if(proudctToOrder==NULL)
    {
        return;
    }
    free(proudctToOrder);
}

static int CompareElements(SetElement proudctToOrder1, SetElement proudctToOrder2)
{
    int id1 = ((ProudctToOrder)proudctToOrder1)->productId;
    int id2 = ((ProudctToOrder)proudctToOrder2)->productId;
    if(id1 > id2)
    {
        return 1;
    }
    else if(id2 > id1)
    {
        return -1;
    }
    else
    {
        return 0;
    }
}
static ProudctToOrder  FindMyProduct(Order order,int productId)
{
    if(order == NULL) {
        return NULL;
    }
    SET_FOREACH(SetElement,iterator,order ->proudctToOrder) {
        if (((ProudctToOrder) iterator) -> productId == productId) {
            return ((ProudctToOrder) iterator);
        }
    }
    return NULL;
}

Order CreateOrder(unsigned int orderId)
{
    Order newOrder = malloc(sizeof(Order));
     if(newOrder == NULL)
     {
         return NULL;
     }
     newOrder->orderId = orderId;
     newOrder->proudctToOrder = setCreate(CopyProudctToOrder, FreeProudctToOrder, CompareElements);
     if(newOrder->proudctToOrder == NULL){
         free(newOrder);
         return NULL;
     }
     return newOrder;
}

void FreeOrder(Order order)
{
    if(order==NULL)
    {
        return;
    }
    setDestroy(order -> proudctToOrder); // always succses
    free(order);
}

Order OrderCopy(Order order)
{
      if(order==NULL)
      {
        return NULL;
      }
      Order copyOrd = malloc(sizeof(Order));
      if(copyOrd==NULL)
      {
          return NULL;
      }
      copyOrd->orderId = order -> orderId;
      copyOrd->proudctToOrder = setCopy(order -> proudctToOrder);
      if(copyOrd->proudctToOrder == NULL){
          free(copyOrd);
          return NULL;
      }
      return copyOrd;
}

int GetOrderId(Order order)
{
    if(order==NULL)
    {
        return -1;
    }
    int orderId = order -> orderId;
    return orderId;
}

/*int GetProductToOrderId(ProudctToOrder productToOrder)
{
    if(productToOrder==NULL)
    {
        return -1;
    }
    int producToOrderId=productToOrder->productId;
    return producToOrderId;
}*/

int OrderGetNumberOfProductSize(Order order)
{
    if(order == NULL)
    {
        return -1;
    }
    int productsSize = setGetSize(order -> proudctToOrder);
    if(productsSize == -1)
    {
        return -1;
        // should not get here,cause productToOrder created and free at createorder function.
    }
    return productsSize;
}

OrderResult DecreaseAndIncreaseProduct(unsigned int productId, Order order, double amount)
{
    if(order == NULL){
        return ORDER_NULL_ARGUMENT;
    }
    ProudctToOrder product = FindMyProduct(order, productId);

    if(product == NULL && amount < 0){
        //TODO : what should i do in case Amount is neg and also the product there is not found ???
    }
    else if(product == NULL && amount > 0)
    {
        struct  ProudctToOrder_t productToAdd;
        productToAdd.productId=productId;
        productToAdd.amountForOrder = amount;
        SetResult res = setAdd(order->proudctToOrder,&productToAdd);
        if(res == SET_NULL_ARGUMENT )
        {
            return  ORDER_NULL_ARGUMENT;
            // should not get here,cause productToOrder created and free at createorder function.

        }
        else if(res == SET_OUT_OF_MEMORY)
        {
            return ORDER_OUT_OF_MEMORY;
        }
    }
    else
    {
        if(product -> amountForOrder + amount > 0)
        {
            product->amountForOrder = product->amountForOrder + amount;
        }
        else
        {
            setRemove(order->proudctToOrder,product);
            // allways succses , productToOrder created and free at createorder function so order->proudctToOrder isnt null.
            // in addition, in this case block product isnt null.
        }
    }

    return ORDER_SUCCESS; //should not get here!
}

bool ProductExsistInOrder(Order order, unsigned int productId)
{
    if(order == NULL){
        return false;
    }
    ProudctToOrder product = FindMyProduct(order, productId);
    if(product == NULL){
        return false;
    }
    return true;
}

OrderResult OrderGetProductAmount(Order order, unsigned int productId, double* amount)
{
    printf("jhdsjs");
    if (order == NULL || amount == NULL) {
        return ORDER_NULL_ARGUMENT;
    }
    ProudctToOrder product = FindMyProduct(order, productId);
    if (product == NULL) {
        return ORDER_PRODUCT_NOT_FOUND;
    }
    (*amount) = (product -> amountForOrder);
    return ORDER_SUCCESS;
}
