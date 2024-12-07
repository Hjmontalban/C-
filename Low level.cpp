#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <cstdlib>
#include <ctime>
#include <iomanip>
#include <sstream> // For string stream formatting

class Restaurant {
private:
    std::string name;
    double costPerOrder;
    double totalSales;
    std::map<std::string, double> menu;
    std::vector<std::string> salesData;

public:
    Restaurant(const std::string& name, double costPerOrder) 
        : name(name), costPerOrder(costPerOrder), totalSales(0.0) {
        menu = {
            {"Pasta", 5.99 * 55},
            {"Burger", 2.99 * 55},
            {"Salad", 1.99 * 55},
            {"Pizza", 10.99 * 55},
            {"Soda", 1.50 * 55},
            {"Coffee", 2.50 * 55}
        };
    }

    void displayMenu() {
        std::cout << "\n----- Menu -----\n";
        int idx = 1;
        for (const auto& item : menu) {
            std::cout << idx++ << ". " << item.first << ": ₱" << std::fixed << std::setprecision(2) << item.second << "\n";
        }
        std::cout << "----------------" << std::endl;
    }

    double randomCustomizeOrder(std::string& size) {
        double extraCost = 0.0;
        std::map<int, std::string> sizeOptions = {{1, "Regular"}, {2, "Medium"}, {3, "Large"}};
        int sizeChoice = rand() % 3 + 1;
        size = sizeOptions[sizeChoice];

        if (rand() % 2 == 1) {
            extraCost += 5.0; // Extra cheese
        }
        if (rand() % 2 == 1) {
            extraCost += 10.0; // Extra toppings
        }
        return extraCost;
    }

    double manualCustomizeOrder(std::string& size) {
        double extraCost = 0.0;
        std::map<int, std::string> sizeOptions = {{1, "Regular"}, {2, "Medium"}, {3, "Large"}};

        std::cout << "Choose size:\n";
        for (const auto& option : sizeOptions) {
            std::cout << option.first << ". " << option.second << "\n";
        }

        int sizeChoice;
        while (true) {
            std::cout << "Enter the size number: ";
            std::cin >> sizeChoice;
            if (sizeOptions.count(sizeChoice)) {
                size = sizeOptions[sizeChoice];
                break;
            } else {
                std::cout << "Invalid choice. Please select a valid size number." << std::endl;
            }
        }

        char addCheese, addToppings;
        std::cout << "Add extra cheese? (Y/N): ";
        std::cin >> addCheese;
        std::cout << "Add extra toppings? (Y/N): ";
        std::cin >> addToppings;

        if (toupper(addCheese) == 'Y') {
            extraCost += 5.0;
        }
        if (toupper(addToppings) == 'Y') {
            extraCost += 10.0;
        }

        return extraCost;
    }

    double applyPromotions(double totalOrders) {
        if (totalOrders > 1000) {
            double discount = totalOrders * 0.10;
            std::cout << "Applying 10% discount of ₱" << std::fixed << std::setprecision(2) << discount << " for orders over ₱1000." << std::endl;
            totalOrders -= discount;
        }
        return totalOrders;
    }

    void takeOrder(int numPeople) {
        displayMenu();
        double totalOrders = 0;
        std::vector<std::string> orderSummary;

        if (numPeople > 10) {
            std::cout << "\n*** Number of people exceeds 10, randomizing orders for all. ***" << std::endl;
            clock_t startTime = clock();

            for (int i = 0; i < numPeople; ++i) {
                std::vector<std::string> personOrders;
                double totalPersonCost = 0;
                int numItems = rand() % 3 + 1;

                for (int j = 0; j < numItems; ++j) {
                    auto it = menu.begin();
                    std::advance(it, rand() % menu.size());
                    std::string orderItem = it->first;
                    double orderAmount = it->second;
                    std::string size;
                    double extraCost = randomCustomizeOrder(size);

                    double totalItemCost = orderAmount + extraCost;

                    // Format costs with two decimal points
                    std::ostringstream extraCostStream, totalCostStream;
                    extraCostStream << std::fixed << std::setprecision(2) << extraCost;
                    std::string formattedExtraCost = extraCostStream.str();

                    totalCostStream << std::fixed << std::setprecision(2) << totalItemCost;
                    std::string formattedTotalItemCost = totalCostStream.str();

                    totalOrders += totalItemCost;
                    totalPersonCost += totalItemCost;

                    personOrders.push_back(orderItem + " (" + size + ", Extra: ₱" + formattedExtraCost + ") - ₱" + formattedTotalItemCost);
                    salesData.push_back(orderItem);
                }

                std::ostringstream totalPersonCostStream;
                totalPersonCostStream << std::fixed << std::setprecision(2) << totalPersonCost;
                std::string formattedTotalPersonCost = totalPersonCostStream.str();

                orderSummary.push_back("\nPerson " + std::to_string(i + 1) + " ordered:");
                orderSummary.insert(orderSummary.end(), personOrders.begin(), personOrders.end());
                orderSummary.push_back("Total for Person " + std::to_string(i + 1) + ": ₱" + formattedTotalPersonCost);
            }

            clock_t endTime = clock();
            std::cout << "\nRandom order generation time: " << double(endTime - startTime) / CLOCKS_PER_SEC << " seconds" << std::endl;
        } else {
            // Manual orders for fewer than 10 people...
        }

        totalSales += totalOrders;
        totalOrders = applyPromotions(totalOrders);
        std::cout << "\nTotal for all orders in this round: ₱" << std::fixed << std::setprecision(2) << totalOrders << std::endl;
        std::cout << "\n----- Order Summary -----" << std::endl;
        for (const auto& detail : orderSummary) {
            std::cout << detail << std::endl;
        }
        std::cout << "--------------------------" << std::endl;
    }

    void checkProfitability() {
        double profit = totalSales - (costPerOrder * salesData.size());
        std::cout << "\nTotal Profit/Loss: ₱" << std::fixed << std::setprecision(2) << profit << std::endl;
    }

    void run() {
        std::cout << "\n***** Welcome to The " << name << " *****" << std::endl;
        while (true) {
            int numPeople;
            while (true) {
                std::cout << "Enter the number of people ordering (up to 100,000): ";
                std::cin >> numPeople;
                if (numPeople >= 1 && numPeople <= 100000) {
                    break;
                } else {
                    std::cout << "Please enter a number between 1 and 100,000." << std::endl;
                }
            }

            takeOrder(numPeople);
            checkProfitability();
            char moreOrders;
            std::cout << "\nWould you like to place more orders? (Y/N): ";
            std::cin >> moreOrders;
            if (toupper(moreOrders) != 'Y') {
                std::cout << "\nThank you for dining at The " << name << "!" << std::endl;
                break;
            }
        }
    }
};

int main() {
    srand(time(0));
    std::string restaurantName = "The C++ Restaurant";
    double costPerOrder;
    std::cout << "Enter the cost incurred by the restaurant per order (in PHP): ";
    std::cin >> costPerOrder;

    Restaurant restaurant(restaurantName, costPerOrder);

    clock_t startTime = clock();
    restaurant.run();
    clock_t endTime = clock();

    std::cout << "\nTotal execution time: " << double(endTime - startTime) / CLOCKS_PER_SEC << " seconds" << std::endl;

    return 0;
}
