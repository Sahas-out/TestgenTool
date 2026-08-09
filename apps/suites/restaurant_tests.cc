#include "suites/restaurant_tests.hh"
#include "specs/RestaurantSpec.hh"

namespace RestaurantTests
{

    // ========================================
    // DEPTH 1: Single API Call Tests
    // ========================================

    void test01_registerLogin(TestExecutor &executor)
    {
        executor.runTest(
            "Test 01: Register Customer-->Login (Depth=2)",
            makeRestaurantSpec(),
            {"registerCustomerOk", "loginCustomerOk"});
    }

    void test02_loginFailure(TestExecutor &executor)
    {
        executor.runTest(
            "Test 02: loginCustomerErr (Depth=1)",
            makeRestaurantSpec(), {"loginCustomerErr"}
            // {"loginCustomerOk"} // Should fail: user not in U
        );
    }

    void test03_browseOnly(TestExecutor &executor)
    {
        executor.runTest(
            "Test 03: registerCustomerOk → loginWrongPasswordErr (Depth=2)",
            makeRestaurantSpec(),
            {"registerCustomerOk", "loginWrongPasswordErr"}
            // {"browseRestaurantsOk"} // Public API, no auth needed
        );
    }

    // ========================================
    // DEPTH 2: Two API Call Tests
    // ========================================

    void test04_Login(TestExecutor &executor)
    {
        executor.runTest(
            "Test 04: loginCustomerOk (Depth=1, Expected UNSAT)",
            makeRestaurantSpec(),
            {"loginCustomerOk"}
            // {"loginOk"} // should be UNSAT
        );
    }

    void test05_registerOwnerAndLogin(TestExecutor &executor)
    {
        executor.runTest(
            "Test 05: registerCustomerOk → loginCustomerOk → addToCartRestaurantOk (Depth=3, Expected UNSAT)",
            makeRestaurantSpec(),
            {"registerCustomerOk", "loginCustomerOk", "addToCartRestaurantOk"} // unsat
            // {"registerOwnerOk", "loginOk"} // should be SAT
        );
    }

    void test06_registerAgentAndLogin(TestExecutor &executor)
    {
        executor.runTest(
            "Test 06: registerOwnerOk → loginOwnerOk → createRestaurantOk → placeOrderOk (Depth=4, Expected UNSAT)",
            makeRestaurantSpec(),
            {"registerOwnerOk", "loginOwnerOk", "createRestaurantOk", "placeOrderOk"}
            // {"registerAgentOk", "loginOk"} // should be SAT
        );
    }

    // ========================================
    // DEPTH 4: Three API Call Tests
    // ========================================

    void test07_loginBrowseView(TestExecutor &executor)
    {
        executor.runTest(
            "Test 07: registerOwnerOk → loginOwnerOk → createRestaurantOk → registerCustomerOk → loginCustomerOk → browseRestaurantsOk → viewMenuOk (Depth=7)",
            makeRestaurantSpec(),
            {"registerOwnerOk",     // 1. Owner registers
             "loginOwnerOk",        // 2. Owner logs in
             "createRestaurantOk",  // 3. Owner creates restaurant
             "registerCustomerOk",  // 4. Customer registers
             "loginCustomerOk",     // 5. Customer logs in
             "browseRestaurantsOk", // 6. Customer browses
             "viewMenuOk"}          // should be SAT
        );
    }

    void test08_loginAndAddToCart(TestExecutor &executor)
    {
        executor.runTest(
            "Test 08: registerOwnerOk → loginOwnerOk → createRestaurantOk → addMenuItemOk → registerCustomerOk → loginCustomerOk → browseRestaurantsOk → viewMenuOk → addToCartRestaurantOk (Depth=9)",
            makeRestaurantSpec(),
            {"registerOwnerOk",       // 1. Owner registers
             "loginOwnerOk",          // 2. Owner logs in
             "createRestaurantOk",    // 3. Owner creates restaurant
             "addMenuItemOk",         // 4. Owner adds menu item
             "registerCustomerOk",    // 5. Customer registers
             "loginCustomerOk",       // 6. Customer logs in
             "browseRestaurantsOk",   // 7. Customer browses
             "viewMenuOk",            // 8. Customer views menu
             "addToCartRestaurantOk"} // 9. Customer adds to cart
        );
    }

