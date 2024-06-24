# Backtesting_Engine
This project uses python for data acquisition and preprocessing, and the backtesting engine has been built with C++. The engine fetches historical stock data from Yahoo Finance. The data is then processed and includes adjustments for stock splits and dividends. 


Some key features: 

Simulation & Analysis: The c++ backtesting engine simulates trading strategies and computes  portfolio values, daily returns, and implementing portfolio rebalancing based on predefined target allocations. 
Risk Metrics: Volatility, maximum drawdown and sharpe ratio have been calculated
Scalability: It handles portfolios of varying sizes

Here is an example result for a portfolio consisting of equity stocks (AAPL, MSFT, GOOGL, AMZN, META, TSLA, NVDA, NFLX, ADBE, PYPL) 

sanjana97@DESKTOP-QHF6V30:~/backtesting$ ./backtestingengine
Final Portfolio Value: $3.39862e+06
Volatility: 0.0208155
Max Drawdown: 0.507318
Sharpe Ratio: -0.417642


Dependencies:

Python Libraries - yfinance, pandas, numpy, datetime 
C++ libraries - standard C++ libraries for file handling, data structures and numerical computations. 