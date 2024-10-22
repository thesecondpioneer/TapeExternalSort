#include <iostream>
#include <random>
#include <fstream>

int main(int argc, char** argv) {
    if (argc != 3){
        std::cout << "Usage: tape_generator <output_file_name> <tape size>";
    }
    std::ofstream fout((std::string(argv[1])));
    int n = atoi(argv[2]);

    std::random_device device;
    std::mt19937 rng(device());
    std::uniform_int_distribution<> dist(-2147483648, 2147483647);

    for(int i = 0; i < n; i++){
        fout << dist(rng) << ' ';
    }
    return 0;
}
