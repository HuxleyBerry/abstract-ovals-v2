#ifndef SYMMETRIC_GROUP_ELEMENT_H
#define SYMMETRIC_GROUP_ELEMENT_H

#include <array>
#include <string>
#include <stdexcept>
#include <vector>
#include <sstream>
#include <iostream>
#include <utility>

template <int N>
class SymmetricGroupElement;

template <int N>
SymmetricGroupElement<N> operator*(const SymmetricGroupElement<N>& left, const SymmetricGroupElement<N>& right);

template <int N>
class SymmetricGroupElement {
public:
    SymmetricGroupElement() {
        // identity permutation
        for (int i = 0; i < N; ++i) {
            m_image[i] = i;
        }
    }

    SymmetricGroupElement<N>(std::array<int, N> images): m_image(std::move(images)) {}

    SymmetricGroupElement(const std::string& stringForm) {
        for (int i = 0; i < N; ++i) {
            m_image[i] = i;
        }

        std::vector<size_t> openBracketLocations;
        std::array<bool, N> pointAppearsInString;
        pointAppearsInString.fill(false);
        size_t pos = stringForm.find('(');
        while (pos != std::string::npos) {
            openBracketLocations.push_back(pos);
            pos = stringForm.find('(', pos + 1);
        }
        for (int i = 0; i < openBracketLocations.size(); ++i) {
            size_t closeBracketLocation = i == openBracketLocations.size() - 1 ? stringForm.size() - 1 :  openBracketLocations[i + 1] - 1;
            if (stringForm[closeBracketLocation] != ')') {
                throw std::invalid_argument("Expected a ')'");
            }
            std::istringstream ss(stringForm.substr(openBracketLocations[i] + 1, closeBracketLocation - openBracketLocations[i] - 1));
            std::string currentNumString;
            int prevNum;
            int firstNum;
            bool anyNumsFound = false;
            while (std::getline(ss, currentNumString, ',')) {
                size_t pos;
                int currentNum;
                try {
                    currentNum = std::stoi(currentNumString, &pos);
                } catch (const std::invalid_argument& e) {
                    throw std::invalid_argument("Invalid number: '" + currentNumString + "'");
                }
                if (pos != currentNumString.size()) {
                    throw std::invalid_argument("Invalid number: '" + currentNumString + "'");
                }
                if (currentNum >= N) {
                    throw std::invalid_argument("Permutations should not involve numbers above " + std::to_string(N - 1));
                }
                if (pointAppearsInString[currentNum]) {
                    throw std::invalid_argument("Number '" + currentNumString + "' appears more than once in permutation");
                }
                pointAppearsInString[currentNum] = true;
                if (!anyNumsFound) {
                    firstNum = currentNum ;
                    anyNumsFound = true;
                } else {
                    m_image[prevNum] = currentNum;
                }
                prevNum = currentNum;
            }
            if (anyNumsFound) {
                m_image[prevNum] = firstNum;
            }
        }
    }

    std::string toString() const {
        std::string ans;
        bool movingPointFound = false;
        std::array<bool, N> pointIncluded;
        pointIncluded.fill(false);
        for (size_t pos = 0; pos < N; ++pos){
            if (!pointIncluded[pos] && m_image[pos] != pos) {
                movingPointFound = true;
                ans += "(" + std::to_string(pos);
                int currentInCycle = pos;
                while (true) {
                    pointIncluded[currentInCycle] = true;
                    currentInCycle = m_image[currentInCycle];
                    if (currentInCycle == pos) {
                        break;
                    }
                    ans += ("," + std::to_string(currentInCycle));
                }
                ans += ")";
            }
        }
        if (!movingPointFound) {
            return "()";
        }
        return ans;
    }

    friend SymmetricGroupElement<N> operator* <> (const SymmetricGroupElement<N>& left, const SymmetricGroupElement<N>& right);
    SymmetricGroupElement<N>& operator*=(const SymmetricGroupElement<N>& right) {
        *this = *this * right;
        return this;
    }

    SymmetricGroupElement<N>& invert() {
        std::array<int, N> ans = std::array<int, N>();
        for (int i = 0; i < N; ++i) {
            ans[m_image[i]] = i;
        }
        return *this;
    }

    SymmetricGroupElement<N> inverse() const {
        SymmetricGroupElement<N> copy(*this);
        return copy.invert();
    }

    std::vector<int> getCycleStructure() const {
        std::vector<int> cycleSizes;
        std::array<int, N> cycleStarts;
        cycleStarts.fill(-1);
        for (int i = 0; i < N; ++i) {
            if (cycleStarts[i] == -1) {
                cycleStarts[i] = i;
                int pos = m_image[i];
                while (pos != i) {
                    cycleStarts[pos] = i;
                    pos = m_image[pos];
                }
            }
        }
        std::array<int, N> startCounts;
        startCounts.fill(0);
        for (int i = 0; i < N; ++i) {
            ++startCounts[cycleStarts[i]];
        }
        for (int count: startCounts) {
            if (count >= 2) {
                cycleSizes.push_back(count);
            }
        }
        return cycleSizes;
    }

    int actOn(int p) {
        return m_image[p];
    }

private:
    std::array<int, N> m_image;

    void printInternalArray() {
        for (int num: m_image) {
            std::cout << num << ",";
        }
        std::cout << "\n";
    }
};

// acting from the left
template <int N>
SymmetricGroupElement<N> operator*(const SymmetricGroupElement<N>& left, const SymmetricGroupElement<N>& right) {
    std::array<int, N> ans = std::array<int, N>();
    for (int i = 0; i < N; ++i) {
        ans[i] = left.m_image[right.m_image[i]];
    }
    return SymmetricGroupElement<N>(std::move(ans));
}

// acting from left
template <int N>
SymmetricGroupElement<N> congjugate(const SymmetricGroupElement<N>& toConjugate, const SymmetricGroupElement<N>& toConjugateWith) {
    return toConjugateWith * toConjugate * toConjugateWith.inverse();
}

template <int N>
std::ostream& operator<<(std::ostream& os, const SymmetricGroupElement<N>& sge) {
    return os << sge.toString();
}

#endif