#include "suites/ecommerce_tests.hh"
#include "specs/EcommerceSpec.hh"

namespace EcommerceTests
{
    // ╔════════════════════════════════════════════════════════════╗
    // ║  SAT TESTS (21 tests = 70%)                                ║
    // ╚════════════════════════════════════════════════════════════╝

    // SAT-01: Register Buyer (Depth=1)
    void test01_registerBuyer(EcommerceTestExecutor &executor)
    {
        executor.runTest(
            "[SAT] Test 01: Register Buyer (Depth=1)",
            makeEcommerceSpec(),
            {"registerBuyerOk"});
    }

    // SAT-02: Register Seller (Depth=1)
    void test02_registerSeller(EcommerceTestExecutor &executor)
    {
        executor.runTest(
            "[SAT] Test 02: Register Seller (Depth=1)",
            makeEcommerceSpec(),
            {"registerSellerOk"});
    }

    // SAT-03: Browse Products - Public API (Depth=1)
    void test03_browseProducts(EcommerceTestExecutor &executor)
    {
        executor.runTest(
            "[SAT] Test 03: Browse All Products - Public (Depth=1)",
            makeEcommerceSpec(),
            {"getAllProductsOk"});
    }

    // SAT-04: Register Buyer → Login (Depth=2)
    void test04_buyerRegisterLogin(EcommerceTestExecutor &executor)
    {
        executor.runTest(
            "[SAT] Test 04: Register Buyer → Login (Depth=2)",
            makeEcommerceSpec(),
            {"registerBuyerOk", "loginBuyerOk"});
    }

    // SAT-05: Register Seller → Login (Depth=2)
    void test05_sellerRegisterLogin(EcommerceTestExecutor &executor)
    {
        executor.runTest(
            "[SAT] Test 05: Register Seller → Login (Depth=2)",
            makeEcommerceSpec(),
            {"registerSellerOk", "loginSellerOk"});
    }

    // SAT-06: Seller Creates Product (Depth=3)
    void test06_sellerCreateProduct(EcommerceTestExecutor &executor)
    {
        executor.runTest(
            "[SAT] Test 06: Seller Creates Product (Depth=3)",
            makeEcommerceSpec(),
            {"registerSellerOk", "loginSellerOk", "createProductOk"});
    }

    // SAT-07: Seller Creates Multiple Products (Depth=5)
    void test07_sellerCreateMultipleProducts(EcommerceTestExecutor &executor)
    {
        executor.runTest(
            "[SAT] Test 07: Seller Creates 3 Products (Depth=5)",
            makeEcommerceSpec(),
            {"registerSellerOk", "loginSellerOk",
             "createProductOk", "createProductOk", "createProductOk"});
    }

    // SAT-08: Seller Creates → Updates Product (Depth=4)
    void test08_sellerUpdateProduct(EcommerceTestExecutor &executor)
    {
        executor.runTest(
            "[SAT] Test 08: Seller Creates → Updates Product (Depth=4)",
            makeEcommerceSpec(),
            {"registerSellerOk", "loginSellerOk", "createProductOk", "updateProductOk"});
    }

    // SAT-09: Seller Creates → Deletes Product (Depth=4)
    void test09_sellerDeleteProduct(EcommerceTestExecutor &executor)
    {
        executor.runTest(
            "[SAT] Test 09: Seller Creates → Deletes Product (Depth=4)",
            makeEcommerceSpec(),
            {"registerSellerOk", "loginSellerOk", "createProductOk", "deleteProductOk"});
    }

    // SAT-10: Seller Views Own Inventory (Depth=4)
    void test10_sellerViewInventory(EcommerceTestExecutor &executor)
    {
        executor.runTest(
            "[SAT] Test 10: Seller Creates → Views Inventory (Depth=4)",
            makeEcommerceSpec(),
            {"registerSellerOk", "loginSellerOk", "createProductOk", "getSellerProductsOk"});
    }

    // SAT-11: Seller creates product → Buyer browses (Depth=6)
    void test11_multiUserBrowse(EcommerceTestExecutor &executor)
    {
        executor.runTest(
            "[SAT] Test 11: Seller Setup → Buyer Browses (Depth=6)",
            makeEcommerceSpec(),
            {"registerSellerOk", "loginSellerOk", "createProductOk",
             "registerBuyerOk", "loginBuyerOk", "getAllProductsOk"});
    }

