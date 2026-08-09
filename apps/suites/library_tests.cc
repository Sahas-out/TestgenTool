#include "suites/library_tests.hh"
#include "specs/LibrarySpec.hh"

namespace LibraryTests
{

    // ========================================
    // DEPTH 1: Single API Call Tests
    // ========================================

    void test01_getAllBooks(LibraryTestExecutor &executor)
    {
        executor.runTest(
            "Test 01: Get All Books (Depth=1)",
            makeLibrarySpec(),
            {"getAllBooksOk"});
    }

    void test02_getAllStudents(LibraryTestExecutor &executor)
    {
        executor.runTest(
            "Test 02: Get All Students (Depth=1)",
            makeLibrarySpec(),
            {"getAllStudentsOk"});
    }

    void test03_saveBook(LibraryTestExecutor &executor)
    {
        executor.runTest(
            "Test 03: Save Book (Depth=1)",
            makeLibrarySpec(),
            {"saveBookOk"});
    }

    void test04_saveStudent(LibraryTestExecutor &executor)
    {
        executor.runTest(
            "Test 04: Save Student (Depth=1)",
            makeLibrarySpec(),
            {"saveStudentOk"});
    }

    // ========================================
    // DEPTH 2: Two API Call Tests
    // ========================================

    void test05_saveAndGetBook(LibraryTestExecutor &executor)
    {
        executor.runTest(
            "Test 05: Save Book → Get Book (Depth=2)",
            makeLibrarySpec(),
            {"saveBookOk", "getBookByCodeOk"});
    }

    void test06_saveAndGetStudent(LibraryTestExecutor &executor)
    {
        executor.runTest(
            "Test 06: Save Student → Get Student (Depth=2)",
            makeLibrarySpec(),
            {"saveStudentOk", "getStudentByIdOk"});
    }

    void test07_saveBookTwice(LibraryTestExecutor &executor)
    {
        executor.runTest(
            "Test 07: Save Two Books (Depth=2)",
            makeLibrarySpec(),
            {"saveBookOk", "saveBookOk"});
    }

    void test08_getBookNotFound(LibraryTestExecutor &executor)
    {
        executor.runTest(
            "Test 08: Get Book Not Found (Depth=1)",
            makeLibrarySpec(),
            {"getBookByCodeErr"});
    }

    // ========================================
    // DEPTH 3: Three API Call Tests
    // ========================================

    void test09_bookCRUD(LibraryTestExecutor &executor)
    {
        executor.runTest(
            "Test 09: Book CRUD - Save → Update → Delete (Depth=3)",
            makeLibrarySpec(),
            {"saveBookOk", "updateBookOk", "deleteBookOk"});
    }

    void test10_studentCRUD(LibraryTestExecutor &executor)
    {
        executor.runTest(
            "Test 10: Student CRUD - Save → Update → Delete (Depth=3)",
            makeLibrarySpec(),
            {"saveStudentOk", "updateStudentOk", "deleteStudentOk"});
    }

    void test11_createBookAndStudent(LibraryTestExecutor &executor)
    {
        executor.runTest(
            "Test 11: Create Book and Student (Depth=2)",
            makeLibrarySpec(),
            {"saveBookOk", "saveStudentOk"});
    }

    // ========================================
    // DEPTH 4+: Request/Loan Flow Tests
    // ========================================

    void test12_createRequest(LibraryTestExecutor &executor)
    {
        executor.runTest(
            "Test 12: Create Request Flow (Depth=3)",
            makeLibrarySpec(),
            {"saveBookOk", "saveStudentOk", "saveRequestOk"});
    }

    void test13_acceptRequest(LibraryTestExecutor &executor)
    {
        executor.runTest(
            "Test 13: Accept Request Flow (Depth=4)",
            makeLibrarySpec(),
            {"saveBookOk", "saveStudentOk", "saveRequestOk", "acceptRequestOk"});
    }