    void test09_loginAndReview(TestExecutor &executor)
    {
        executor.runTest(
            "Test 09: registerOwnerOk → loginOwnerOk → createRestaurantOk → addMenuItemOk → registerCustomerOk → loginCustomerOk → browseRestaurantsOk → viewMenuOk → addToCartRestaurantOk → placeOrderOk (Depth=10)",
            makeRestaurantSpec(),
            {"registerOwnerOk",     // 1. Owner registers
             "loginOwnerOk",        // 2. Owner logs in
             "createRestaurantOk",  // 3. Owner creates restaurant
             "addMenuItemOk",       // 4. Owner adds menu item
             "registerCustomerOk",  // 5. Customer registers
             "loginCustomerOk",     // 6. Customer logs in
             "browseRestaurantsOk", // 7. Customer browses
             "viewMenuOk",          // 8. Customer views menu
             "addToCartRestaurantOk",
             "placeOrderOk"});
    }

    void test10_reviewWithoutLogin(TestExecutor &executor)
    {
        executor.runTest(
            "Test 10: Full Order Lifecycle with Review (Depth=19)",
            makeRestaurantSpec(),
            {"registerOwnerOk",          // 1. Owner registers
             "loginOwnerOk",             // 2. Owner logs in
             "createRestaurantOk",       // 3. Owner creates restaurant
             "addMenuItemOk",            // 4. Owner adds menu item
             "registerCustomerOk",       // 5. Customer registers
             "loginCustomerOk",          // 6. Customer logs in
             "browseRestaurantsOk",      // 7. Customer browses
             "viewMenuOk",               // 8. Customer views menu
             "addToCartRestaurantOk",    // 9. Customer adds to cart
             "placeOrderOk",             // 10. Customer places order (status: placed)
             "registerAgentOk",          // 11. Delivery agent registers
             "loginAgentOk",             // 12. Delivery agent logs in
             "updateOrderStatusOwnerOk", // 13. Owner accepts order (status: accepted)
             "updateOrderStatusOwnerOk", // 14. Owner marks preparing (status: preparing)
             "updateOrderStatusOwnerOk", // 15. Owner marks ready (status: ready)
             "assignOrderOk",            // 16. Owner assigns delivery agent
             "updateOrderStatusAgentOk", // 17. Agent picks up (status: picked_up)
             "updateOrderStatusAgentOk", // 18. Agent delivers (status: delivered)
             "leaveReviewOk"});
    }

    void test11_fullCustomerOrder(TestExecutor &executor)
    {
        executor.runTest(
            "Test 11: registerOwnerOk → loginOwnerOk → createRestaurantOk (Depth=3)",
            makeRestaurantSpec(),
            {"registerOwnerOk", "loginOwnerOk", "createRestaurantOk"}
            // {"registerCustomerOk", "loginOk", "addToCartOk", "placeOrderOk"} // should fail no browse restaurants
        );
    }

    void test12_ownerCreateRestaurant(TestExecutor &executor)
    {
        executor.runTest(
            "Test 12: registerOwnerOk → loginOwnerOk → createRestaurantOk → addMenuItemOk → addMenuItemOk → addMenuItemOk (Depth=6)",
            makeRestaurantSpec(),
            {"registerOwnerOk", "loginOwnerOk", "createRestaurantOk",
             "addMenuItemOk", "addMenuItemOk", "addMenuItemOk"}
            // {"registerOwnerOk", "loginOk", "createRestaurantOk", "addMenuItemOk"}
        );
    }

    void test13_cartWithoutItems(TestExecutor &executor)
    {
        executor.runTest(
            "Test 13: registerOwnerOk → loginOwnerOk → createRestaurantOk → addMenuItemOk → registerCustomerOk → loginCustomerOk → browseRestaurantsOk → viewMenuOk → addToCartRestaurantOk (Depth=9)",
            makeRestaurantSpec(),
            {"registerOwnerOk", "loginOwnerOk", "createRestaurantOk", "addMenuItemOk",
             "registerCustomerOk", "loginCustomerOk", "browseRestaurantsOk", "viewMenuOk",
             "addToCartRestaurantOk"}
            // {"registerCustomerOk", "loginOk", "placeOrderErr"} // Should fail: cart empty
        );
    }

