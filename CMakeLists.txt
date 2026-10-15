cmake_minimum_required(VERSION 3.16)

project(CryptoExecutionEngine LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# -----------------------------
# Main application
# -----------------------------
add_executable(crypto_engine
    src/main.cpp
    src/OrderBook.cpp
    src/MarketDataReplay.cpp
    src/ExecutionEngine.cpp
)

target_include_directories(crypto_engine PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# -----------------------------
# Execution engine tests
# -----------------------------
add_executable(test_execution_engine
    tests/test_execution_engine.cpp
    src/OrderBook.cpp
    src/ExecutionEngine.cpp
)

target_include_directories(test_execution_engine PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# -----------------------------
# Order book tests
# -----------------------------
add_executable(test_orderbook
    tests/test_orderbook.cpp
    src/OrderBook.cpp
)

target_include_directories(test_orderbook PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# -----------------------------
# Risk engine tests
# -----------------------------
add_executable(test_risk_engine
    tests/test_risk_engine.cpp
    src/RiskEngine.cpp
)

target_include_directories(test_risk_engine PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)