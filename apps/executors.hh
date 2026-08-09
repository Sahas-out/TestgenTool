#pragma once

// Test drivers: each wraps one subject application's FunctionFactory and runs a
// test string through the pipeline at one of three depths (see TestMode).
// Extracted verbatim from the former test_libapplication.cpp.

#include <iostream>
#include <memory>
#include <string>

#include "ast/ast.hh"
#include "ast/printvisitor.hh"
#include "atc/atc_generator.hh"
#include "core/env.hh"
#include "tester/tester.hh"
#include "see/see.hh"
#include "see/factories/restaurantfunctionfactory.hh"
#include "see/factories/ecommercefunctionfactory.hh"
#include "see/factories/libraryfunctionfactory.hh"

using namespace std;

// ============================================
// TEST EXECUTION MODES
// ============================================

enum class TestMode
{
    ORIGINAL,     // Just genATC (no rewrite)
    REWRITE_ONLY, // genATC + RewriteGlobalsVisitor (no backend)
    FULL_PIPELINE // Complete: genATC + Rewrite + SEE + Backend
};

// ============================================
// TEST EXECUTOR LIBRARY
// ============================================

class LibraryTestExecutor
{
private:
    TestMode mode;
    string backendUrl;

public:
    LibraryTestExecutor(TestMode m, const string &url = "http://localhost:8080")
        : mode(m), backendUrl(url) {}

    void runTest(
        const string &testName,
        unique_ptr<Spec> spec,
        const vector<string> &testSequence)
    {
        cout << "\n========================================" << endl;
        cout << "TEST: " << testName << endl;
        cout << "MODE: " << getModeString() << endl;
        cout << "DEPTH: " << testSequence.size() << " API calls" << endl;
        cout << "========================================\n"
             << endl;

        try
        {
            switch (mode)
            {
            case TestMode::ORIGINAL:
                runOriginal(std::move(spec), testSequence);
                break;
            case TestMode::REWRITE_ONLY:
                runRewriteOnly(std::move(spec), testSequence);
                break;
            case TestMode::FULL_PIPELINE:
                runFullPipeline(std::move(spec), testSequence);
                break;
            }
            cout << "\n✓ " << testName << " COMPLETE!\n"
                 << endl;
        }
        catch (const runtime_error &e)
        {
            string errorMsg = e.what();
            if (errorMsg.find("UNSAT") != string::npos)
            {
                cout << "\n⊘ " << testName << " UNSAT: Preconditions not satisfiable\n"
                     << endl;
            }
            else
            {
                cout << "\n✗ " << testName << " FAILED: " << errorMsg << "\n"
                     << endl;
            }
        }
        catch (const exception &e)
        {
            cout << "\n✗ " << testName << " FAILED: " << e.what() << "\n"
                 << endl;
        }
    }

private:
    string getModeString()
    {
        switch (mode)
        {
        case TestMode::ORIGINAL:
            return "Original (No Rewrite)";
        case TestMode::REWRITE_ONLY:
            return "Rewrite Only (No Backend)";
        case TestMode::FULL_PIPELINE:
            return "Full Pipeline (With Backend)";
        }
        return "Unknown";
    }

    void runOriginal(unique_ptr<Spec> spec, const vector<string> &ts)
    {
        Program atc = genATC(*spec, ts);
        PrintVisitor printer;
        printer.visitProgram(atc);
    }

    void runRewriteOnly(unique_ptr<Spec> spec, const vector<string> &ts)
    {
        auto factory = make_unique<Library::LibraryFunctionFactory>(backendUrl);
        Tester tester(factory.get());

        unique_ptr<Program> testApiATC = tester.generateATC(std::move(spec), ts);
    }