    // ========================================
    // DEPTH 5: Five API Call Tests
    // ========================================

    void test14_customerFullWorkflow(TestExecutor &executor)
    {
        executor.runTest(
            "Test 14: registerOwnerOk → loginOwnerOk → createRestaurantOk → addMenuItemOk → registerCustomerOk → loginCustomerOk → browseRestaurantsOk → viewMenuOk → addToCartRestaurantOk → placeOrderOk (Depth=10)",
            makeRestaurantSpec(),
            {"registerOwnerOk", "loginOwnerOk", "createRestaurantOk", "addMenuItemOk",
             "registerCustomerOk", "loginCustomerOk", "browseRestaurantsOk", "viewMenuOk",
             "addToCartRestaurantOk", "placeOrderOk"}
            // {"registerCustomerOk", "loginOk", "browseRestaurantsOk", "addToCartRestaurantOk", "placeOrderOk"}
        );
    }

    void test15_ownerFullSetup(TestExecutor &executor)
    {
        executor.runTest(
            "Test 15: registerOwnerOk → loginOwnerOk → createRestaurantOk → addMenuItemOk → registerCustomerOk → loginCustomerOk → browseRestaurantsOk → viewMenuOk → addToCartRestaurantOk → placeOrderOk → updateOrderStatusOwnerOk x3 (Depth=13)",
            makeRestaurantSpec(),
            {"registerOwnerOk", "loginOwnerOk", "createRestaurantOk", "addMenuItemOk",
             "registerCustomerOk", "loginCustomerOk", "browseRestaurantsOk", "viewMenuOk",
             "addToCartRestaurantOk", "placeOrderOk",
             "updateOrderStatusOwnerOk", // accepted
             "updateOrderStatusOwnerOk", // preparing
             "updateOrderStatusOwnerOk"}
            // {"registerOwnerOk", "loginOk", "createRestaurantOk", "addMenuItemOk", "addMenuItemOk"}
        );
    }

    void test16_agentAssignOrder(TestExecutor &executor)
    {
        executor.runTest(
            "Test 16: registerCustomerOk → registerCustomerOk (Depth=2, Expected UNSAT - Duplicate Registration)",
            makeRestaurantSpec(),
            {"registerOwnerOk", "registerOwnerOk"}
            //{"registerAgentOk", "loginOk", "placeOrderOk", "assignOrderOk", "updateOrderStatusAgentOk"} // delivery agent can't place order
        );
    }

    // ========================================
    // DEPTH 6: Six API Call Tests
    // ========================================

    void test17_ownerManageOrder(TestExecutor &executor)
    {
        executor.runTest(
            "Test 17: registerOwnerOk → loginOwnerOk → createRestaurantOk → addMenuItemOk → registerCustomerOk → loginCustomerOk → browseRestaurantsOk → viewMenuOk → addToCartRestaurantOk → placeOrderOk → registerAgentOk → loginAgentOk → updateOrderStatusAgentOk (Depth=13, Expected UNSAT)",
            makeRestaurantSpec(),
            {"registerOwnerOk", "loginOwnerOk", "createRestaurantOk", "addMenuItemOk",
             "registerCustomerOk", "loginCustomerOk", "browseRestaurantsOk", "viewMenuOk",
             "addToCartRestaurantOk", "placeOrderOk",
             "registerAgentOk", "loginAgentOk",
             "updateOrderStatusAgentOk"}
            //{"registerOwnerOk", "loginOk", "createRestaurantOk", "placeOrderOk"} // should be UNSAT, owner can't place order
        );
    }

    void test18_multipleCartAdditions(TestExecutor &executor)
    {
        executor.runTest(
            "Test 18: loginCustomerOk → browseRestaurantsOk → addToCartRestaurantOk x3 → placeOrderOk (Depth=6, Expected UNSAT - No Registration)",
            makeRestaurantSpec(),
            {"loginCustomerOk", "browseRestaurantsOk", "addToCartRestaurantOk", "addToCartRestaurantOk", "addToCartRestaurantOk", "placeOrderOk"} // should return unsat as no registration
        );
    }

