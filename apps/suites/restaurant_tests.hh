#pragma once

// Hand-written test strings for the RestaurantTests subject application.
// Extracted verbatim from the former test_libapplication.cpp.

#include "executors.hh"

namespace RestaurantTests
{
    void test01_registerLogin(TestExecutor &executor);
    void test02_loginFailure(TestExecutor &executor);
    void test03_browseOnly(TestExecutor &executor);
    void test04_Login(TestExecutor &executor);
    void test05_registerOwnerAndLogin(TestExecutor &executor);
    void test06_registerAgentAndLogin(TestExecutor &executor);
    void test07_loginBrowseView(TestExecutor &executor);
    void test08_loginAndAddToCart(TestExecutor &executor);
    void test09_loginAndReview(TestExecutor &executor);
    void test10_reviewWithoutLogin(TestExecutor &executor);
    void test11_fullCustomerOrder(TestExecutor &executor);
    void test12_ownerCreateRestaurant(TestExecutor &executor);
    void test13_cartWithoutItems(TestExecutor &executor);
    void test14_customerFullWorkflow(TestExecutor &executor);
    void test15_ownerFullSetup(TestExecutor &executor);
    void test16_agentAssignOrder(TestExecutor &executor);
    void test17_ownerManageOrder(TestExecutor &executor);
    void test18_multipleCartAdditions(TestExecutor &executor);
    void test19_wrongRoleAccess(TestExecutor &executor);
    void test20_fullLifecycle(TestExecutor &executor);
    void test21_ownerCompleteFlow(TestExecutor &executor);
    void test22_complexOrderManagement(TestExecutor &executor);
    void test23_invalidSequence(TestExecutor &executor);
    void test24_deepWorkflow(TestExecutor &executor);
    void test25_registerCustomerDuplicate(TestExecutor &executor);
}
