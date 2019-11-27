//
// Created by kremisi on 16/11/2019.
//
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>
#include "set.h"
#include "list.h"
#include "matamazom.h"
#include "order.h"
/*double prodSelling =  productInStorage -> productSelling;
            prodSelling += basicGetPrice(productInStorage ->mtmProductData , amountProductToOrder);

            if(prodSelling > topSeller ->productSelling)
            {
                matamazom ->bestSellingProudctId = productOrderId;
            }
            else if(prodSelling == (topSeller -> productSelling))
            {
                if(IdCmp(productOrderId , topSeller -> productId) == 1)
                {
                    matamazom -> bestSellingProudctId = productOrderId;
                }
            }
             */

typedef struct Proudct_t {
    MtmProductData mtmProductData;
    MatamazomAmountType matamazomAmountType;
    MtmCopyData mtmCopyData;
    MtmFreeData mtmFreeData;
    MtmGetProductPrice mtmProductPrice;
    unsigned int productId;
    double amountInStorge;
    double productSelling;
    char* name;
}*Product ;

typedef struct Matamazom_t{
    MtmFilterProduct mtmFilterProduct;
    unsigned int bestSellingProudctId;
    List products;
    Set order;
    unsigned int ids;
}*Matamzom;
static ListElement FindMyProductInStorage(List products , unsigned int productId)
{
    LIST_FOREACH(ListElement,iterator, products)
    {
        if ((((Product) iterator)->productId) == productId) {
            return (Product) iterator;
        }
    }
    return NULL;//should not get here!
}


static SetElement OrdCopy(SetElement order) {
    return OrderCopy(order);
}

static void OrderFree(SetElement order)
{
    FreeOrder(order);
}

static int OrderCompare(SetElement order1,SetElement order2)
{
    return CompareOrders(order1,order2);
}

static void FreeProduct(ListElement product)
{
    if(product == NULL){
        return;
    }
    free(((Product)product)->name);
    ((Product)product)->mtmFreeData(((Product)product)->mtmProductData);
    free((Product)product);
}

static ListElement ProductCopy(ListElement product)
{
    if(product == NULL)
    {
        return NULL;
    }
    Product copy = malloc(sizeof(struct Proudct_t));
    if(copy == NULL)
    {
        return NULL;
    }
    copy -> name = malloc(sizeof(char)*(strlen(((Product)product) ->name)+1));
    strcpy(copy -> name,((Product)product) ->name);
    copy -> productId = ((Product)product) -> productId;
    copy -> amountInStorge = ((Product)product) -> amountInStorge;
    copy -> productSelling = ((Product)product) -> productSelling;
    copy -> matamazomAmountType = ((Product)product) -> matamazomAmountType;
    copy -> mtmProductData = ((Product)product) -> mtmCopyData(((Product)product) -> mtmProductData);;
    copy -> amountInStorge = ((Product)product) -> amountInStorge;
    copy -> productSelling = ((Product)product) -> productSelling;
    copy -> matamazomAmountType = ((Product)product) -> matamazomAmountType;
    copy -> mtmCopyData = ((Product)product) -> mtmCopyData;
    copy -> mtmFreeData = ((Product)product) -> mtmFreeData;
    copy -> mtmProductPrice = ((Product)product) -> mtmProductPrice;
    return  copy;
}

static Product FindMyProduct(Matamzom matamazom, unsigned int id)
{
    LIST_FOREACH(ListElement,iterator,matamazom -> products)
    {
        if ((((Product) iterator)->productId) == id) {
            return ((Product) iterator);
        }
    }
    return NULL;
}

static Order FindMyOrder(Matamzom matamazom, unsigned int id)
{
    SET_FOREACH(SetElement ,iterator,matamazom -> order)
    {
        if ((GetOrderId((Order) iterator)) == id) {
            return ((Order) iterator);
        }
    }
    return NULL;
}

static double basicGetPrice(MtmProductData basePrice, double amount) {
         return (*(double*)basePrice) * amount;
}

static  int IdCmp(unsigned int id1 , unsigned int id2)
{
    if(id1>id2)return 1;
    else return 2;
}


 bool ValidAmount( const MatamazomAmountType amountType,const double amount) {
    if (amountType == MATAMAZOM_INTEGER_AMOUNT) {

        if (amount - 0.001 <=  floor(amount) ||
            amount + 0.001 >=  ceil(amount))//TODO check if floor&ceil returns double
        {

            return true;
        }
        return false;
    }
    else if (amountType == MATAMAZOM_HALF_INTEGER_AMOUNT)
    {

        if (amount - 0.001 <= floor(amount) || amount + 0.001 >=  ceil(amount))
        {
            return true;
        }
        if ((amount + 0.001  >= ceil(amount)-0.5) && (amount < (ceil(amount)-0.5)))
        {
            return true;
        }
        if(((amount - 0.001) <=  ((floor(amount)) + 0.5)) && (amount > floor(amount) + 0.5))
        {
            return  true;
        }
        return false;
    }
    else
    {
        return true;
    }
}

