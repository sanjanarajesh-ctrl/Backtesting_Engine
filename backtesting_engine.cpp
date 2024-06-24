#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <map>
#include <cmath>
#include <algorithm>
#include <numeric>  // <-- Include this header

// Your data structure for holding stock data
struct StockData {
    long long date;
    double open;
    double high;
    double low;
    double close;
};

// Function to read CSV files
std::vector<StockData> readCSV(const std::string& filename) {
    std::vector<StockData> data;
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error opening file: " << filename << std::endl;
        return data;
    }
    std::string line;
    while (std::getline(file, line)) {
        std::stringstream ss(line);
        std::string value;
        StockData stockData;
        std::getline(ss, value, ',');
        stockData.date = std::stoll(value);
        std::getline(ss, value, ',');
        stockData.open = std::stod(value);
        std::getline(ss, value, ',');
        stockData.high = std::stod(value);
        std::getline(ss, value, ',');
        stockData.low = std::stod(value);
        std::getline(ss, value, ',');
        stockData.close = std::stod(value);
        data.push_back(stockData);
    }
    if (data.empty()) {
        std::cerr << "No data read from file: " << filename << std::endl;
    }
    return data;
}

// Function to compute portfolio value
double computePortfolioValue(const std::map<std::string, double>& portfolio, const std::map<std::string, StockData>& prices) {
    double value = 0.0;
    for (const auto& [ticker, amount] : portfolio) {
        value += amount * prices.at(ticker).close;
    }
    return value;
}

// Function to rebalance portfolio
void rebalancePortfolio(std::map<std::string, double>& portfolio, const std::map<std::string, double>& targetAllocation, double totalValue, const std::map<std::string, StockData>& prices) {
    for (const auto& [ticker, targetPercent] : targetAllocation) {
        portfolio[ticker] = (targetPercent * totalValue) / prices.at(ticker).close;
    }
}

// Function to compute volatility
double computeVolatility(const std::vector<double>& returns) {
    double sum = std::accumulate(returns.begin(), returns.end(), 0.0);
    double mean = sum / returns.size();
    double sq_sum = std::inner_product(returns.begin(), returns.end(), returns.begin(), 0.0);
    double stdev = std::sqrt(sq_sum / returns.size() - mean * mean);
    return stdev;
}

// Function to compute max drawdown
double computeMaxDrawdown(const std::vector<double>& portfolioValues) {
    double maxDrawdown = 0.0;
    double peak = portfolioValues[0];
    for (double value : portfolioValues) {
        if (value > peak) {
            peak = value;
        }
        double drawdown = (peak - value) / peak;
        if (drawdown > maxDrawdown) {
            maxDrawdown = drawdown;
        }
    }
    return maxDrawdown;
}

// Function to compute Sharpe ratio
double computeSharpeRatio(const std::vector<double>& returns, double riskFreeRate) {
    double meanReturn = std::accumulate(returns.begin(), returns.end(), 0.0) / returns.size();
    double stdev = computeVolatility(returns);
    return (meanReturn - riskFreeRate) / stdev;
}

int main() {
    std::vector<std::string> tickers = {"AAPL", "MSFT", "GOOGL", "AMZN", "META", "TSLA", "NVDA", "NFLX", "ADBE", "PYPL"};
    std::map<std::string, std::vector<StockData>> stockData;
    std::map<std::string, double> targetAllocation;
    std::map<std::string, double> portfolio;
    double initialInvestment = 1000000.0; // Initial investment amount

    // Set target allocation (e.g., equal weight)
    for (const auto& ticker : tickers) {
        targetAllocation[ticker] = 1.0 / tickers.size();
    }

    // Read stock data from CSV files
    for (const auto& ticker : tickers) {
        std::string filename = ticker + ".csv"; // Adjust the path to your CSV files if necessary
        stockData[ticker] = readCSV(filename);
        if (stockData[ticker].empty()) {
            std::cerr << "Error: No data for ticker " << ticker << std::endl;
            return 1;
        }
        portfolio[ticker] = 0.0;
    }

    // Initialize portfolio
    std::map<std::string, StockData> initialPrices;
    for (const auto& ticker : tickers) {
        if (stockData[ticker].empty()) {
            std::cerr << "Error: Empty data for ticker " << ticker << std::endl;
            return 1;
        }
        initialPrices[ticker] = stockData[ticker][0];
    }
    rebalancePortfolio(portfolio, targetAllocation, initialInvestment, initialPrices);

    // Backtesting loop
    std::vector<double> portfolioValues;
    std::vector<double> dailyReturns;
    double previousValue = initialInvestment;

    for (size_t day = 0; day < stockData[tickers[0]].size(); ++day) {
        std::map<std::string, StockData> dailyPrices;
        for (const auto& ticker : tickers) {
            if (day >= stockData[ticker].size()) {
                std::cerr << "Error: Day index out of range for ticker " << ticker << std::endl;
                return 1;
            }
            dailyPrices[ticker] = stockData[ticker][day];
        }

        double portfolioValue = computePortfolioValue(portfolio, dailyPrices);
        portfolioValues.push_back(portfolioValue);

        double dailyReturn = (portfolioValue - previousValue) / previousValue;
        dailyReturns.push_back(dailyReturn);

        previousValue = portfolioValue;

        // Rebalance portfolio (e.g., monthly)
        if (day % 20 == 0) {
            rebalancePortfolio(portfolio, targetAllocation, portfolioValue, dailyPrices);
        }
    }

    // Compute risk metrics
    double volatility = computeVolatility(dailyReturns);
    double maxDrawdown = computeMaxDrawdown(portfolioValues);
    double sharpeRatio = computeSharpeRatio(dailyReturns, 0.01); // Assuming 1% risk-free rate

    // Print results
    std::cout << "Final Portfolio Value: $" << portfolioValues.back() << std::endl;
    std::cout << "Volatility: " << volatility << std::endl;
    std::cout << "Max Drawdown: " << maxDrawdown << std::endl;
    std::cout << "Sharpe Ratio: " << sharpeRatio << std::endl;

    return 0;
}