# Order Book Reconstruction (MBO to MBP-10)

## Overview

This program reconstructs a 10-level Market-by-Price (MBP-10) limit order book from a Market-by-Order (MBO) data feed. It processes order events (Add, Cancel, Trade, etc.) from an input CSV file and outputs the original event data along with the reconstructed order book state.

## Features

- Processes MBO (Market by Order) events
- Maintains a 10-level order book
- Handles various order types (Add, Cancel, Trade, etc.)
- Outputs combined event and order book state data
- Efficient price level management using STL containers

## Prerequisites

- C++17 compatible compiler (g++)
- Make build system
- Unix-like environment (for make commands)

## Building the Project

Use the provided Makefile to compile the program:

```bash
make
```

This will create an executable named `reconstruction_deep`.

## Usage

Run the program with two command-line arguments:

```bash
./reconstruction_deep <data>/<input_mbo.csv> <output.csv>
```

Example:
```bash
./reconstruction_deep data/mbo.csv output.csv
```

### Input Format

The input CSV file should contain MBO (Market by Order) data with the following fields:
- Time
- Event type (A: Add, C: Cancel, T: Trade, F: Fill, R: Reset)
- Side (B: Bid, A: Ask)
- Price
- Size
- Order ID
- Additional metadata

### Output Format

The program generates a CSV file containing:
- Original event data
- Event-specific information
- Top 10 price levels for both bid and ask sides
- For each price level: price, size, and count

## Implementation Details

### Core Data Structures

The order book uses two `map` containers:
- Bids: `map<double, Level, greater<double>>`
- Asks: `map<double, Level>`

Benefits:
- Automatic price level sorting
- O(log N) complexity for operations
- Memory efficient

### Special Event Handling

- **Trade (T)**: Updates opposite side of the book
- **Fill (F)**: Tracks order IDs for proper execution
- **Cancel (C)**: Removes or reduces orders
- **Reset (R)**: Clears the entire book

## Error Handling

The program includes basic error handling for:
- Invalid command-line arguments
- File I/O operations
- Data parsing

## Clean Up

To remove the compiled executable:

```bash
make clean
```