bool Validname(const char *name)
{
    if(strlen(name) == 0)
    {
        return false;
    }
    char firstletter = name[0];
    if((firstletter >= 0 && firstletter <= 9) || (firstletter <= 'z' && firstletter >= 'a')||(firstletter >= 'A' && firstletter <= 'Z'))
    {
        return true;
    }
    return  false;
}
/*
 ListElement CreateNewProduct(  unsigned int id,  const char *name,
                                      double amount,  MatamazomAmountType amountType,
                                      MtmProductData customData, MtmCopyData copyData,
                                      MtmFreeData freeData, MtmGetProductPrice prodPrice)
{
    Product newProduct = malloc(sizeof(Product));
    if(newProduct == NULL)
    {
        return NULL;
    }
    char* newProductName = malloc(strlen(name) +1);
    if(newProductName == NULL)
    {
        free(newProduct);
        return NULL;
    }
    strcpy(newProductName,name);
    newProduct ->name = newProductName;
    newProduct -> productId = id;
    newProduct -> amountInStorge = amount;
    newProduct -> mtmProductData = copyData(customData);
    newProduct -> matamazomAmountType = amountType;
    newProduct -> mtmFreeData = freeData;
    newProduct -> mtmCopyData = copyData;
    newProduct -> mtmProductPrice = prodPrice;
    printf("*%s",newProductName);
    return newProduct;
}*/


Matamzom  matamazomCreate() {
          Matamzom matamzom = malloc(sizeof(struct Matamazom_t));
          if (matamzom == NULL)
          {
            return NULL;
          }
           matamzom -> products = listCreate(ProductCopy, FreeProduct);
           if(matamzom -> products == NULL)
           {
               free(matamzom);
               return NULL;
           }
           matamzom -> order = setCreate(OrdCopy,OrderFree,OrderCompare);
           if(matamzom -> order == NULL)
           {
              listDestroy(matamzom -> products);
              free(matamzom);
              return NULL;
           }
           matamzom->ids=1;
           return  matamzom;
}

void matamazomDestroy(Matamzom matamzom)
{
     if(matamzom == NULL)
     {
         return;
     }
     listDestroy(matamzom -> products);
     setDestroy(matamzom -> order);
     free(matamzom);
}

MatamazomResult mtmNewProduct(Matamazom matamazom, const unsigned int id, const char *name,
                              const double amount, const MatamazomAmountType amountType,
                              const MtmProductData customData, MtmCopyData copyData,
                              MtmFreeData freeData, MtmGetProductPrice prodPrice)
{
    if(matamazom == NULL || customData == NULL || copyData == NULL || freeData == NULL || name == NULL)
    {
        return MATAMAZOM_NULL_ARGUMENT;
    }
    if(strlen(name) == 0 || Validname(name) == false)
    {
        return MATAMAZOM_INVALID_NAME;
    }
    if(amount < 0 || ValidAmount(amountType,amount) == false)
    {
        return MATAMAZOM_INVALID_AMOUNT;
    }
    if(FindMyProduct(matamazom,id) != NULL)
    {
        return MATAMAZOM_PRODUCT_ALREADY_EXIST;
    }
    struct Proudct_t newProduct;
    newProduct.name = (char*)name;
    newProduct.productId = id;
    newProduct.amountInStorge = amount;
    newProduct.mtmProductData = customData;
    newProduct.matamazomAmountType = amountType;
    newProduct.mtmFreeData = freeData;
    newProduct.mtmCopyData = copyData;
    newProduct.mtmProductPrice = prodPrice;
    newProduct.productSelling = 0;
    ListResult res = listInsertLast(matamazom -> products,&newProduct);
    if(res == LIST_NULL_ARGUMENT){
        return MATAMAZOM_NULL_ARGUMENT;
    }
    if(res == LIST_OUT_OF_MEMORY){
        return MATAMAZOM_OUT_OF_MEMORY;
    }
    return  MATAMAZOM_SUCCESS;
}

MatamazomResult mtmChangeProductAmount(Matamazom matamazom, const unsigned int id, const double amount)
{
    if(matamazom == NULL)
    {
        return MATAMAZOM_NULL_ARGUMENT;
    }
    Product product =  FindMyProduct(matamazom,id);
    if(product == NULL)
    {
        return MATAMAZOM_PRODUCT_NOT_EXIST;
    }
    if(ValidAmount(product->matamazomAmountType,amount)==false)
    {
        return MATAMAZOM_INVALID_AMOUNT;
    }
    if(((product->amountInStorge) - amount) < 0 )
    {
        return MATAMAZOM_INVALID_AMOUNT;
    }
    product->amountInStorge = (product->amountInStorge) + amount;
    return MATAMAZOM_SUCCESS;
}
MatamazomResult mtmClearProduct(Matamazom matamazom, const unsigned int id)
{
    if(matamazom == NULL)
    {
        return MATAMAZOM_NULL_ARGUMENT;
    }
    Product product =  FindMyProduct(matamazom,id);
    if(product == NULL)
    {
        return MATAMAZOM_PRODUCT_NOT_EXIST;
    }
    LIST_FOREACH(ListElement,iterator,matamazom -> products)
    {
        if ((((Product) iterator)->productId) == id) {
            listRemoveCurrent(matamazom->products);
        }
    }

    SET_FOREACH(SetElement ,iterator,matamazom ->order)
    {
        RemoveProductFromOrder(((Order)iterator),id);
    }
    return MATAMAZOM_SUCCESS;
}