    // SAT-12: Seller creates product → Buyer adds to cart (Depth=7)
    void test12_multiUserAddToCart(EcommerceTestExecutor &executor)
    {
        executor.runTest(
            "[SAT] Test 12: Seller Setup → Buyer Adds to Cart (Depth=7)",
            makeEcommerceSpec(),
            {"registerSellerOk", "loginSellerOk", "createProductOk",
             "registerBuyerOk", "loginBuyerOk",
             "addToCartOk"});
    }

    // SAT-13: Seller creates product → Buyer views cart (Depth=8)
    void test13_multiUserViewCart(EcommerceTestExecutor &executor)
    {
        executor.runTest(
            "[SAT] Test 13: Seller Setup → Buyer Adds & Views Cart (Depth=8)",
            makeEcommerceSpec(),
            {"registerSellerOk", "loginSellerOk", "createProductOk",
             "registerBuyerOk", "loginBuyerOk",
             "addToCartOk", "getCartOk"});
    }

    // SAT-14: Seller creates product → Buyer creates order (Depth=8)
    void test14_multiUserCreateOrder(EcommerceTestExecutor &executor)
    {
        executor.runTest(
            "[SAT] Test 14: Seller Setup → Buyer Creates Order (Depth=8)",
            makeEcommerceSpec(),
            {"registerSellerOk", "loginSellerOk", "createProductOk",
             "registerBuyerOk", "loginBuyerOk",
             "addToCartOk", "createOrderOk"});
    }

    // SAT-15: Full order flow → Buyer views orders (Depth=9)
    void test15_multiUserViewOrders(EcommerceTestExecutor &executor)
    {
        executor.runTest(
            "[SAT] Test 15: Full Order Flow → Buyer Views Orders (Depth=9)",
            makeEcommerceSpec(),
            {"registerSellerOk", "loginSellerOk", "createProductOk",
             "registerBuyerOk", "loginBuyerOk",
             "addToCartOk", "createOrderOk", "getBuyerOrdersOk"});
    }

    // SAT-16: Full order flow → Seller views orders (Depth=9)
    void test16_sellerViewsOrders(EcommerceTestExecutor &executor)
    {
        executor.runTest(
            "[SAT] Test 16: Full Order → Seller Views Orders (Depth=9)",
            makeEcommerceSpec(),
            {"registerSellerOk", "loginSellerOk", "createProductOk",
             "registerBuyerOk", "loginBuyerOk",
             "addToCartOk", "createOrderOk", "getSellerOrdersOk"});
    }

    // SAT-17: Full flow → Buyer creates review (Depth=9)
    void test17_multiUserCreateReview(EcommerceTestExecutor &executor)
    {
        executor.runTest(
            "[SAT] Test 17: Full Order → Buyer Creates Review (Depth=9)",
            makeEcommerceSpec(),
            {"registerSellerOk", "loginSellerOk", "createProductOk",
             "registerBuyerOk", "loginBuyerOk",
             "addToCartOk", "createOrderOk", "createReviewOk"});
    }

    // SAT-18: Complete E-Commerce Journey (Depth=12)
    void test18_completeEcommerceFlow(EcommerceTestExecutor &executor)
    {
        executor.runTest(
            "[SAT] Test 18: Complete E-Commerce Flow (Depth=12)",
            makeEcommerceSpec(),
            {"registerSellerOk", "loginSellerOk", "createProductOk", "createProductOk",
             "registerBuyerOk", "loginBuyerOk",
             "getAllProductsOk", "addToCartOk", "addToCartOk",
             "createOrderOk", "getBuyerOrdersOk", "createReviewOk"});
    }

    // SAT-19: Multiple Orders by Same Buyer (Depth=12)
    void test19_multipleOrders(EcommerceTestExecutor &executor)
    {
        executor.runTest(
            "[SAT] Test 19: Buyer Places Multiple Orders (Depth=12)",
            makeEcommerceSpec(),
            {"registerSellerOk", "loginSellerOk",
             "createProductOk", "createProductOk", "createProductOk",
             "registerBuyerOk", "loginBuyerOk",
             "addToCartOk", "createOrderOk",
             "addToCartOk", "createOrderOk",
             "getBuyerOrdersOk"});
    }

    // SAT-20: Seller Full Product Management (Depth=10)
    void test20_sellerFullManagement(EcommerceTestExecutor &executor)
    {
        executor.runTest(
            "[SAT] Test 20: Seller Full Product Management (Depth=10)",
            makeEcommerceSpec(),
            {"registerSellerOk", "loginSellerOk",
             "createProductOk", "createProductOk", "createProductOk",
             "updateProductOk", "updateProductOk",
             "deleteProductOk",
             "getSellerProductsOk", "getSellerOrdersOk"});
    }

