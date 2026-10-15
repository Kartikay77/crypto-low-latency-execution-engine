# Crypto Low-Latency Order Book & Execution Engine

A C++20 trading systems project focused on building a simplified low-latency crypto order book, matching engine, execution engine, risk engine, and market-data replay framework.

The project is designed to explore core concepts used in quantitative trading and electronic market infrastructure, including:

- Limit order books
- Price-time priority
- Order matching
- Market microstructure
- Execution tracking
- Risk controls
- Market-data replay
- Latency measurement
- Throughput benchmarking
- Automated testing

---

## Current Features

### Limit Order Book

- Separate bid and ask books
- Descending bid prices
- Ascending ask prices
- FIFO queues within each price level
- Best bid retrieval
- Best ask retrieval
- Aggregated quantity by price level

### Order Lifecycle

Supports:

- ADD
- CANCEL
- MODIFY

Orders are identified using 64-bit order IDs.

### Matching Engine

Supports:

- Crossing buy and sell orders
- Partial fills
- Multiple fills
- Price-time priority
- FIFO execution at equal prices
- Trade generation

### Trade Events

Each generated trade contains:

- Buy order ID
- Sell order ID
- Execution price
- Execution quantity
- Buy-side ownership flag
- Sell-side ownership flag

This allows the execution engine to distinguish between internal strategy orders and external market orders.

### Market Data Replay

The project includes CSV-based market-data replay supporting:

- ADD events
- CANCEL events
- MODIFY events
- Automatic order matching after market-data updates

A synthetic benchmark workload generates:

- 100,000 total events
- 70% ADD
- 20% CANCEL
- 10% MODIFY

### Execution Engine

Supports:

- Buy fill processing
- Sell fill processing
- Position tracking
- Cash tracking
- Fill counting
- Ownership-aware trade processing

### Risk Engine

Current risk controls include:

- Maximum order quantity
- Maximum position
- Maximum order notional
- Kill switch
- Order validation

### Latency Benchmarking

Market-data replay is instrumented using `std::chrono::steady_clock`.

Current benchmark metrics include:

- Average event-processing latency
- P50 latency
- P95 latency
- P99 latency
- Total processing time
- Events processed per second

Example mixed-event benchmark:

```text
Events: 100000
Average: ~13.8 µs
P50: ~1.0 µs
P95: ~82 µs
P99: ~129 µs
Throughput: ~71K events/second

Benchmark results vary by hardware, compiler settings, system load, and workload.
Automated Tests
Order Book Tests
Tests cover:
- Best bid / best ask
- Partial fills
- Multiple fills
- FIFO price-time priority
- Order cancellation
- Order modification
Example:
testBestBidAsk PASSED
testPartialFill PASSED
testMultipleFills PASSED
testFIFO PASSED
testCancel PASSED
testModify PASSED

ALL ORDER BOOK TESTS PASSED

Execution Engine Tests
Tests cover:
- Buy fills
- Sell fills
- Partial buy fills
- Position updates
- Cash updates
- Fill counting
Example:
testBuyFill PASSED
testSellFill PASSED
testPartialBuyFill PASSED

ALL EXECUTION ENGINE TESTS PASSED

Risk Engine Tests
Tests cover:
- Valid order approval
- Maximum order size rejection
- Maximum position rejection
- Maximum notional rejection
- Kill switch behavior
Example:

testValidOrder PASSED
testMaxOrderQuantity PASSED
testMaxPosition PASSED
testMaxNotional PASSED
testKillSwitch PASSED

ALL RISK ENGINE TESTS PASSED

Project Structure

crypto_execution_engine/
├── CMakeLists.txt
├── README.md
├── include/
│   ├── Order.h
│   ├── OrderBook.h
│   ├── Trade.h
│   ├── MarketDataReplay.h
│   ├── ExecutionEngine.h
│   └── RiskEngine.h
│
├── src/
│   ├── main.cpp
│   ├── OrderBook.cpp
│   ├── MarketDataReplay.cpp
│   ├── ExecutionEngine.cpp
│   └── RiskEngine.cpp
│
├── tests/
│   ├── test_orderbook.cpp
│   ├── test_execution_engine.cpp
│   └── test_risk_engine.cpp
│
└── data/

Order Representation

enum class Side : uint8_t
{
    Buy,
    Sell
};

struct Order
{
    std::uint64_t orderId;
    Side side;
    double price;
    std::uint64_t quantity;
    bool isOurOrder = false;
};


Order Book Design
The bid side is maintained in descending price order:

100.00
99.50
99.00

The ask side is maintained in ascending price order:

101.00
101.50
102.00

Orders at the same price level are stored in FIFO order.
Example:

Price 100.00

Order 1 -> Order 5 -> Order 8

This implements simplified price-time priority commonly used in electronic markets.


Build Instructions
From the project root:

mkdir -p build
cd build
cmake ..
cmake --build .

Run the main benchmark:
./crypto_engine

Run order-book tests:
./test_orderbook

Run execution-engine tests:
./test_execution_engine

Run risk-engine tests:
./test_risk_engine

Technologies
- C++20
- CMake
- STL
- std::map
- std::deque
- std::unordered_map
- std::chrono
- File I/O
- Git


Development Roadmap
Completed
- Limit order book
- Price-time priority
- ADD / CANCEL / MODIFY
- Partial fills
- Multiple fills
- Trade generation
- Market-data replay
- Execution tracking
- Position and cash accounting
- Ownership-aware fills
- Risk engine
- Kill switch
- Latency benchmarking
- Throughput benchmarking
- Automated testing
Next
- Integrate RiskEngine directly into order submission
- Strategy layer
- Market-making logic
- Inventory-aware quoting
- Spread / microprice calculations
- Multithreaded market-data and execution pipelines
- Low-lock or lock-free queues
- 1M+ event stress testing
- Historical crypto market-data replay
- Exchange WebSocket integration
- Linux profiling with perf
- Hot-path optimization


Goal
The goal of this project is to progressively develop a production-oriented trading systems prototype while studying the engineering challenges involved in low-latency crypto market infrastructure, execution systems, market microstructure, performance optimization, and risk management.