unsigned int mtmCreateNewOrder(Matamazom matamazom)
{
    Order order = CreateOrder(matamazom->ids);
    if(order == NULL){
        return 0;
    }
    matamazom->ids ++;
    return matamazom->ids -1;
}

MatamazomResult mtmChangeProductAmountInOrder(Matamazom matamazom, const unsigned int orderId,
                                              const unsigned int productId, const double amount)
{
    if(matamazom == NULL)
    {
        return MATAMAZOM_NULL_ARGUMENT;
    }
    Order order = FindMyOrder(matamazom, orderId);
    if(order==NULL){
        return MATAMAZOM_ORDER_NOT_EXIST;
    }
    Product product = FindMyProduct(matamazom,productId);
    if(product==NULL){
        return MATAMAZOM_PRODUCT_NOT_EXIST;
    }
    if(ValidAmount(product->matamazomAmountType,amount)==false)
    {
        return MATAMAZOM_INVALID_AMOUNT;
    }
    OrderResult res = DecreaseAndIncreaseProduct(productId,order,amount);
    if(res == ORDER_OUT_OF_MEMORY){
        return MATAMAZOM_OUT_OF_MEMORY;
    }
    return  MATAMAZOM_SUCCESS;
}
MatamazomResult mtmCancelOrder(Matamazom matamazom, const unsigned int orderId)
{
    if(matamazom == NULL)
    {
        return MATAMAZOM_NULL_ARGUMENT;
    }
    Order order = FindMyOrder(matamazom, orderId);
    if(order==NULL){
        return MATAMAZOM_ORDER_NOT_EXIST;
    }
    setRemove(matamazom->order,order);
    return MATAMAZOM_SUCCESS;
}
MatamazomResult mtmShipOrder(Matamazom matamazom, const unsigned int orderId)
{
    if(matamazom == NULL)
    {
        return MATAMAZOM_NULL_ARGUMENT;
    }
    Order order = FindMyOrder(matamazom, orderId);
    if(order == NULL){
        return MATAMAZOM_ORDER_NOT_EXIST;
    }
    double amountProductToOrder;
    unsigned  int productOrderId;
    Set productToOrder =  GetProductsSet(order);
    SET_FOREACH(SetElement ,iterator,productToOrder) {
        GetProductIdAndAmount(((ProductToOrder) iterator), &amountProductToOrder, &productOrderId);
        Product productInStorage = FindMyProduct(matamazom, productOrderId);
        if ((productInStorage -> amountInStorge) < amountProductToOrder) {
            return MATAMAZOM_INSUFFICIENT_AMOUNT;
        }
    }
    SET_FOREACH(SetElement , iterator , productToOrder) {
        GetProductIdAndAmount(((ProductToOrder) iterator), &amountProductToOrder, &productOrderId);
        Product productInStorage = FindMyProduct(matamazom, productOrderId);
        Product topSeller = FindMyProduct(matamazom, matamazom->bestSellingProudctId);
        if ((productInStorage -> amountInStorge) == amountProductToOrder) {
            MatamazomResult res = mtmClearProduct(matamazom, productInStorage->productId);
            if (res == MATAMAZOM_NULL_ARGUMENT) {
                return MATAMAZOM_NULL_ARGUMENT;
            }
            MatamazomResult res1 = mtmCancelOrder(matamazom, orderId);
            if (res1 == MATAMAZOM_NULL_ARGUMENT) {
                return MATAMAZOM_NULL_ARGUMENT;
            }
        }
        double newProductSelling = basicGetPrice(productInStorage->mtmProductData, amountProductToOrder) +
                                   productInStorage->productSelling;
        productInStorage->productSelling = newProductSelling;
        if (newProductSelling > topSeller->productSelling) {
            matamazom->bestSellingProudctId = productOrderId;
        }
        if (newProductSelling == topSeller->productSelling) {
            int bigger = IdCmp(productOrderId, topSeller->productId);
            if (bigger == 1) {
                matamazom->bestSellingProudctId = productOrderId;
            }
        }
        mtmChangeProductAmount(productOrderId, amountProductToOrder);
        MatamazomResult res1 = mtmCancelOrder(matamazom, orderId);
        if (res1 == MATAMAZOM_NULL_ARGUMENT) {
            return MATAMAZOM_NULL_ARGUMENT;
        }


    }
}




