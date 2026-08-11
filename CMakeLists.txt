cmake_minimum_required(VERSION 3.16)
project(testgen CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

# Emit build/compile_commands.json so editors/clangd resolve includes correctly.
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Dependencies are vendored under third_party/ (gitignored) so the project is
# self-contained and needs no system-wide installs. See refactor.txt.
set(TP ${CMAKE_SOURCE_DIR}/third_party)

add_executable(testgen
    # --- entry point + test suites -------------------------------------
    # (executors.hh is header-only: the three executor classes were already
    #  fully inline, so splitting out a .cc would have meant retyping ~350
    #  lines of method bodies for no benefit.)
    apps/main.cc
    apps/suites/restaurant_tests.cc
    apps/suites/library_tests.cc
    apps/suites/ecommerce_tests.cc

    # --- ast: AST nodes and visitors -----------------------------------
    src/ast/ast.cc
    src/ast/astvisitor.cc
    src/ast/printvisitor.cc
    src/ast/clonevisitor.cc
    src/ast/symvar.cc

    # --- core: symbol tables, value envs, type maps --------------------
    src/core/env.cc
    src/core/typemap.cc

    # --- atc: Spec + test string -> Abstract Test Case -----------------
    src/atc/atc_generator.cc
    src/atc/rewrite_globals_visitor.cc

    # --- see: symbolic execution engine + solvers ----------------------
    src/see/see.cc
    src/see/solver.cc
    src/see/z3solver.cc
    src/see/factories/functionfactory.cc
    src/see/factories/restaurantfunctionfactory.cc
    src/see/factories/ecommercefunctionfactory.cc
    src/see/factories/libraryfunctionfactory.cc

    # --- http: live backend execution ----------------------------------
    src/http/httpclient.cc

    # --- tester: ATC -> Concrete Test Case -----------------------------
    src/tester/tester.cc

    # --- specs: the three subject applications -------------------------
    src/specs/RestaurantSpec.cc
    src/specs/EcommerceSpec.cc
    src/specs/LibrarySpec.cc

    # --- teststringGenerator ------------------------------------------
    # The GA generator builds as its own library (ga_teststring, below) and
    # is not linked here yet: nothing in apps/ calls it, and it needs
    # neither Z3 nor curl. Add it to this target when main.cc starts
    # generating test strings instead of using hand-written ones.
)

target_include_directories(testgen PRIVATE
    ${CMAKE_SOURCE_DIR}/src
    ${CMAKE_SOURCE_DIR}/apps
    ${TP}/include          # nlohmann/json.hpp
    ${TP}/z3/include       # z3++.h, z3_api.h, ...
    ${TP}/curl/include     # curl/curl.h
)

target_link_libraries(testgen PRIVATE
    ${TP}/z3/lib/libz3.so
    ${TP}/curl/lib/libcurl.so
)

# Find the vendored shared objects at run time without needing LD_LIBRARY_PATH.
set_target_properties(testgen PROPERTIES
    BUILD_RPATH "${TP}/z3/lib;${TP}/curl/lib"
    INSTALL_RPATH "${TP}/z3/lib;${TP}/curl/lib")

# Suppressions carried over from the original makefile. These silence
# pre-existing style issues across the codebase, not new ones.
target_compile_options(testgen PRIVATE
    -Wall
    -Wno-reorder
    -Wno-deprecated-declarations
    -Wno-unused-variable
    -Wno-unused-function)

if(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    # macOS/clang-only spellings from the original makefile.
    target_compile_options(testgen PRIVATE
        -Wno-delete-non-abstract-non-virtual-dtor
        -Wno-unqualified-std-cast-call
        -Wno-unused-private-field)
endif()

# ---------------------------------------------------------------------------
# ga_teststring: genetic generation of test strings.
#
# Its own library rather than more sources on testgen, so it can be exercised
# by a test binary that links neither Z3 nor curl.
# ---------------------------------------------------------------------------
add_library(ga_teststring STATIC
    src/teststringGenerator/ga_types.cc
    src/teststringGenerator/genetic_generator.cc
    src/teststringGenerator/baseline_ga_factory.cc
    src/teststringGenerator/strategies/crossover_strategies/single_point_crossover.cc
    src/teststringGenerator/strategies/mutation_strategies/block_replacement_mutation.cc
    src/teststringGenerator/strategies/selection_strategies/tournament_selection.cc
    src/teststringGenerator/strategies/fitness_strategies/block_coverage_fitness.cc
    src/teststringGenerator/strategies/population_initialization_strategies/random_population_initializer.cc
    src/teststringGenerator/strategies/validation_strategies/always_valid_validator.cc
)

target_include_directories(ga_teststring PUBLIC ${CMAKE_SOURCE_DIR}/src)

target_compile_options(ga_teststring PRIVATE -Wall)

# ---------------------------------------------------------------------------
# ga_smoke_test: does the generator emit well-formed test strings?
#
# Deliberately narrow — it exercises the generator alone, not the ATC/CTC
# pipeline, so it needs no solver, no HTTP client and no running backend. The
# ast/ sources are pulled in because Spec and the Library spec are built from
# them; those five files include only each other.
# ---------------------------------------------------------------------------
add_executable(ga_smoke_test
    tests/ga_smoke_test.cc
    src/ast/ast.cc
    src/ast/astvisitor.cc
    src/ast/clonevisitor.cc
    src/ast/printvisitor.cc
    src/ast/symvar.cc
    src/specs/LibrarySpec.cc
)

target_link_libraries(ga_smoke_test PRIVATE ga_teststring)

# -Wno-reorder and -Wno-unused-but-set-variable silence pre-existing warnings in
# ast.cc and LibrarySpec.cc, matching the suppressions on the testgen target.
target_compile_options(ga_smoke_test PRIVATE
    -Wall
    -Wno-reorder
    -Wno-unused-variable
    -Wno-unused-but-set-variable)
