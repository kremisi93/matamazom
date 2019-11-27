//
// Created by kremisi on 16/11/2019.
//
#include "set.h"
#include "list.h"
#include <stdio.h>
#include "order.h"
#include <stdlib.h>

struct Order_t{
    unsigned int orderId;
    Set productToOrder;
};

 struct ProductToOrder_t{
    unsigned int productId;
    double amount;
};
//TODO add to order.h
static int Compare(unsigned int num1, unsigned int num2)
{
    if(num1 > num2)
    {
        return 1;
    }
    else if(num1 > num2)
    {
        return -1;
    }
    else
    {
        return 0;
    }
}


static SetElement CopyProudctToOrder(SetElement proudctToOrder)
{
    if(proudctToOrder == NULL){
        return NULL;
    }
    ProductToOrder newProudctToOrder = malloc(sizeof(struct ProductToOrder_t));
    if(newProudctToOrder == NULL){
        return NULL;
    }
    newProudctToOrder->productId = ((ProductToOrder)proudctToOrder)->productId;
    newProudctToOrder->amount = ((ProductToOrder)proudctToOrder)->amount;
    return newProudctToOrder;
}

static void FreeProudctToOrder(SetElement productToOrder)
{
    if(productToOrder == NULL)
    {
        return;
    }
    free(productToOrder);
}

static int CompareElements(SetElement productToOrder1, SetElement productToOrder2)
{
    int id1 = ((ProductToOrder)productToOrder1)->productId;
    int id2 = ((ProductToOrder)productToOrder2)->productId;
    return Compare(id1,id2);
}
static ProductToOrder  FindMyProduct(Order order, unsigned int productId)
{

    if(order == NULL) {
        return NULL;
    }
    SET_FOREACH(SetElement,iterator,order ->productToOrder) {
        if (((ProductToOrder) iterator) -> productId == productId) {
            return ((ProductToOrder) iterator);
        }
    }
    return NULL;
}

Order CreateOrder(unsigned int orderId)
{
    Order newOrder = malloc(sizeof(struct Order_t));
     if(newOrder == NULL)
     {
         return NULL;
     }
     newOrder -> orderId = orderId;
     newOrder -> productToOrder = setCreate(CopyProudctToOrder, FreeProudctToOrder, CompareElements);
     if(newOrder -> productToOrder == NULL){
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
    setDestroy(order -> productToOrder); // always succses
    free(order);
}

Order OrderCopy(Order order) {
    if (order == NULL) {
        return NULL;
    }
    Order copyOrd = malloc(sizeof(struct Order_t));
    if (copyOrd == NULL) {
        return NULL;
    }
    copyOrd->orderId = order->orderId;
    copyOrd->productToOrder = setCopy(order->productToOrder);
    if (copyOrd->productToOrder == NULL) {
        free(copyOrd);
        return NULL;
    }
    return copyOrd;
}

int CompareOrders(Order order1,Order order2)
{
    int id1 = order1 -> orderId;
    int id2= order2 -> orderId;
    return Compare(id1,id2);
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

int OrderGetNumberOfProductSize(Order order)
{
    if(order == NULL)
    {
        return -1;
    }
    int productsSize = setGetSize(order -> productToOrder);
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

    ProductToOrder product = FindMyProduct(order, productId);

    if(product == NULL && (amount < 0)){
        //TODO : what should i do in case Amount is neg and also the product  is not found ???
    }

    else if(product == NULL && (amount > 0))
    {
        struct  ProductToOrder_t productToAdd;
        productToAdd.productId = productId;
        productToAdd.amount = amount;
        SetResult res = setAdd(order->productToOrder, &productToAdd);
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
        if(((product -> amount )+ amount) > 0)
        {
            product -> amount = (product -> amount) + amount;
        }
        else
        {
            setRemove(order->productToOrder, product);
            // allways succses , productToOrder created and free at createorder function so order->productToOrder isnt null.
            // in addition, in this case block product isnt null.
        }
    }
    return ORDER_SUCCESS;
}

bool ProductExsistInOrder(Order order, unsigned int productId)
{
    if(order == NULL){
        return false;
    }
    ProductToOrder product = FindMyProduct(order, productId);
    if(product == NULL){
        return false;
    }
    return true;
}

double GetProductAmount(Order order, unsigned int productId)
{

    if (order == NULL)
    {
        return -1.0;
    }
    ProductToOrder product = FindMyProduct(order, productId);
    if (product == NULL)
    {
       return -1.0;
    }
    double amount = product -> amount;
    return amount;
}

OrderResult RemoveProductFromOrder(Order order, unsigned int id)
{
    ProductToOrder product = FindMyProduct(order, id);
    SetResult res = setRemove(order->productToOrder,product);
    if(res == SET_NULL_ARGUMENT){
        return ORDER_NULL_ARGUMENT;
    }
    if(res == SET_ITEM_DOES_NOT_EXIST){
        return ORDER_PRODUCT_NOT_FOUND;
    }
    return ORDER_SUCCESS;
}

Set GetProductsSet(Order order) {
    if(order == NULL)
    {
        return  NULL;
    }
    return order -> productToOrder;
}

ProductResult GetProductIdAndAmount(ProductToOrder  productToOrder ,double* amount, unsigned int* id)
{

    if(productToOrder == NULL || amount == NULL || id == NULL)
    {
        return PRODUCT_NULL_ARGUMENT;
    }
    *amount = productToOrder -> amount;
    *id = productToOrder -> productId;
    return PRODUCT_SUCCESS;
}




