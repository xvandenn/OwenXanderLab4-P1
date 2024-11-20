#include "poly.h"

polynomial::polynomial(){
canonical_form.pushback({0,0});
}

template <typename Iter>
polynomial(Iter begin, Iter end){
	canonical_form.assign(begin, end);
}

polynomial(const polynomial &other){
	canonical_form = other.canonical_form;
}

void print() const {

	for(auto& term: canonical_form()){
		power pw = term.first();
		coeff co = term.second();

		if(co == 0){
			continue;
		}

		if(co > 0){
			std::cout << " + " << co;
		}
		else{
			std::cout << " - " << (-1 * co);
		}

		if(pw > 0){
			std::cout << "x";
			if(pw > 1){
				std::cout << "^" << pw;
			}
		}

	}

}

polynomial &operator=(const polynomial &other){
if(this != &other){
	canonical_form = other.canonical_form;
}
return *this;
}

size_t find_degree_of(){
if(canonical_form.empty()){
	return 0;
}
return canonical_form.front().first;
}