    void test19_wrongRoleAccess(TestExecutor &executor)
    {
        executor.runTest(
            "Test 19: registerCustomerOk → loginCustomerOk → createRestaurantCustomerErr (Depth=3, Expected UNSAT - Customer Can't Create Restaurant)",
            makeRestaurantSpec(),
            {"registerCustomerOk", "loginCustomerOk", "createRestaurantCustomerErr"});
    }

    // ========================================
    // DEPTH 7: Seven API Call Tests
    // ========================================

    void test20_fullLifecycle(TestExecutor &executor)
    {
        executor.runTest(
            "Test 20: registerCustomerOk → loginCustomerOk → browseRestaurantsOk → viewMenuOk → addToCartRestaurantOk → placeOrderOk → leaveReviewOk (Depth=7, Expected UNSAT)",
            makeRestaurantSpec(),
            {"registerCustomerOk", "loginCustomerOk", "browseRestaurantsOk", "viewMenuOk",
             "addToCartRestaurantOk", "placeOrderOk", "leaveReviewOk"});
    }

    void test21_ownerCompleteFlow(TestExecutor &executor)
    {
        executor.runTest(
            "Test 21: registerOwnerOk → loginOwnerOk → createRestaurantOk → addMenuItemOk x3 (Depth=6)",
            makeRestaurantSpec(),
            {"registerOwnerOk", "loginOwnerOk", "createRestaurantOk",
             "addMenuItemOk", "addMenuItemOk", "addMenuItemOk"});
    }

    

    void test22_complexOrderManagement(TestExecutor &executor)
    {
        executor.runTest(
            "Test 23: registerOwnerOk → loginOwnerOk → createRestaurantOk → addMenuItemOk → registerCustomerOk → loginCustomerOk → browseRestaurantsOk → viewMenuOk → addToCartRestaurantOk → placeOrderOk (Depth=10)",
            makeRestaurantSpec(),
            {"registerOwnerOk",       // 1. Owner registers
             "loginOwnerOk",          // 2. Owner logs in
             "createRestaurantOk",    // 3. Owner creates restaurant
             "addMenuItemOk",         // 4. Owner adds menu item
             "registerCustomerOk",    // 5. Customer registers
             "loginCustomerOk",       // 6. Customer logs in
             "browseRestaurantsOk",   // 7. Customer browses
             "viewMenuOk",            // 8. Customer views menu
             "addToCartRestaurantOk", // 9. Customer adds to cart
             "placeOrderOk"});
    }

    void test23_invalidSequence(TestExecutor &executor)
    {
        executor.runTest(
            "Test 24: registerCustomerOk → loginCustomerOk → leaveReviewOk (Depth=3, Expected UNSAT - Review Before Order)",
            makeRestaurantSpec(),
            {"registerCustomerOk", "loginCustomerOk", "leaveReviewOk"}
            // Should fail: can't review restaurant you haven't ordered from
        );
    }

    void test24_deepWorkflow(TestExecutor &executor)
    {
        executor.runTest(
            "Test 25: registerOwnerOk → loginOwnerOk → createRestaurantOk → addMenuItemOk x5 → updateOrderStatusOwnerOk x2 (Depth=10, Expected UNSAT)",
            makeRestaurantSpec(),
            {"registerOwnerOk", "loginOwnerOk", "createRestaurantOk",
             "addMenuItemOk", "addMenuItemOk", "addMenuItemOk",
             "addMenuItemOk", "addMenuItemOk", "updateOrderStatusOwnerOk", "updateOrderStatusOwnerOk"});
    }

    void test25_registerCustomerDuplicate(TestExecutor &executor)
    {
        executor.runTest(
            "Test 26: registerCustomerOk → registerCustomerOk (Depth=2, Expected UNSAT - Duplicate Email)",
            makeRestaurantSpec(),
            {"registerCustomerOk", "registerCustomerOk"}); // edge case should return unsat
    }
    
}
