// TestGen entry point.
//
// Test selection is still done by commenting/uncommenting calls below; that is
// the pre-existing workflow and is left unchanged by the reorganization. It is
// what the GA test-string generator (src/teststring/) is intended to replace.

#include <iostream>

#include "executors.hh"
#include "suites/restaurant_tests.hh"
#include "suites/library_tests.hh"
#include "suites/ecommerce_tests.hh"

using namespace std;

// ============================================
// MAIN FUNCTION - TEST SELECTION
// ============================================

int main()
{
    try
    {
        // ========================================
        // CONFIGURATION
        // ========================================

        // Choose test mode
        // TestMode mode = TestMode::REWRITE_ONLY;
        // TestMode mode = TestMode::ORIGINAL;
         TestMode mode = TestMode::FULL_PIPELINE; // Needs backend running!

        // Backend URL (only used for FULL_PIPELINE mode)
        string backendUrl = "http://localhost:5002"; // for restaurant

         TestExecutor executor(mode, backendUrl); // for restaurant

        // ========================================
        // RUN TESTS
        // ========================================

         cout << "\n╔════════════════════════════════════════╗" << endl; // for restaurant
         cout << "║  TESTGEN - RESTAURANT TEST SUITE      ║" << endl; // for restaurant
         cout << "║  Total Tests: 25                       ║" << endl; // for restaurant
         cout << "╚════════════════════════════════════════╝\n" // for restaurant
             << endl; // for restaurant


        // RUN ALL 25 TESTS (comment out for selective testing)
         cout << "\n=== DEPTH TESTS ===" << endl; // for restaurant
        // RestaurantTests::test01_registerLogin(executor);
        // RestaurantTests::test02_loginFailure(executor);
        // RestaurantTests::test03_browseOnly(executor);

        // RestaurantTests::test04_Login(executor);
        // RestaurantTests::test05_registerOwnerAndLogin(executor);
        //  RestaurantTests::test06_registerAgentAndLogin(executor);

        // RestaurantTests::test07_loginBrowseView(executor);
        // RestaurantTests::test08_loginAndAddToCart(executor);
        // RestaurantTests::test09_loginAndReview(executor);
        // RestaurantTests::test10_reviewWithoutLogin(executor);

        // RestaurantTests::test11_fullCustomerOrder(executor);
        // RestaurantTests::test12_ownerCreateRestaurant(executor);
        // RestaurantTests::test13_cartWithoutItems(executor);

         // cout << "\n=== DEPTH 5 TESTS ===" << endl;
        // RestaurantTests::test14_customerFullWorkflow(executor);
        // RestaurantTests::test15_ownerFullSetup(executor);
        // RestaurantTests::test16_agentAssignOrder(executor);

         // cout << "\n=== DEPTH 6 TESTS ===" << endl;
        // RestaurantTests::test17_ownerManageOrder(executor);
        // RestaurantTests::test18_multipleCartAdditions(executor);
        // RestaurantTests::test19_wrongRoleAccess(executor);

         // cout << "\n=== DEPTH 7 TESTS ===" << endl;
        // RestaurantTests::test20_fullLifecycle(executor);
        // RestaurantTests::test21_ownerCompleteFlow(executor);

         // cout << "\n===More TESTS ===" << endl;
         // RestaurantTests::test22_complexOrderManagement(executor);
         // RestaurantTests::test23_invalidSequence(executor);
         // RestaurantTests::test24_deepWorkflow(executor);
          RestaurantTests::test25_registerCustomerDuplicate(executor);

         // ========================================
         // E-COMMERCE TESTS
         // ========================================

         // cout << "\n╔════════════════════════════════════════╗" << endl; //uncomment for ecom
         // cout << "║  TESTGEN - E-COMMERCE TEST SUITE       ║" << endl; //uncomment for ecom
         // cout << "║  Total Tests: 30 (21 SAT, 9 UNSAT)     ║" << endl; //uncomment for ecom
         // cout << "╚════════════════════════════════════════╝\n" //uncomment for ecom
         //      << endl; //uncomment for ecom

         // E-commerce backend URL
         // string ecommerceBackendUrl = "http://localhost:3000"; //uncomment for ecom
         // EcommerceTestExecutor ecommerceExecutor(mode, ecommerceBackendUrl); //uncomment for ecom

         // === SAT TESTS ===
         // EcommerceTests::test01_registerBuyer(ecommerceExecutor);
         // EcommerceTests::test02_registerSeller(ecommerceExecutor);
         // EcommerceTests::test03_browseProducts(ecommerceExecutor);
         // EcommerceTests::test04_buyerRegisterLogin(ecommerceExecutor);
         // EcommerceTests::test05_sellerRegisterLogin(ecommerceExecutor);
         // EcommerceTests::test06_sellerCreateProduct(ecommerceExecutor);
         // EcommerceTests::test07_sellerCreateMultipleProducts(ecommerceExecutor);
         // EcommerceTests::test08_sellerUpdateProduct(ecommerceExecutor);
         // EcommerceTests::test09_sellerDeleteProduct(ecommerceExecutor);
         // EcommerceTests::test10_sellerViewInventory(ecommerceExecutor);
         // EcommerceTests::test11_multiUserBrowse(ecommerceExecutor);
         // EcommerceTests::test12_multiUserAddToCart(ecommerceExecutor);
         // EcommerceTests::test13_multiUserViewCart(ecommerceExecutor);
         // EcommerceTests::test14_multiUserCreateOrder(ecommerceExecutor);
         // EcommerceTests::test15_multiUserViewOrders(ecommerceExecutor);
         // EcommerceTests::test16_sellerViewsOrders(ecommerceExecutor);
         // EcommerceTests::test17_multiUserCreateReview(ecommerceExecutor);
         // EcommerceTests::test18_completeEcommerceFlow(ecommerceExecutor);
         // EcommerceTests::test19_multipleOrders(ecommerceExecutor);
         // EcommerceTests::test20_sellerFullManagement(ecommerceExecutor);
         // EcommerceTests::test21_deepWorkflow(ecommerceExecutor);

         // === UNSAT TESTS ===
         // EcommerceTests::test22_loginWithoutRegister(ecommerceExecutor);
         // EcommerceTests::test23_sellerLoginWithoutRegister(ecommerceExecutor);
         // EcommerceTests::test24_duplicateRegistration(ecommerceExecutor);
         // EcommerceTests::test25_buyerCannotCreateProduct(ecommerceExecutor);
         // EcommerceTests::test26_sellerCannotAddToCart(ecommerceExecutor);
         // EcommerceTests::test27_sellerCannotCreateOrder(ecommerceExecutor);
         // EcommerceTests::test28_addToCartNoProduct(ecommerceExecutor);
         // EcommerceTests::test29_createOrderEmptyCart(ecommerceExecutor);
         // EcommerceTests::test30_reviewWithoutOrder(ecommerceExecutor);

         // ========================================
         // CONFIGURATION
         // ========================================

         // Choose test mode
         // TestMode mode = TestMode::REWRITE_ONLY;
         // TestMode mode = TestMode::ORIGINAL;
         // TestMode mode = TestMode::FULL_PIPELINE; // Needs backend running!

         // Backend URL - Spring Boot default port
         // string backendUrl = "http://localhost:8080";

         // LibraryTestExecutor executor(mode, backendUrl);

         // ========================================
         // RUN TESTS
         // ========================================

         // cout << "\n╔════════════════════════════════════════╗" << endl;
         // cout << "║  TESTGEN - LIBRARY TEST SUITE          ║" << endl;
         // cout << "║  Total Tests: 25                       ║" << endl;
         // cout << "╚════════════════════════════════════════╝\n"
         //     << endl;

         // ========== BASIC TESTS ==========
         // cout << "\n=== BASIC SINGLE OPERATION TESTS ===" << endl;
         // LibraryTests::test01_getAllBooks(executor);
         // LibraryTests::test02_getAllStudents(executor);
         // LibraryTests::test03_saveBook(executor);
         // LibraryTests::test04_saveStudent(executor);

         // ========== DEPTH 2 TESTS ==========
         // cout << "\n=== DEPTH 2 TESTS ===" << endl;
         // LibraryTests::test05_saveAndGetBook(executor);
         // LibraryTests::test06_saveAndGetStudent(executor);
         // LibraryTests::test07_saveBookTwice(executor);
         // LibraryTests::test08_getBookNotFound(executor);

         // ========== CRUD TESTS ==========
         // cout << "\n=== CRUD TESTS ===" << endl;
         // LibraryTests::test09_bookCRUD(executor);
         // LibraryTests::test10_studentCRUD(executor);
         // LibraryTests::test11_createBookAndStudent(executor);

         // ========== BORROW FLOW TESTS ==========
         // cout << "\n=== BORROW FLOW TESTS ===" << endl;
         // LibraryTests::test12_createRequest(executor);
         // LibraryTests::test13_acceptRequest(executor);
         // LibraryTests::test14_fullBorrowReturn(executor);
         // LibraryTests::test15_directLoan(executor);
         // LibraryTests::test16_rejectRequest(executor);

         // ========== NEGATIVE TESTS (UNSAT) ==========
         // cout << "\n=== NEGATIVE TESTS (Expected UNSAT) ===" << endl;
         // LibraryTests::test17_requestWithoutBook(executor);
         // LibraryTests::test18_requestWithoutStudent(executor);
         // LibraryTests::test19_acceptWithoutRequest(executor);
         // LibraryTests::test20_returnWithoutLoan(executor);

         // ========== COMPLEX WORKFLOW TESTS ==========
         // cout << "\n=== COMPLEX WORKFLOW TESTS ===" << endl;
         // LibraryTests::test21_multipleBooks(executor);
         // LibraryTests::test22_multipleStudents(executor);
         // LibraryTests::test23_multipleBorrowings(executor);
         // LibraryTests::test24_fullLibraryWorkflow(executor);
         // LibraryTests::test25_complexScenario(executor);

         // cout << "\n╔════════════════════════════════════════╗" << endl;
         // cout << "║  ALL TESTS COMPLETED                   ║" << endl;
         // cout << "╚════════════════════════════════════════╝\n"
         //     << endl;
    }
    catch (const exception &e)
    {
        cerr << "\n❌ FATAL ERROR: " << e.what() << endl;
        return 1;
    }

    return 0;
}