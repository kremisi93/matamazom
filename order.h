//
// Created by kremisi on 16/11/2019.
//

#ifndef HW1_ORDER_H
#define HW1_ORDER_H

#include <stdbool.h>
typedef struct Order_t *Order;

typedef enum OrderResult_t {
    ORDER_SUCCESS,
    ORDER_NULL_ARGUMENT,
    ORDER_OUT_OF_MEMORY,
    ORDER_PRODUCT_NOT_FOUND
} OrderResult;

/**
 * CreateOrder: create an empty order.
 * @param orderId - the id of the order.
 * @return A new order in case of success, and NULL otherwise (e.g.
 *     in case of an allocation error)
 */
Order CreateOrder(unsigned int orderId);


/**
 * GetOrderId: return the id of the given order.
 * @param orderId - the id of the order.
 * @return A order id in case of succes, else if the order is null return -1.
 */
int GetOrderId(Order order);


/**
 * FreeOrder: free a order, and all its contents, from
 * memory.
 * @param order - the order to free from memory. A NULL value is
 *     allowed, and in that case the function does nothing.
 */
void FreeOrder(Order order);


/**
 * OrderGetNumberOfProductSize: return the number of product to order.
 * @param order - the order to get the number of product.
 * @return A number of product to order in case of succes, else if the order is null return -1.
 */
int OrderGetNumberOfProductSize(Order order);


/**
 * OrderCopy: copy a given order.
 * @param order - the for copy.
 * @return A new copy order in case of success, and NULL otherwise (e.g.
 *     in case of an allocation error or order is NULL)
 */
Order OrderCopy(Order order);


/**
*  DecreaseAndIncreaseProduct: increase or decrease the amount of an product given order.
 * if 'amount' < 0 then amount decreased from the product in the order.
 * if 'amount' > 0 then amount added to the product in the order.
 * if 'amount' = 0 then nothing be done.
 * please note:
 *  1) If the amount to decrease('amount' < 0) is *larger or equal* then the amount of the product in the
 *     order, then the product is removed entirely from the order.
 *  2) If 'amount' > 0 and the product doesn't exist inside the order then the product add to the order
 *     with the amount given as argument.
 *
 * @param orderId - id of the order being modified.
 * @param productId - id of product to add to the order.
 * @param amount - amount of product to add to the order.
 * @return
 *      ORDER_NULL_ARGUMENT - if the order is null.
 *      ORDER_OUT_OF_MEMORY - if case of memory filed.
 *      ORDER_SUCCESS - in case of success.
 */
OrderResult DecreaseAndIncreaseProduct(unsigned int productId, Order order, double amount);


/**
 * ProductExsistInOrder: check if given product include in order.
 * @param order - the order to check.
 * @param productId - the id of the product.
 * @return
 *      false - if order is null of the product not exsist in order.
 *      true  - else (the product exsist).
 */
bool ProductExsistInOrder(Order order, unsigned int productId);


/**
 * OrderGetProductAmount: get amount of product in given order.
 * @param order - the order to check.
 * @param productId - the id of the product.
 * @param amount - for return the amount.
 * @return
 *      ORDER_NULL_ARGUMENT - if order is null.
 *      ORDER_PRODUCT_NOT_FOUND - if the product not found.
 *      ORDER_SUCCESS - in case of success.
 */
OrderResult OrderGetProductAmount(Order order, unsigned int productId, double* amount);

#endif //HW1_ORDER_H
