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
#include "matamazom_print.h"

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
    int bestSellingProudctId;
    List products;
    Set order;
    unsigned int ids;
}*Matamzom;

int compareProducts (void* e1,void *e2){
    Product P1 = e1;
    Product P2 = e2;
    if(P1->productId < P2->productId) return -1;
    else if (P1->productId == P2->productId) return 0;
    else return 1;
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

static char* productName (Matamazom matamazom, int id) {
    if (matamazom == NULL) {
        return NULL;
    }
    LIST_FOREACH(Product, iterator, matamazom->products) {
        if (iterator->productId == id) {
            return iterator->name;
        }
    }
    return NULL;
}

static double productPrice (Matamazom matamazom, int id){
    if(matamazom == NULL){
        return 0;
    }
    LIST_FOREACH(Product, iterator, matamazom->products){
        if(iterator->productId == id){
            return iterator->mtmProductPrice(iterator->mtmProductData, 1);
        }
    }
    return 0;
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

MatamazomResult mtmPrintInventory(Matamazom matamazom, FILE *output){
    if(matamazom == NULL || output == NULL){
        return MATAMAZOM_NULL_ARGUMENT;
    }
    printf("Inventory Status:\n");
    List duplicate = listCopy(matamazom->products);
    listSort(duplicate, compareProducts);
    LIST_FOREACH(Product, iterator, matamazom->products){
        mtmPrintProductDetails(iterator->name, iterator->productId, iterator->amountInStorge, iterator->mtmProductPrice(iterator->mtmProductData,1), output);
    }
    listDestroy(duplicate);
    return MATAMAZOM_SUCCESS;
}
MatamazomResult mtmPrintOrder(Matamazom matamazom, const unsigned int orderId, FILE *output) {
    if (matamazom == NULL || output == NULL) {
        return MATAMAZOM_NULL_ARGUMENT;
    }
    bool found = false;
    Set to_print = NULL;
    double sum = 0;
    SET_FOREACH(Order, iterator, matamazom->order) {
        if (GetOrderId(iterator) == orderId) {
            found = true;
            to_print = GetProductsSet(iterator);
        }
    }
    if (!found) {
        return MATAMAZOM_ORDER_NOT_EXIST;
    }
    mtmPrintOrderHeading(orderId, output);
    SET_FOREACH(ProductToOrder, iterator, to_print){
        double* amount = NULL;
        unsigned int* id = NULL;
        GetProductIdAndAmount(iterator, amount, id);
        char* name = productName(matamazom, *id);
        double price = (*amount)*productPrice(matamazom, *id);
        mtmPrintProductDetails(name,*id, *amount,price,output);
        sum = sum + price;
    }
    mtmPrintOrderSummary(sum,output);
    return MATAMAZOM_SUCCESS;
}
MatamazomResult mtmPrintFiltered(Matamazom matamazom, MtmFilterProduct customFilter, FILE *output) {
    if (matamazom == NULL || customFilter == NULL || output == NULL) {
        return MATAMAZOM_NULL_ARGUMENT;
    }
    LIST_FOREACH(Product, iterator, matamazom->products) {
        char *name = iterator->name;
        int id = iterator->productId;
        double amount = iterator->amountInStorge;

        if (customFilter(id, name, amount, iterator->mtmProductData)) {
            mtmPrintProductDetails(name, id, amount, productPrice(matamazom, id), output);
        }
    }
    return MATAMAZOM_SUCCESS;
}

static int compareProductPrice(void* Product_1, void* Product_2){
    Product P1 = (Product)Product_1;
    Product P2 = (Product)Product_2;
    if(P1->productSelling > P2->productSelling){
        return 1;
    }
    else if (P1->productSelling == P2->productSelling){
        return 0;
    }
    else return -1;
}

MatamazomResult mtmPrintBestSelling(Matamazom matamazom, FILE *output){
    if(matamazom == NULL || output == NULL){
        return MATAMAZOM_NULL_ARGUMENT;
    }
    printf("Best Selling Product:\n");
    List duplicate = listCopy(matamazom->products);
    listSort(duplicate,compareProductPrice);
    while(listGetSize(duplicate)>0) {
        LIST_FOREACH(Product, iterator, duplicate) {
            mtmPrintIncomeLine(iterator->name, iterator->productId, iterator->productSelling, output);
        }
    }
    listDestroy(duplicate);
    return MATAMAZOM_SUCCESS;
}