    void runFullPipeline(unique_ptr<Spec> spec, const vector<string> &ts)
    {
        SymbolTable *symbolTable = new SymbolTable(nullptr);

        auto factory = make_unique<Library::LibraryFunctionFactory>(backendUrl);
        Tester tester(factory.get());

        unique_ptr<Program> testApiATC = tester.generateATC(std::move(spec), ts);

        vector<Expr *> inputVars;
        ValueEnvironment *valueEnv = new ValueEnvironment(nullptr);

        unique_ptr<Program> ctc = tester.generateCTC(
            std::move(testApiATC),
            inputVars,
            valueEnv);

        if (!ctc)
        {
            cout << "\n[RESULT] UNSAT - Test preconditions cannot be satisfied" << endl;
            delete symbolTable;
            delete valueEnv;
            throw runtime_error("UNSAT: Preconditions not satisfiable");
        }

        cout << "\n[FINAL CTC]" << endl;
        PrintVisitor printer;
        printer.visitProgram(*ctc);

        delete symbolTable;
        delete valueEnv;
    }
};

// ============================================
// TEST EXECUTOR
// ============================================

class TestExecutor
{
private:
    TestMode mode;
    string backendUrl;

public:
    TestExecutor(TestMode m, const string &url = "http://localhost:5002")
        : mode(m), backendUrl(url) {}

    void runTest(
        const string &testName,
        unique_ptr<Spec> spec,
        const vector<string> &testSequence)
    {
        cout << "\n========================================" << endl;
        cout << "TEST: " << testName << endl;
        cout << "MODE: " << getModeString() << endl;
        cout << "DEPTH: " << testSequence.size() << " API calls" << endl;
        cout << "========================================\n"
             << endl;

        try
        {
            switch (mode)
            {
            case TestMode::ORIGINAL:
                runOriginal(std::move(spec), testSequence);
                break;
            case TestMode::REWRITE_ONLY:
                runRewriteOnly(std::move(spec), testSequence);
                break;
            case TestMode::FULL_PIPELINE:
                runFullPipeline(std::move(spec), testSequence);
                break;
            }
            cout << "\n✓ " << testName << " COMPLETE!\n"
                 << endl;
        }
        catch (const runtime_error &e)
        {
            string errorMsg = e.what();
            // Check if this is an UNSAT error
            if (errorMsg.find("UNSAT") != string::npos)
            {
                cout << "\n⊘ " << testName << " UNSAT: Preconditions not satisfiable\n"
                     << endl;
            }
            else
            {
                cout << "\n✗ " << testName << " FAILED: " << errorMsg << "\n"
                     << endl;
            }
        }
        catch (const exception &e)
        {
            cout << "\n✗ " << testName << " FAILED: " << e.what() << "\n"
                 << endl;
        }
    }

private:
    string getModeString()
    {
        switch (mode)
        {
        case TestMode::ORIGINAL:
            return "Original (No Rewrite)";
        case TestMode::REWRITE_ONLY:
            return "Rewrite Only (No Backend)";
        case TestMode::FULL_PIPELINE:
            return "Full Pipeline (With Backend)";
        }
        return "Unknown";
    }

    void runOriginal(unique_ptr<Spec> spec, const vector<string> &ts)
    {
        Program atc = genATC(*spec, ts);
        PrintVisitor printer;
        printer.visitProgram(atc);
    }

    void runRewriteOnly(unique_ptr<Spec> spec, const vector<string> &ts)
    {
        auto factory = make_unique<RestaurantFunctionFactory>(backendUrl);
        Tester tester(factory.get());

        unique_ptr<Program> testApiATC = tester.generateATC(std::move(spec), ts);
    }

    void runFullPipeline(unique_ptr<Spec> spec, const vector<string> &ts)
    {
        SymbolTable *symbolTable = new SymbolTable(nullptr);

        auto factory = make_unique<RestaurantFunctionFactory>(backendUrl);
        Tester tester(factory.get());

        unique_ptr<Program> testApiATC = tester.generateATC(std::move(spec), ts);

        vector<Expr *> inputVars;
        ValueEnvironment *valueEnv = new ValueEnvironment(nullptr);

        unique_ptr<Program> ctc = tester.generateCTC(
            std::move(testApiATC),
            inputVars,
            valueEnv);

        if (!ctc)
        {
            cout << "\n[RESULT] UNSAT - Test preconditions cannot be satisfied" << endl;
            delete symbolTable;
            delete valueEnv;
            throw runtime_error("UNSAT: Preconditions not satisfiable");
        }

        cout << "\n[FINAL CTC]" << endl;
        PrintVisitor printer;
        printer.visitProgram(*ctc);

        delete symbolTable;
        delete valueEnv;
    }
};

