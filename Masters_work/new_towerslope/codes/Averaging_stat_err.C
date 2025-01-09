#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <cmath>
#include <TString.h>

void process_p1_series() {
    // Define the path to the directory containing the input file
    TString directory = "/home/plewis/Master_w/Masters_work/new_towerslope/l3_data/txt_files"; // Update this path with your directory
    TString filepath = TString::Format("%s/U100_mill.txt", directory.Data()); // Replace with your filename if needed

    std::vector<double> p1_values;
    std::vector<double> uncertainties;

    std::ifstream infile(filepath.Data());
    if (!infile.is_open()) {
        std::cerr << "Error: Could not open file at " << filepath << std::endl;
        return;
    }

    std::string line;
    while (std::getline(infile, line)) {
        // Look for lines containing "p_1shift"
        if (line.find("p_1shift") != std::string::npos) {
            std::istringstream iss(line);
            std::string temp;
            double p1, uncertainty;

            // Parse the line to extract p1 and uncertainty
            while (iss >> temp) {
                if (temp == "=") {
                    iss >> p1;         // Read p1 value
                    iss >> temp;       // Skip "±"
                    iss >> uncertainty; // Read uncertainty value
                    p1_values.push_back(p1);
                    uncertainties.push_back(uncertainty);
                    break;
                }
            }
        }
    }
    infile.close();

    if (p1_values.size() < 96) { // Ensure at least 96 values (16 measurements for 6 series)
        std::cerr << "Error: Not enough measurements in file for the required analysis." << std::endl;
        return;
    }

    std::cout << "Series | Mean p1     | Uncertainty" << std::endl;
    std::cout << "-----------------------------------" << std::endl;

    for (int series = 0; series < 6; ++series) {
        double sum_p1 = 0.0;
        double sum_uncertainties_squared = 0.0;

        for (int i = 0; i < 16; ++i) {
            int index = series * 16 + i; // Adjusted grouping to use 16 measurements per series
            sum_p1 += p1_values[index];
            sum_uncertainties_squared += std::pow(uncertainties[index], 2);
        }

        double mean_p1 = sum_p1 / 16;
        double mean_uncertainty = std::sqrt(sum_uncertainties_squared) / 16;

        std::cout << "  " << series + 1 << "    | " << mean_p1 << " | " << mean_uncertainty << std::endl;
    }
}

    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
        























































































