    void test14_fullBorrowReturn(LibraryTestExecutor &executor)
    {
        executor.runTest(
            "Test 14: Full Borrow/Return Lifecycle (Depth=5)",
            makeLibrarySpec(),
            {"saveBookOk", "saveStudentOk", "saveRequestOk", "acceptRequestOk", "returnBookOk"});
    }

    void test15_directLoan(LibraryTestExecutor &executor)
    {
        executor.runTest(
            "Test 15: Direct Loan Creation (Depth=3)",
            makeLibrarySpec(),
            {"saveBookOk", "saveStudentOk", "saveLoanOk"});
    }

    void test16_rejectRequest(LibraryTestExecutor &executor)
    {
        executor.runTest(
            "Test 16: Reject Request (Depth=4)",
            makeLibrarySpec(),
            {"saveBookOk", "saveStudentOk", "saveRequestOk", "deleteRequestOk"});
    }

    // ========================================
    // NEGATIVE TESTS (Expected UNSAT)
    // ========================================

    void test17_requestWithoutBook(LibraryTestExecutor &executor)
    {
        executor.runTest(
            "Test 17: Request Without Book (Should be UNSAT)",
            makeLibrarySpec(),
            {"saveStudentOk", "saveRequestOk"}); // No book - should fail
    }

    void test18_requestWithoutStudent(LibraryTestExecutor &executor)
    {
        executor.runTest(
            "Test 18: Request Without Student (Should be UNSAT)",
            makeLibrarySpec(),
            {"saveBookOk", "saveRequestOk"}); // No student - should fail
    }

    void test19_acceptWithoutRequest(LibraryTestExecutor &executor)
    {
        executor.runTest(
            "Test 19: Accept Without Request (Should be UNSAT)",
            makeLibrarySpec(),
            {"acceptRequestOk"}); // No request - should fail
    }

    void test20_returnWithoutLoan(LibraryTestExecutor &executor)
    {
        executor.runTest(
            "Test 20: Return Without Loan (Should be UNSAT)",
            makeLibrarySpec(),
            {"returnBookOk"}); // No loan - should fail
    }

    // ========================================
    // COMPLEX WORKFLOWS
    // ========================================

    void test21_multipleBooks(LibraryTestExecutor &executor)
    {
        executor.runTest(
            "Test 21: Multiple Books (Depth=5)",
            makeLibrarySpec(),
            {"saveBookOk", "saveBookOk", "saveBookOk", "getAllBooksOk", "getBookByCodeOk"});
    }

    void test22_multipleStudents(LibraryTestExecutor &executor)
    {
        executor.runTest(
            "Test 22: Multiple Students (Depth=5)",
            makeLibrarySpec(),
            {"saveStudentOk", "saveStudentOk", "saveStudentOk", "getAllStudentsOk", "getStudentByIdOk"});
    }

    void test23_multipleBorrowings(LibraryTestExecutor &executor)
    {
        executor.runTest(
            "Test 23: Multiple Borrowings (Depth=7)",
            makeLibrarySpec(),
            {"saveBookOk", "saveBookOk", "saveStudentOk",
             "saveRequestOk", "saveRequestOk",
             "acceptRequestOk", "acceptRequestOk"});
    }

    void test24_fullLibraryWorkflow(LibraryTestExecutor &executor)
    {
        executor.runTest(
            "Test 24: Full Library Workflow (Depth=8)",
            makeLibrarySpec(),
            {"saveBookOk", "saveBookOk",
             "saveStudentOk", "saveStudentOk",
             "saveRequestOk", "acceptRequestOk",
             "returnBookOk", "getAllLoansOk"});
    }

    void test25_complexScenario(LibraryTestExecutor &executor)
    {
        executor.runTest(
            "Test 25: Complex Multi-User Scenario (Depth=10)",
            makeLibrarySpec(),
            {"saveBookOk", "saveBookOk", "saveBookOk",
             "saveStudentOk", "saveStudentOk",
             "saveRequestOk", "saveRequestOk",
             "acceptRequestOk",
             "returnBookOk",
             "getAllRequestsOk"});
    }
}