    // SAT-21: Deep E-Commerce Workflow (Depth=15)
    void test21_deepWorkflow(EcommerceTestExecutor &executor)
    {
        executor.runTest(
            "[SAT] Test 21: Deep E-Commerce Workflow (Depth=15)",
            makeEcommerceSpec(),
            {"registerSellerOk", "loginSellerOk",
             "createProductOk", "createProductOk", "createProductOk",
             "registerBuyerOk", "loginBuyerOk",
             "getAllProductsOk", "addToCartOk", "addToCartOk",
             "createOrderOk", "createReviewOk",
             "addToCartOk", "createOrderOk", "createReviewOk"});
    }

    // ╔════════════════════════════════════════════════════════════╗
    // ║  UNSAT TESTS (9 tests = 30%)                               ║
    // ╚════════════════════════════════════════════════════════════╝

    // UNSAT-01: Login without registration (Depth=1)
    void test22_loginWithoutRegister(EcommerceTestExecutor &executor)
    {
        executor.runTest(
            "[UNSAT] Test 22: Buyer Login Without Registration (Depth=1)",
            makeEcommerceSpec(),
            {"loginBuyerOk"});
    }

    // UNSAT-02: Seller login without registration (Depth=1)
    void test23_sellerLoginWithoutRegister(EcommerceTestExecutor &executor)
    {
        executor.runTest(
            "[UNSAT] Test 23: Seller Login Without Registration (Depth=1)",
            makeEcommerceSpec(),
            {"loginSellerOk"});
    }

    // UNSAT-03: Duplicate registration (Depth=2)
    void test24_duplicateRegistration(EcommerceTestExecutor &executor)
    {
        executor.runTest(
            "[UNSAT] Test 24: Duplicate Buyer Registration (Depth=2)",
            makeEcommerceSpec(),
            {"registerBuyerOk", "registerBuyerOk"});
    }

    // UNSAT-04: Buyer tries to create product (Depth=3)
    void test25_buyerCannotCreateProduct(EcommerceTestExecutor &executor)
    {
        executor.runTest(
            "[UNSAT] Test 25: Buyer Cannot Create Product (Depth=3)",
            makeEcommerceSpec(),
            {"registerBuyerOk", "loginBuyerOk", "createProductOk"});
    }

    // UNSAT-05: Seller tries to add to cart (Depth=3)
    void test26_sellerCannotAddToCart(EcommerceTestExecutor &executor)
    {
        executor.runTest(
            "[UNSAT] Test 26: Seller Cannot Add to Cart (Depth=3)",
            makeEcommerceSpec(),
            {"registerSellerOk", "loginSellerOk", "addToCartOk"});
    }

    // UNSAT-06: Seller tries to create order (Depth=4)
    void test27_sellerCannotCreateOrder(EcommerceTestExecutor &executor)
    {
        executor.runTest(
            "[UNSAT] Test 27: Seller Cannot Create Order (Depth=4)",
            makeEcommerceSpec(),
            {"registerSellerOk", "loginSellerOk", "createProductOk", "createOrderOk"});
    }

    // UNSAT-07: Buyer adds to cart without product existing (Depth=3)
    void test28_addToCartNoProduct(EcommerceTestExecutor &executor)
    {
        executor.runTest(
            "[UNSAT] Test 28: Add to Cart - No Product Exists (Depth=3)",
            makeEcommerceSpec(),
            {"registerBuyerOk", "loginBuyerOk", "addToCartOk"});
    }

    // UNSAT-08: Create order without items in cart (Depth=6)
    void test29_createOrderEmptyCart(EcommerceTestExecutor &executor)
    {
        executor.runTest(
            "[UNSAT] Test 29: Create Order - Empty Cart (Depth=6)",
            makeEcommerceSpec(),
            {"registerSellerOk", "loginSellerOk", "createProductOk",
             "registerBuyerOk", "loginBuyerOk",
             "createOrderOk"});
    }

    // UNSAT-09: Create review without placing order (Depth=7)
    void test30_reviewWithoutOrder(EcommerceTestExecutor &executor)
    {
        executor.runTest(
            "[UNSAT] Test 30: Create Review - No Order Placed (Depth=7)",
            makeEcommerceSpec(),
            {"registerSellerOk", "loginSellerOk", "createProductOk",
             "registerBuyerOk", "loginBuyerOk",
             "addToCartOk", "createReviewOk"});
    }
}
