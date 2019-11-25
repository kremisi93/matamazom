#include <stdio.h>
#include <stdlib.h>
#include "order.h"
#include "test_utilities.h"
#include "matamazom.h"
#include "set.h"
#include "list.h"
#include <math.h>


static MtmProductData copyDouble(MtmProductData number) {
    double *copy = malloc(sizeof(*copy));
    if (copy) {
        *copy = *(double*)number;
    }
    return copy;
}

static void freeDouble(MtmProductData number) {
    free(number);
}

static double simplePrice(MtmProductData basePrice, const double amount) {
    return (*(double*)basePrice) * amount;
}

int TestCreateOrder(){
    int batelId = 204571509;
    Order order = CreateOrder(batelId);
    int orderId = GetOrderId(order);
    ASSERT_TEST(batelId == orderId);
    FreeOrder(order);
    printf("TestCreateOrder is working!\n");
    return 1;
}
int TestCopyOrder_Faild()
{
    Order order=NULL;
    Order newOrder=OrderCopy(order);
    ASSERT_TEST(newOrder == NULL);
    FreeOrder(order);
    FreeOrder(newOrder);
    printf("TestCopyOrder_Faild is working!\n");
    return 1;
}

int TestCopyOrder_Succses()
{
    int batelId = 204571509;
    Order order = CreateOrder(batelId);
    Order newOrder=OrderCopy(order);
    int newOrderId = GetOrderId(newOrder);
    ASSERT_TEST(newOrderId==batelId);
    FreeOrder(order);
    FreeOrder(newOrder);
    printf("TestCopyOrder_Succses is working!\n");
    return 1;
}

int TestGetIdOrder()
{
    unsigned int orId=204493613;
    Order order = CreateOrder(orId);
    int orderId = GetOrderId(order);
    ASSERT_TEST(orderId == orId);
    FreeOrder(order);
    printf("TestGetIdOrder is working!\n");
    return 1;
}

int TestOrderGetSize()
{
    int orderId = 204493613;
    Order order = CreateOrder(orderId);
    int size = OrderGetNumberOfProductSize(order);
    ASSERT_TEST(size == 0);
    FreeOrder(order);
    printf("TestOrderGetSize is working!\n");
    return 1;
}

int TestOrderGetProductAmount() {
    unsigned  int BatelIdOrder = 204571;
    unsigned int idProduct = 1;
    double  amount = 100;
    Order BatelOrder = CreateOrder(BatelIdOrder);
    ASSERT_TEST(GetProductAmount(BatelOrder, idProduct) != amount);
    ASSERT_TEST(DecreaseAndIncreaseProduct(idProduct,BatelOrder,amount) == ORDER_SUCCESS);
    ASSERT_TEST(GetProductAmount(NULL, idProduct) == -1.0);
   // ASSERT_TEST(OrderGetProductAmount(BatelOrder, idProduct) == NULL);
    ASSERT_TEST(GetProductAmount(BatelOrder, idProduct + 1 ) == -1.0);
    ASSERT_TEST(GetProductAmount(BatelOrder, idProduct) == amount);
    FreeOrder(BatelOrder);
    printf("TestOrderGetProductAmount is working!\n");
    return 1;
}

int TestProductExsistInOrder() {
    int batelIdOrder = 204571509;
    unsigned  int idProduct = 3;
    //double  amount = 100.0;
    Order order = CreateOrder(batelIdOrder);
    ASSERT_TEST(ProductExsistInOrder(order, idProduct) == false);
    ASSERT_TEST(ProductExsistInOrder(NULL, idProduct) == false);
    //ASSERT_TEST(OrderResult DecreaseAndIncreaseProduct((idProduct,order,amount) == ORDER_SUCCESS);
    ASSERT_TEST(ProductExsistInOrder(NULL, idProduct) == false);
    ASSERT_TEST(ProductExsistInOrder(order, idProduct + 1) == false);
    //ASSERT_TEST(ProductExsistInOrder(order, idProduct) == true);
    FreeOrder(order);
    printf("TestProductExsistInOrder is working!\n");
    return 1;
}

