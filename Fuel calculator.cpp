#include <iostream>
#include <string>
#include <unordered_map>
#include <iomanip>
#include <chrono>  // For measuring execution time

using namespace std;

// Structure to hold car type information
struct CarType {
    double fuel_efficiency;    // in km/l
    double weight;             // in kg
    int passenger_capacity;    // number of passengers
};

// Define the car types and their attributes
unordered_map<string, CarType> car_types = {
    {"sedan", {15.0, 1500.0, 5}},
    {"suv", {8.0, 2000.0, 7}},
    {"truck", {20.0, 3000.0, 2}}
};

// Define fuel types and their prices per liter
unordered_map<string, double> fuel_types = {
    {"gasoline", 58.50},
    {"diesel", 51.00}
};

// Define road conditions and their effect on fuel efficiency
unordered_map<string, double> road_conditions = {
    {"normal", 1.0},
    {"uphill", 1.25},
    {"rough", 1.15}
};

// Function to adjust fuel efficiency based on weight, passengers, and road type
double adjust_fuel_efficiency(double base_efficiency, double car_weight, int passengers, string road_type) {
    double weight_factor = 1 + (car_weight - 1500.0) / 10000.0;  // Adjust based on weight
    double passenger_factor = 1 + (passengers / 10.0);           // Adjust based on passengers
    double road_factor = road_conditions[road_type];             // Adjust based on road condition

    // Return the adjusted fuel efficiency
    return base_efficiency / (weight_factor * passenger_factor * road_factor);
}

// Function to calculate fuel consumption and cost for a given trip
pair<double, double> calculate_fuel_consumption(string car_type, string fuel_type, double distance_km, int passengers, string road_type) {
    // Validate car type
    if (car_types.find(car_type) == car_types.end()) {
        throw invalid_argument("Car type '" + car_type + "' is not supported.");
    }
    // Validate fuel type
    if (fuel_types.find(fuel_type) == fuel_types.end()) {
        throw invalid_argument("Fuel type '" + fuel_type + "' is not supported.");
    }

    // Retrieve car info based on the selected car type
    CarType car_info = car_types[car_type];
    double base_efficiency = car_info.fuel_efficiency;
    double car_weight = car_info.weight;

    // Adjust fuel efficiency based on weight, passengers, and road type
    double adjusted_efficiency = adjust_fuel_efficiency(base_efficiency, car_weight, passengers, road_type);

    // Calculate the fuel needed and total cost
    double fuel_needed = distance_km / adjusted_efficiency;
    double fuel_cost = fuel_needed * fuel_types[fuel_type];

    return {fuel_needed, fuel_cost};  // Return fuel needed and cost as a pair
}

// Structure to hold prediction results for fuel consumption
struct TravelPrediction {
    double total_distance;
    double current_fuel_needed;
    double future_fuel_needed;
    double total_fuel_needed;
    double current_fuel_cost_php;
    double future_fuel_cost_php;
    double total_fuel_cost_php;
};

// Function to predict future travel fuel consumption and costs
TravelPrediction predict_future_travel(string car_type, string fuel_type, double current_distance, double future_distance, int passengers, string road_type) {
    // Calculate fuel and cost for current and future travel
    auto [current_fuel_needed, current_fuel_cost] = calculate_fuel_consumption(car_type, fuel_type, current_distance, passengers, road_type);
    auto [future_fuel_needed, future_fuel_cost] = calculate_fuel_consumption(car_type, fuel_type, future_distance, passengers, road_type);

    // Calculate totals for distance, fuel, and cost
    double total_distance = current_distance + future_distance;
    double total_fuel_needed = current_fuel_needed + future_fuel_needed;
    double total_fuel_cost = current_fuel_cost + future_fuel_cost;

    // Return the prediction results
    return {total_distance, current_fuel_needed, future_fuel_needed, total_fuel_needed, current_fuel_cost, future_fuel_cost, total_fuel_cost};
}

int main() {
    // Start timing the execution
    auto start = chrono::high_resolution_clock::now();  

    int car_type_num, fuel_type_num, road_type_num;
    double current_distance, future_distance;
    int passengers;

    // Prompt the user to input car type
    cout << "Enter car type (1 for sedan, 2 for suv, 3 for truck): ";
    cin >> car_type_num;
    string car_type;
    switch (car_type_num) {
        case 1:
            car_type = "sedan";
            break;
        case 2:
            car_type = "suv";
            break;
        case 3:
            car_type = "truck";
            break;
        default:
            cerr << "Invalid car type. Exiting.\n";
            exit(1);  
    }

    // Prompt the user to input fuel type
    cout << "Enter fuel type (1 for gasoline, 2 for diesel): ";
    cin >> fuel_type_num;
    string fuel_type;
    switch (fuel_type_num) {
        case 1:
            fuel_type = "gasoline";
            break;
        case 2:
            fuel_type = "diesel";
            break;
        default:
            cerr << "Invalid fuel type. Exiting.\n";
            exit(1);  
    }

    // Prompt for travel distances and number of passengers
    cout << "Enter current travel distance (in km): ";
    cin >> current_distance;
    cout << "Enter future travel distance to predict (in km): ";
    cin >> future_distance;
    cout << "Enter the number of passengers: ";
    cin >> passengers;

    // Prompt the user to input road type
    cout << "Enter road type (1 for normal, 2 for uphill, 3 for rough): ";
    cin >> road_type_num;
    string road_type;
    switch (road_type_num) {
        case 1:
            road_type = "normal";
            break;
        case 2:
            road_type = "uphill";
            break;
        case 3:
            road_type = "rough";
            break;
        default:
            cerr << "Invalid road type. Exiting.\n";
            exit(1);  
    }

    // Try to calculate and display the fuel predictions
    try {
        TravelPrediction result = predict_future_travel(car_type, fuel_type, current_distance, future_distance, passengers, road_type);

        // Display the results for current and future travel
        cout << fixed << setprecision(2);
        cout << "\nFor a " << car_type << " running on " << fuel_type << ":\n";
        cout << "Current distance traveled: " << current_distance << " km\n";
        cout << "Fuel needed for current distance: " << result.current_fuel_needed << " liters\n";
        cout << "Fuel cost for current distance: PHP " << result.current_fuel_cost_php << "\n\n";

        cout << "Predicted future distance: " << future_distance << " km\n";
        cout << "Fuel needed for future distance: " << result.future_fuel_needed << " liters\n";
        cout << "Fuel cost for future distance: PHP " << result.future_fuel_cost_php << "\n\n";

        cout << "Total distance to travel: " << result.total_distance << " km\n";
        cout << "Total fuel needed: " << result.total_fuel_needed << " liters\n";
        cout << "Total fuel cost: PHP " << result.total_fuel_cost_php << "\n";

    } catch (const invalid_argument &e) {
        cerr << e.what() << '\n';
    }

    // Stop timing the execution and display the execution time
    auto stop = chrono::high_resolution_clock::now();  
    auto duration = chrono::duration_cast<chrono::microseconds>(stop - start);

    cout << "\nExecution time: " << duration.count() << " microseconds\n";  

    return 0;
}
