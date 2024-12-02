#include "poly.h"
#include <thread>

using namespace std;


polynomial::polynomial()
{
	p.push(std::pair<power, coeff>(0, 0));
}


polynomial::polynomial(const polynomial &other)
{
	p = std::priority_queue<std::pair<power, coeff>>(other.p);
}


void polynomial::print() const
{
	std::priority_queue<std::pair<power, coeff>> temp(p);
	while(!temp.empty())
	{
		std::cout<<temp.top().second<<"X^"<<temp.top().first<<" + ";
		temp.pop();
	}
	std::cout<<std::endl;
}


polynomial& polynomial::operator=(const polynomial& other)
{
	return *this;
}


polynomial polynomial::operator+(const polynomial& other) const
{
	return *this;
}

polynomial polynomial::operator*(const polynomial& other) const
{
	return *this;
}

polynomial polynomial::operator%(const polynomial& other) const
{
	// if(other.degree == 0 && other.p.at(0) == 0)
	// 	return polynomial(*this);
	
	// polynomial r(*this);
	// polynomial t;

	// while(r.degree != 0 && r.p.at(0) != 0 && r.degree >= other.degree)
	// {
	// 	t.p[r.degree - other.degree] = r.p[r.degree] / other.p.at(other.degree);
	// 	t.degree = r.degree - other.degree;
	// 	t = t * -1;
	// 	r = (r + (t * other));
	// 	t.p.clear();
	// }
	// r.degree = r.find_degree_of();
	// return polynomial(r);
	return *this;
}


size_t polynomial::find_degree_of() const
{
	return p.top().first;
}


std::vector<std::pair<power, coeff>> polynomial::canonical_form() const
{
	return std::vector<std::pair<power,coeff>>();
}

polynomial operator+(int i, const polynomial& other)
{
	return other;
}


polynomial operator*(int i, const polynomial& other)
{
	return other;
}