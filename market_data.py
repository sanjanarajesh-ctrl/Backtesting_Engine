#!/usr/bin/env python3

import numpy as np
import yfinance as yf
from datetime import datetime

def fetch_data(stocks, start_date, end_date):
    for ticker in stocks:
        # Download historical data
        price_frame = yf.download(ticker, start=start_date, end=end_date)

        # Reset index and convert date to timestamp
        price_frame.reset_index(inplace=True)
        price_frame['Date'] = price_frame['Date'].astype(np.int64) // 10**9

        # Adjust OHLC prices if needed
        ohlc_cols = ['Open', 'High', 'Low', 'Close']
        if 'Adj Close' in price_frame.columns:
            scale = price_frame['Adj Close'] / price_frame['Close']
            for col in ohlc_cols:
                price_frame[col] *= scale
            del price_frame['Adj Close']

        # Adjust high and low prices
        price_frame['High'] = price_frame[ohlc_cols].max(axis=1)
        price_frame['Low'] = price_frame[ohlc_cols].min(axis=1)

        # Save data to CSV file in current directory
        filename = f"{ticker}.csv"
        price_frame.to_csv(filename, index=False, header=None)
        print(f"Saved data for {ticker} to {filename}")

if __name__ == "__main__":
    # Example: Input from user (10 stocks, start date, end date)
    stocks = ['AAPL', 'MSFT', 'GOOGL', 'AMZN', 'META', 'TSLA', 'NVDA', 'NFLX', 'ADBE', 'PYPL']
    start_date = datetime(2020, 1, 1)
    end_date = datetime(2024, 12, 31)

    # Fetch data for the stocks
    fetch_data(stocks, start_date, end_date)
