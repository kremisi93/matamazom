//
// Created by kremisi on 16/11/2019.
//
#include <stdio.h>
#include "set.h"
#include "list.h"
#include "matamazom.h"
#include "order.h"

struct Proudct_t{
    char *name;
    unsigned int productId;
    const double amountInStorge;
    const double proudctSelling;
    MtmProductData mtmProductData;
    MatamazomAmountType matamazomAmountType;
    MtmCopyData mtmCopyData;
    MtmFreeData mtmFreeData;
    MtmGetProductPrice mtmGetProductPrice;
};
typedef struct Proudct_t *Proudct;

struct Matamazom_t{
    MtmFilterProduct mtmFilterProduct;
    int bestSellingProudctId;
    List proudcts;
    Set order;
};
