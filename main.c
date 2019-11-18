#include <stdio.h>
#include "order.h"
#include "test_utilities.h"
#include "set.h"

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
    int batelIdOrder = 204571509;
    int idProduct = 1;
    double  amount = 100;
    double res = -1;
    Order order = CreateOrder(batelIdOrder);
    ASSERT_TEST(OrderGetProductAmount(order, idProduct, &res) == ORDER_PRODUCT_NOT_FOUND);
    ASSERT_TEST(res = -1);
    ASSERT_TEST(DecreaseAndIncreaseProduct(idProduct,order,amount) == ORDER_SUCCESS);
    ASSERT_TEST(OrderGetProductAmount(NULL, idProduct, &res) == ORDER_NULL_ARGUMENT);
    ASSERT_TEST(OrderGetProductAmount(order, idProduct, NULL) == ORDER_NULL_ARGUMENT);
    ASSERT_TEST(OrderGetProductAmount(order, idProduct + 1, &res) == ORDER_PRODUCT_NOT_FOUND);
    ASSERT_TEST(OrderGetProductAmount(order, idProduct, &res) == ORDER_SUCCESS);
    ASSERT_TEST(res == amount);
    FreeOrder(order);
    printf("TestOrderGetProductAmount is working!\n");
    return 1;
}

int TestProductExsistInOrder() {
    int batelIdOrder = 204571509;
    int idProduct = 1;
    double  amount = 100;
    Order order = CreateOrder(batelIdOrder);
    ASSERT_TEST(ProductExsistInOrder(order, idProduct) == false);
    ASSERT_TEST(ProductExsistInOrder(NULL, idProduct) == false);
    ASSERT_TEST(DecreaseAndIncreaseProduct(idProduct,order,amount) == ORDER_SUCCESS);
    ASSERT_TEST(ProductExsistInOrder(NULL, idProduct) == false);
    ASSERT_TEST(ProductExsistInOrder(order, idProduct + 1) == false);
    ASSERT_TEST(ProductExsistInOrder(order, idProduct) == true);
    FreeOrder(order);
    printf("TestProductExsistInOrder is working!\n");
    return 1;
}

int TestDecreaseAndIncreaseProduct() {
    int batelId = 204571509;
    Order order = CreateOrder(batelId);
    //TODO

    FreeOrder(order);
    printf("TestDecreaseAndIncreaseProduct is working!\n");
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
    printf("ALL TESTS ARE WORKING!!!:)\n");

    return 0;
}