int TestDecreaseAndIncreaseProduct() {
    unsigned int batelId = 204571509;
    Order order = CreateOrder(batelId);
    ASSERT_TEST(DecreaseAndIncreaseProduct(1,order,2.0) == ORDER_SUCCESS);
    ASSERT_TEST(DecreaseAndIncreaseProduct(1,order,-3.0) == ORDER_SUCCESS);
    ASSERT_TEST(DecreaseAndIncreaseProduct(2,NULL,2.0) == ORDER_NULL_ARGUMENT);

    FreeOrder(order);
    printf("TestDecreaseAndIncreaseProduct is working!\n");
    return 1;
}
int TestValidAmount(){
    ASSERT_TEST(ValidAmount(MATAMAZOM_INTEGER_AMOUNT,5.001) == true);
    ASSERT_TEST(ValidAmount(MATAMAZOM_INTEGER_AMOUNT,8.011) == false);
    ASSERT_TEST(ValidAmount(MATAMAZOM_HALF_INTEGER_AMOUNT,8.501) == true);
    ASSERT_TEST(ValidAmount(MATAMAZOM_HALF_INTEGER_AMOUNT,8.5011) == false);
    ASSERT_TEST(ValidAmount(MATAMAZOM_ANY_AMOUNT,8.501) == true);
    printf("TestValidamount is working!\n");
    return 1;
}
int TestValidName(){
    char* name1 = "or";
    char* name2 = "#batel";
    ASSERT_TEST(Validname(name1) == true);
    ASSERT_TEST(Validname(name2) == false);
    printf("TestValidName is working!\n");
    return 1;
}

int TestMtmNewProduct() {
    Matamazom matamazom = matamazomCreate();
    ASSERT_TEST(matamazom!=NULL);
    double d = 1.5;
    ASSERT_TEST(mtmNewProduct(NULL, 1, "OR", 2.0, MATAMAZOM_INTEGER_AMOUNT, NULL, NULL, NULL, NULL) == MATAMAZOM_NULL_ARGUMENT);
    ASSERT_TEST(mtmNewProduct(matamazom,1,"OR",2.0,MATAMAZOM_INTEGER_AMOUNT,&d,copyDouble,freeDouble,simplePrice) == MATAMAZOM_SUCCESS);
    ASSERT_TEST(mtmNewProduct(matamazom,2,"OR",2.0,MATAMAZOM_INTEGER_AMOUNT,&d,copyDouble,freeDouble,simplePrice) == MATAMAZOM_SUCCESS);
    ASSERT_TEST(mtmNewProduct(matamazom,3,"OR",2.0,MATAMAZOM_INTEGER_AMOUNT,&d,copyDouble,freeDouble,simplePrice) == MATAMAZOM_SUCCESS);
    ASSERT_TEST(mtmNewProduct(matamazom,4,"OR",2.0,MATAMAZOM_INTEGER_AMOUNT,&d,copyDouble,freeDouble,simplePrice) == MATAMAZOM_SUCCESS);

    ASSERT_TEST(mtmNewProduct(matamazom,12,"OR",2.0,MATAMAZOM_INTEGER_AMOUNT,&d,copyDouble,freeDouble,simplePrice) == MATAMAZOM_SUCCESS);
    ASSERT_TEST(mtmNewProduct(matamazom,1,"OR",2.0,MATAMAZOM_INTEGER_AMOUNT,&d,copyDouble,freeDouble,simplePrice) == MATAMAZOM_PRODUCT_ALREADY_EXIST);
    ASSERT_TEST(mtmNewProduct(matamazom,1,"OR",2.6,MATAMAZOM_INTEGER_AMOUNT,&d,copyDouble,freeDouble,simplePrice) == MATAMAZOM_INVALID_AMOUNT);
    ASSERT_TEST(mtmNewProduct(matamazom,1,"*OR",2.0,MATAMAZOM_INTEGER_AMOUNT,&d,copyDouble,freeDouble,simplePrice) == MATAMAZOM_INVALID_NAME);

    printf("TestMtmNewProduct is working!\n");


    matamazomDestroy(matamazom);
    return 1;
}




int main() {
    TestCreateOrder();
    TestCopyOrder_Faild();
    TestCopyOrder_Succses();
    TestGetIdOrder();
    TestOrderGetSize();
    TestDecreaseAndIncreaseProduct();
    TestProductExsistInOrder();
    TestOrderGetProductAmount();
    TestValidAmount();
    TestValidName();
    TestMtmNewProduct();
    printf("ALL TESTS ARE WORKING!!!:)\n");

    return 0;
}

