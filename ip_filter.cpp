#include <cstdlib>
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>


class IpAddress {
public:
    IpAddress() = delete;
    IpAddress(size_t n1, size_t n2, size_t n3, size_t n4) 
    : n1_(n1), n2_(n2), n3_(n3), n4_(n4) {}

    size_t get_n1() const { return n1_; }
    size_t get_n2() const { return n2_; }
    size_t get_n3() const { return n3_; }
    size_t get_n4() const { return n4_; }

private:
    size_t n1_;
    size_t n2_;
    size_t n3_;
    size_t n4_;
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

        std::sort(data_.begin(), data_.end(), [](const IpAddress& lhs, const IpAddress& rhs){
            return lhs.get_n1() != rhs.get_n1() ? lhs.get_n1() > rhs.get_n1() :
                lhs.get_n2() != rhs.get_n2() ? lhs.get_n2() > rhs.get_n2() :
                lhs.get_n3() != rhs.get_n3() ? lhs.get_n3() > rhs.get_n3() : 
                lhs.get_n4() > rhs.get_n4();
        });
    }

    std::vector<IpAddress> filter(int n1) {
        std::vector<IpAddress> res;
        std::copy_if (data_.begin(), data_.end(), std::back_inserter(res), 
            [n1](const IpAddress& ip_address){
                return ip_address.get_n1() == n1;
        });

        return res;
    }

    std::vector<IpAddress> filter(int n1, int n2) {
        std::vector<IpAddress> res;
        std::copy_if (data_.begin(), data_.end(), std::back_inserter(res), 
            [n1, n2](const IpAddress& ip_address){
                return ip_address.get_n1() == n1 && ip_address.get_n2() == n2;
        });

        return res;
    }

    std::vector<IpAddress> filter_any(int n) {
        std::vector<IpAddress> res;
        std::copy_if (data_.begin(), data_.end(), std::back_inserter(res), 
            [n](const IpAddress& ip_address){
                return ip_address.get_n1() == n || ip_address.get_n2() == n 
                    || ip_address.get_n3() == n || ip_address.get_n4() == n;
        });

        return res;
    }

private:
    std::vector<IpAddress> data_;
};

std::ostream& operator<<(std::ostream& os, const IpAddress& ip_address) {
    os << ip_address.get_n1() << "." << ip_address.get_n2() << "." <<
        ip_address.get_n3() << "." << ip_address.get_n4();
    
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


