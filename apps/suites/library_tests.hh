#pragma once

// Hand-written test strings for the LibraryTests subject application.
// Extracted verbatim from the former test_libapplication.cpp.

#include "executors.hh"

namespace LibraryTests
{
    void test01_getAllBooks(LibraryTestExecutor &executor);
    void test02_getAllStudents(LibraryTestExecutor &executor);
    void test03_saveBook(LibraryTestExecutor &executor);
    void test04_saveStudent(LibraryTestExecutor &executor);
    void test05_saveAndGetBook(LibraryTestExecutor &executor);
    void test06_saveAndGetStudent(LibraryTestExecutor &executor);
    void test07_saveBookTwice(LibraryTestExecutor &executor);
    void test08_getBookNotFound(LibraryTestExecutor &executor);
    void test09_bookCRUD(LibraryTestExecutor &executor);
    void test10_studentCRUD(LibraryTestExecutor &executor);
    void test11_createBookAndStudent(LibraryTestExecutor &executor);
    void test12_createRequest(LibraryTestExecutor &executor);
    void test13_acceptRequest(LibraryTestExecutor &executor);
    void test14_fullBorrowReturn(LibraryTestExecutor &executor);
    void test15_directLoan(LibraryTestExecutor &executor);
    void test16_rejectRequest(LibraryTestExecutor &executor);
    void test17_requestWithoutBook(LibraryTestExecutor &executor);
    void test18_requestWithoutStudent(LibraryTestExecutor &executor);
    void test19_acceptWithoutRequest(LibraryTestExecutor &executor);
    void test20_returnWithoutLoan(LibraryTestExecutor &executor);
    void test21_multipleBooks(LibraryTestExecutor &executor);
    void test22_multipleStudents(LibraryTestExecutor &executor);
    void test23_multipleBorrowings(LibraryTestExecutor &executor);
    void test24_fullLibraryWorkflow(LibraryTestExecutor &executor);
    void test25_complexScenario(LibraryTestExecutor &executor);
}
