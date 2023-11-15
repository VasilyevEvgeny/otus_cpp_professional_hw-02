#include <cstdlib>
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <array>

static constexpr auto N_BYTES = 4U;

class IpAddress {
public:
    IpAddress() = delete;
    IpAddress(size_t byte_1, size_t byte_2, size_t byte_3, size_t byte_4) 
    : ip_address_{byte_1, byte_2, byte_3, byte_4} {}

    bool operator< (const IpAddress& other) {
        return get_byte_1() != other.get_byte_1() ? get_byte_1() > other.get_byte_1() :
                get_byte_2() != other.get_byte_2() ? get_byte_2() > other.get_byte_2() :
                get_byte_3() != other.get_byte_3() ? get_byte_3() > other.get_byte_3() : 
                get_byte_4() > other.get_byte_4();
    }

    bool operator== (const IpAddress& other) {
        return get_byte_1() == other.get_byte_1() && \
                get_byte_2() == other.get_byte_2() && \
                get_byte_3() == other.get_byte_3() && \
                get_byte_4() == other.get_byte_4();
    }

    size_t get_byte_1() const { return ip_address_[0]; }
    size_t get_byte_2() const { return ip_address_[1]; }
    size_t get_byte_3() const { return ip_address_[2]; }
    size_t get_byte_4() const { return ip_address_[3]; }

private:
    std::array<size_t, N_BYTES> ip_address_;
};

class IpAddressPool {
public:
    IpAddressPool() = default;

    void add(const std::string& str) {
        std::string substr;
        std::vector<int> numbers;
        for (const auto& c : str) {
            if (c != '.') {
                substr += c;
            } else {
                numbers.push_back(std::stoi(substr));
                substr.clear();
            }
        }
        numbers.push_back(std::stoi(substr));

        data_.emplace_back(
            static_cast<size_t>(numbers[0]),
            static_cast<size_t>(numbers[1]),
            static_cast<size_t>(numbers[2]),
            static_cast<size_t>(numbers[3])
        );

        std::sort(data_.begin(), data_.end());
    }

    std::vector<IpAddress> filter(int n1) {
        std::vector<IpAddress> res;
        std::copy_if (data_.begin(), data_.end(), std::back_inserter(res), 
            [n1](const IpAddress& ip_address){
                return ip_address.get_byte_1() == n1;
        });

        return res;
    }

    std::vector<IpAddress> filter(int n1, int n2) {
        std::vector<IpAddress> res;
        std::copy_if (data_.begin(), data_.end(), std::back_inserter(res), 
            [n1, n2](const IpAddress& ip_address){
                return ip_address.get_byte_1() == n1 && ip_address.get_byte_2() == n2;
        });

        return res;
    }

    std::vector<IpAddress> filter_any(int n) {
        std::vector<IpAddress> res;
        std::copy_if (data_.begin(), data_.end(), std::back_inserter(res), 
            [n](const IpAddress& ip_address){
                return ip_address.get_byte_1() == n || ip_address.get_byte_2() == n 
                    || ip_address.get_byte_3() == n || ip_address.get_byte_4() == n;
        });

        return res;
    }

private:
    std::vector<IpAddress> data_;
};

std::ostream& operator<<(std::ostream& os, const IpAddress& ip_address) {
    os << ip_address.get_byte_1() << "." << ip_address.get_byte_2() << "." <<
        ip_address.get_byte_3() << "." << ip_address.get_byte_4();
    
    return os;
}

std::ostream& operator<<(std::ostream& os, const std::vector<IpAddress>& ip_addresses) {
    for (size_t i = 0; i < ip_addresses.size(); ++i) {
        os << ip_addresses[i];
        if (i < ip_addresses.size() - 1) {
           os << std::endl;
        }
    }
    
    return os;
}


int main(int argc, char const *argv[]) {
    IpAddressPool ip_address_pool;
    
    for (std::string line; std::getline(std::cin, line);) {
        auto pos = line.find_first_of('\t', 0U);
        ip_address_pool.add(line.substr(0U, pos));
    }

    std::cout << ip_address_pool.filter(1) << std::endl;
    std::cout << ip_address_pool.filter(46, 70) << std::endl;
    std::cout << ip_address_pool.filter_any(46) << std::endl;
    
    return 0;
}


