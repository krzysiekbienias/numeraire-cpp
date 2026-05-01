#pragma once
#include <iostream>
#include <map>
#include <stdexcept>
#include <string>
#include <vector>

namespace analytics_cli {

struct Args {
    std::string                        product;
    std::map<std::string, std::string> params;
    bool                               showHelp = false;
};

inline void printGeneralHelp() {
    std::cout <<
        "\nUsage: main_analytics --product <name> [--help] [product-specific args]\n"
        "\nAvailable products:\n"
        "  forward_contract   Forward price: F = S * exp(r * T)\n"
        "\nRun with --product <name> --help for product-specific arguments.\n"
        "\nExample:\n"
        "  ./main_analytics --product forward_contract --help\n"
        << std::endl;
}

inline void printProductHelp(const std::string& product) {
    if (product == "forward_contract") {
        std::cout <<
            "\nProduct: forward_contract\n"
            "Formula: F = S * exp(r * T)\n"
            "\nArguments:\n"
            "  --spot    Spot price of the underlying, e.g. 30.0\n"
            "  --rate    Continuously compounded risk-free rate, e.g. 0.05\n"
            "  --start   Start date in DD-MM-YYYY format, e.g. 02-03-2025\n"
            "  --end     Termination date in DD-MM-YYYY format, e.g. 02-07-2025\n"
            "\nExample:\n"
            "  ./main_analytics --product forward_contract --spot 30 --rate 0.05 --start 02-03-2025 --end 02-07-2025\n"
            << std::endl;
    } else {
        std::cout << "No help available for product: " << product << std::endl;
    }
}

inline Args parse(int argc, char* argv[]) {
    Args args;
    std::vector<std::string> tokens(argv + 1, argv + argc);

    for (size_t i = 0; i < tokens.size(); ++i) {
        const auto& t = tokens[i];

        if (t == "--help") {
            args.showHelp = true;
            continue;
        }

        if (t == "--product") {
            if (i + 1 >= tokens.size())
                throw std::invalid_argument("Missing value for --product");
            args.product = tokens[++i];
            continue;
        }

        if (t.rfind("--", 0) == 0) {
            const std::string key = t.substr(2);
            if (i + 1 >= tokens.size())
                throw std::invalid_argument("Missing value for argument: " + t);
            args.params[key] = tokens[++i];
            continue;
        }

        throw std::invalid_argument("Unknown argument: " + t);
    }

    return args;
}

inline std::string require(const Args& args, const std::string& key) {
    auto it = args.params.find(key);
    if (it == args.params.end())
        throw std::invalid_argument("Missing required argument: --" + key);
    return it->second;
}

inline double requireDouble(const Args& args, const std::string& key) {
    return std::stod(require(args, key));
}

} // namespace analytics_cli
