#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <regex>

// Struct to store p1 value, uncertainty, and related function name
struct P1Data {
    double p1_value;
    double uncertainty;
    std::string function_name;
};

// Function to reorganize p1 data based on the minimization output
void reorganize_p1() {
    // Open the log file
    std::ifstream infile("l3_Parameters.txt");
    if (!infile) {
        std::cerr << "Error opening file!" << std::endl;
        return;
    }
    std::cout << "File opened successfully." << std::endl;

    std::string line;
    std::regex p1_regex(R"(p1\s*=\s*([\d.]+)\s*\+/-\s*([\d.]+))");  // Regex to match p1 values
    std::regex function_regex(R"(root \[\d+\]\s*(\w+\(\)))");        // Regex to match function calls like ten_mil()
    std::string current_function;  // To store the function name after a dashed line

    std::vector<P1Data> p1_values; // Vector to store p1 values and function names
    bool in_section = false;       // Flag to indicate we're in a section

    // Read the log file line by line
    while (std::getline(infile, line)) {
        // Check for dashed lines
        if (line.find("---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------") != std::string::npos) {
            // New section starts, so reset the flag
            in_section = true;
            continue; // Skip the dashed line itself
        }

        std::smatch match;

        // Look for a function call after dashed lines (if we're in a section)
        if (in_section && std::regex_search(line, match, function_regex)) {
            current_function = match[1]; // Capture the function name (e.g., ten_mil())
            std::cout << "Found function: " << current_function << std::endl;
            in_section = false; // Function found, exit the section
        }

        // Look for p1 value in the blockrun	
        if (std::regex_search(line, match, p1_regex)) {
            double p1_value = std::stod(match[1]);
            double uncertainty = std::stod(match[2]);

            // Store the p1 value and function name
            p1_values.push_back({p1_value, uncertainty, current_function});

            // Print the event p1 value and uncertainty
            std::cout << "p1 = " << p1_value << " +/- " << uncertainty << " from function " << current_function << std::endl;
            std::cout << "--------------------------------------" << std::endl;  // Separator for clarity
        }
    }

    infile.close();
    std::cout << "Finished reading the file." << std::endl;

    // Print out all the p1 values and their associated functions
    for (const auto& p1_data : p1_values) {
        std::cout << "Function: " << p1_data.function_name << std::endl;
        std::cout << "  p1 = " << p1_data.p1_value << " +/- " << p1_data.uncertainty << std::endl;
        std::cout << "--------------------------------------" << std::endl;
    }
}

void runReorganize() {
    reorganize_p1();
}




