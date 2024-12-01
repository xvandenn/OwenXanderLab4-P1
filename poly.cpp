#include "poly.h"
#include <thread>

using namespace std;


polynomial::polynomial()
{
	p[0] = 0;
}


polynomial::polynomial(const polynomial &other)
{

	// Threads

	int length = other.p.size();
	int cSize = length / 4;

	auto iter = other.p.begin();

	thread ft(&polynomial::copyRange, this, std::cref(other.p), std::ref(iter), 0, cSize);
	thread st(&polynomial::copyRange, this, std::cref(other.p), std::ref(iter), cSize, 2*cSize);
	thread tt(&polynomial::copyRange, this, std::cref(other.p), std::ref(iter), 2*cSize, 3*cSize);
	thread fft(&polynomial::copyRange, this, std::cref(other.p), std::ref(iter), 3*cSize, length);

	ft.join();
	st.join();
	tt.join();
	fft.join();

}

void polynomial::copyRange(std::map<power, coeff>& data, std::map<power, coeff>::const_iterator& iter, int start, int end){
	int i = 0;
	while(i < (end - start) && iter != data.end()){
		data[iter->first] = iter->second;
		iter++;
		i++;
	}

}


void polynomial::print() const
{
	auto iter = p.end();
	iter--;

	while(iter != p.begin())
	{
		
		if(iter->second != 0)
			std::cout<<iter->second<<"X^"<<iter->first<<" + ";
		iter--;
	}

	if(iter->second != 0)
	{
		std::cout<<iter->second;
		if(iter->first != 0)
			std::cout<<"X^"<<iter->first;
		std::cout<<std::endl;
	}
	else
		std::cout<<"0"<<std::endl;

}


polynomial& polynomial::operator=(const polynomial& other)
{
	p.clear();
	for(auto pair: other.p)
		p[pair.first] = pair.second;
	
	degree = other.degree;
	return *this;
}


polynomial polynomial::operator+(const polynomial& other) const
{
	auto begin1 = p.begin();
	auto end1 = p.end();

	auto begin2 = other.p.begin();
	auto end2 = other.p.end();

	polynomial sum;
	while(begin1 != end1 && begin2 != end2)
	{
		if(begin1->first == begin2->first)
		{
			if(begin1->second != -begin2->second)
				sum.p[begin1->first] = begin1->second + begin2->second;
			begin1++;
			begin2++;
		}
		else if(begin1->first > begin2->first)
		{
			sum.p[begin2->first] = begin2->second;
			begin2++;
		}
		else
		{
			sum.p[begin1->first] = begin1->second;
			begin1++;
		}
	}

	while(begin1 != end1)
	{
		sum.p[begin1->first] = begin1->second;
		begin1++;
	}
	while(begin2 != end2)
	{
		sum.p[begin2->first] = begin2->second;
		begin2++;
	}

	sum.degree = sum.find_degree_of();
	return polynomial(sum);
}


polynomial polynomial::operator+(const int i) const
{
	const polynomial& ref = *this;
	polynomial sum = polynomial(ref);
	sum.p[0] += i;
	sum.degree = sum.find_degree_of();
	return polynomial(sum);
}

polynomial polynomial::operator*(const polynomial& other) const
{
	polynomial temp;
	polynomial product;

	for(auto p1:p)
	{
		for(auto p2:other.p)
		{
			temp.p[p1.first + p2.first] = p1.second * p2.second;
			if(temp.degree < p1.first + p2.first)
				temp.degree = p1.first + p2.first;
		}
		product = product + temp;
		temp.p.clear();
	}
	product.degree = product.find_degree_of();
	return polynomial(product);
}

polynomial polynomial::operator*(const int i) const
{
	polynomial product;
	product = product + i;
	product = (product * *this);
	return polynomial(product);
}


polynomial polynomial::operator%(const polynomial& other) const
{
	if(other.degree == 0 && other.p.at(0) == 0)
		return polynomial(*this);
	
	polynomial r(*this);
	polynomial t;

	while(r.degree != 0 && r.p.at(0) != 0 && r.degree >= other.degree)
	{
		t.p[r.degree - other.degree] = r.p[r.degree] / other.p.at(other.degree);
		t.degree = r.degree - other.degree;
		t = t * -1;
		r = (r + (t * other));
		t.p.clear();
	}
	r.degree = r.find_degree_of();
	return polynomial(r);
}


size_t polynomial::find_degree_of() const
{
	auto itter = p.end();
	itter--;
	while(itter->second == 0 && itter != p.begin())
	{
		itter--;
	}
	return (size_t)itter->first;
}


std::vector<std::pair<power, coeff>> polynomial::canonical_form() const
{
	std::vector<std::pair<power, coeff>> canon;
	auto iter = p.end();
	iter--;
	while(iter != p.begin())
	{
		canon.push_back(*iter);
		iter--;
	}
	if(iter->second != 0 || canon.size() == 0)
		canon.push_back(*iter);
	return canon;
}

polynomial operator+(int i, const polynomial& other)
{
	return other + i;
}


polynomial operator*(int i, const polynomial& other)
{
	return other * i;
}