// ============================================
// ECOMMERCE TEST EXECUTOR
// ============================================

class EcommerceTestExecutor
{
private:
    TestMode mode;
    string backendUrl;

public:
    EcommerceTestExecutor(TestMode m, const string &url = "http://localhost:3000")
        : mode(m), backendUrl(url) {}

    void runTest(
        const string &testName,
        unique_ptr<Spec> spec,
        const vector<string> &testSequence)
    {
        cout << "\n========================================" << endl;
        cout << "TEST: " << testName << endl;
        cout << "MODE: " << getModeString() << endl;
        cout << "DEPTH: " << testSequence.size() << " API calls" << endl;
        cout << "========================================\n"
             << endl;

        try
        {
            switch (mode)
            {
            case TestMode::ORIGINAL:
                runOriginal(std::move(spec), testSequence);
                break;
            case TestMode::REWRITE_ONLY:
                runRewriteOnly(std::move(spec), testSequence);
                break;
            case TestMode::FULL_PIPELINE:
                runFullPipeline(std::move(spec), testSequence);
                break;
            }
            cout << "\n✓ " << testName << " COMPLETE!\n"
                 << endl;
        }
        catch (const runtime_error &e)
        {
            string errorMsg = e.what();
            if (errorMsg.find("UNSAT") != string::npos)
            {
                cout << "\n⊘ " << testName << " UNSAT: Preconditions not satisfiable\n"
                     << endl;
            }
            else
            {
                cout << "\n✗ " << testName << " FAILED: " << errorMsg << "\n"
                     << endl;
            }
        }
        catch (const exception &e)
        {
            cout << "\n✗ " << testName << " FAILED: " << e.what() << "\n"
                 << endl;
        }
    }

private:
    string getModeString()
    {
        switch (mode)
        {
        case TestMode::ORIGINAL:
            return "Original (No Rewrite)";
        case TestMode::REWRITE_ONLY:
            return "Rewrite Only (No Backend)";
        case TestMode::FULL_PIPELINE:
            return "Full Pipeline (With Backend)";
        }
        return "Unknown";
    }

    void runOriginal(unique_ptr<Spec> spec, const vector<string> &ts)
    {
        Program atc = genATC(*spec, ts);
        PrintVisitor printer;
        printer.visitProgram(atc);
    }

    void runRewriteOnly(unique_ptr<Spec> spec, const vector<string> &ts)
    {
        auto factory = make_unique<Ecommerce::EcommerceFunctionFactory>(backendUrl);
        Tester tester(factory.get());

        unique_ptr<Program> testApiATC = tester.generateATC(std::move(spec), ts);
    }

    void runFullPipeline(unique_ptr<Spec> spec, const vector<string> &ts)
    {
        SymbolTable *symbolTable = new SymbolTable(nullptr);

        auto factory = make_unique<Ecommerce::EcommerceFunctionFactory>(backendUrl);
        Tester tester(factory.get());

        unique_ptr<Program> testApiATC = tester.generateATC(std::move(spec), ts);

        vector<Expr *> inputVars;
        ValueEnvironment *valueEnv = new ValueEnvironment(nullptr);

        unique_ptr<Program> ctc = tester.generateCTC(
            std::move(testApiATC),
            inputVars,
            valueEnv);

        if (!ctc)
        {
            cout << "\n[RESULT] UNSAT - Test preconditions cannot be satisfied" << endl;
            delete symbolTable;
            delete valueEnv;
            throw runtime_error("UNSAT: Preconditions not satisfiable");
        }

        cout << "\n[FINAL CTC]" << endl;
        PrintVisitor printer;
        printer.visitProgram(*ctc);

        delete symbolTable;
        delete valueEnv;
    }
};
