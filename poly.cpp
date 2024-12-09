#include "poly.h"

// Default constructor
polynomial::polynomial() : degree(0) {
    p[0] = 0; // Represents the zero polynomial
}

// Copy constructor
polynomial::polynomial(const polynomial& other) : p(other.p), degree(other.degree) {}

// Print the polynomial
void polynomial::print() const {
    bool first = true;
    for (const auto& term : p) {
        if (term.second == 0) continue;
        if (!first) std::cout << " + ";
        std::cout << term.second << "X^" << term.first;
        first = false;
    }
    if (first) std::cout << "0"; // Handle zero polynomial
    std::cout << std::endl;
}

// Insert or update a term
void polynomial::insertTerm(power pow, coeff coef) {
    if (coef != 0) {
        p[pow] += coef;
        if (p[pow] == 0) {
            p.erase(pow); // Remove zero p
        } else if (pow > degree) {
            degree = pow; // Update degree if necessary
        }
    }
    updateDegree(); // Ensure degree is up-to-date
}

// Update the degree based on the current p
void polynomial::updateDegree() {
    if (p.empty()) {
        degree = 0;
        return;
    }
    degree = 0;
    for (const auto& term : p) {
        if (term.second != 0 && term.first > degree) {
            degree = term.first;
        }
    }
}

//get rid of some zero p from p
void polynomial::simplify() {
    for(auto iter = p.begin(); iter != p.end();) {
        if (iter->second == 0) {
            iter = p.erase(iter);
        } else {
            iter++;
        }
    }
    if (p.empty()) {
        p[0] = 0; 
    }
    updateDegree();
}


polynomial& polynomial::operator=(const polynomial& other) {
    if (this != &other) {
        p = other.p;
        degree = other.degree;
    }
    return *this;
}

//Addition
polynomial polynomial::operator+(const polynomial& other) const {
    polynomial result(*this);
    for (const auto& term : other.p) {
        result.insertTerm(term.first, term.second);
    }
    return result;
}

polynomial polynomial::operator+(int i) const {
    polynomial result(*this);
    result.insertTerm(0, i);
    return result;
}

polynomial operator+(int constant, const polynomial& other) {
    return other + constant;
}

// Multiplication operators
polynomial polynomial::multNoThread(const polynomial& a, const polynomial& b) const {
    polynomial product;
    for (const auto& term1 : a.p) {
        for (const auto& term2 : b.p) {
            product.insertTerm(term1.first + term2.first, term1.second * term2.second);
        }
    }
    return product;
}

polynomial polynomial::operator*(const polynomial& other) const {
    const int numThreads = 8; // Number of threads to use
    std::vector<std::thread> threads;
    std::vector<std::unordered_map<power, coeff>> partialProduct(numThreads); // putting partial products in vector so I don't need mutex

    auto iter = p.begin();
    size_t opsPerThread = (p.size() + numThreads - 1) / numThreads;

	//testing using lambda instead of separate function
    auto mux = [&/* using ref instead of copy for memory sake */](int currentThread, auto start, auto end) {
        std::unordered_map<power, coeff> threadProduct;

	//just found out about iterator for loops. idk why it wasn't obvious lol
        for (auto iter = start; iter != end; iter++) {
            for (const auto& otherIter : other.p) {
				//breaking up into multiple steps now for debug, because I can't read the computers mind anymore...or my own mind
                power resultPower = iter->first + otherIter.first;
                coeff resultCoeff = iter->second * otherIter.second;
                threadProduct[resultPower] += resultCoeff; 
            }
        }

        //apparently std swap is bad and std move is good. Thank god for online forums and people making my mistakes first
        partialProduct[currentThread] = std::move(threadProduct);
    };

    
    for (int i = 0; i < numThreads; i++) {
        auto start = iter;
        size_t j = 0;
        while (j < opsPerThread && iter != p.end()) {
            iter++;
            j++;
        }
        auto end = iter;
		//testing emplace so i call the constructor instead of passing a copy. because memory and whatnot
        threads.emplace_back(mux, i, start, end);
        if (iter == p.end()) break; //optimized exit flag so i can leave early if i got to the end 
    }

    // Join threads
    for (auto& thread : threads) {
        thread.join();
    }

    // Merge partial results into the final result
    std::unordered_map<power, coeff> combinedProduct;
    for (const auto& partial : partialProduct) {
        for (const auto& partialIter : partial) {
            combinedProduct[partialIter.first] += partialIter.second; // Accumulate terms with the same power
        }
    }

    // Convert combinedProduct to a polynomial object
    polynomial result;
    for (const auto& combinedIter : combinedProduct) {
        if (combinedIter.second != 0) {
            result.insertTerm(combinedIter.first, combinedIter.second);
        }
    }

    return result;
}

polynomial polynomial::operator*(int i) const {
    polynomial product;
    for (const auto& term : p) {
        product.insertTerm(term.first, term.second * i);
    }
    return product;
}

polynomial operator*(int i, const polynomial& other) {
    return other * i;
}

// Modulo operator
polynomial polynomial::operator%(const polynomial& other) const {
    if (other.p.empty() || (other.p.size() == 1 && other.p.begin()->second == 0)) {
		//adding exception so i can see if divide by zero is the crash/???
        throw std::invalid_argument("Division by zero polynomial");
    }

    polynomial remainder(*this);
    while (!remainder.p.empty() && remainder.degree >= other.degree) {
        power degreeDiff = remainder.degree - other.degree;
        coeff coefDiff =  -1 * remainder.p[remainder.degree] / other.p.at(other.degree);
        polynomial temp;
        temp.insertTerm(degreeDiff, coefDiff);
        remainder = remainder + multNoThread(other, temp);
    }
    return remainder;
}

// Find the degree of the polynomial
size_t polynomial::find_degree_of() const {
    return degree;
}

// Return a canonical form as a vector of pairs
std::vector<std::pair<power, coeff>> polynomial::canonical_form() const {
    std::vector<std::pair<power, coeff>> canon;
    for (const auto& term : p) {
        if (term.second != 0) {
            canon.emplace_back(term.first, term.second);
        }
    }
    std::sort(canon.rbegin(), canon.rend()); // Sort in descending order
    return canon;
}
