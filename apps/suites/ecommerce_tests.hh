#pragma once

// Hand-written test strings for the EcommerceTests subject application.
// Extracted verbatim from the former test_libapplication.cpp.

#include "executors.hh"

namespace EcommerceTests
{
    void test01_registerBuyer(EcommerceTestExecutor &executor);
    void test02_registerSeller(EcommerceTestExecutor &executor);
    void test03_browseProducts(EcommerceTestExecutor &executor);
    void test04_buyerRegisterLogin(EcommerceTestExecutor &executor);
    void test05_sellerRegisterLogin(EcommerceTestExecutor &executor);
    void test06_sellerCreateProduct(EcommerceTestExecutor &executor);
    void test07_sellerCreateMultipleProducts(EcommerceTestExecutor &executor);
    void test08_sellerUpdateProduct(EcommerceTestExecutor &executor);
    void test09_sellerDeleteProduct(EcommerceTestExecutor &executor);
    void test10_sellerViewInventory(EcommerceTestExecutor &executor);
    void test11_multiUserBrowse(EcommerceTestExecutor &executor);
    void test12_multiUserAddToCart(EcommerceTestExecutor &executor);
    void test13_multiUserViewCart(EcommerceTestExecutor &executor);
    void test14_multiUserCreateOrder(EcommerceTestExecutor &executor);
    void test15_multiUserViewOrders(EcommerceTestExecutor &executor);
    void test16_sellerViewsOrders(EcommerceTestExecutor &executor);
    void test17_multiUserCreateReview(EcommerceTestExecutor &executor);
    void test18_completeEcommerceFlow(EcommerceTestExecutor &executor);
    void test19_multipleOrders(EcommerceTestExecutor &executor);
    void test20_sellerFullManagement(EcommerceTestExecutor &executor);
    void test21_deepWorkflow(EcommerceTestExecutor &executor);
    void test22_loginWithoutRegister(EcommerceTestExecutor &executor);
    void test23_sellerLoginWithoutRegister(EcommerceTestExecutor &executor);
    void test24_duplicateRegistration(EcommerceTestExecutor &executor);
    void test25_buyerCannotCreateProduct(EcommerceTestExecutor &executor);
    void test26_sellerCannotAddToCart(EcommerceTestExecutor &executor);
    void test27_sellerCannotCreateOrder(EcommerceTestExecutor &executor);
    void test28_addToCartNoProduct(EcommerceTestExecutor &executor);
    void test29_createOrderEmptyCart(EcommerceTestExecutor &executor);
    void test30_reviewWithoutOrder(EcommerceTestExecutor &executor);
}
