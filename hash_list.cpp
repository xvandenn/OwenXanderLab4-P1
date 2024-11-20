#include "hash_list.h"

hash_list::hash_list() {}

/**-----------------------------------------------------------------------------------
 * START Part 1
 *------------------------------------------------------------------------------------*/

void hash_list::insert(int key, float value) {
node* start = head;
node* temp = nullptr;

while(start != nullptr){
	if(start->key == key){
		start->value = value;
		return;
	}
	temp = start;
	start = start->next;
}

node* toAdd = new node(key, value);

if(head == nullptr){
	head = toAdd;
}
else {
	temp->next = toAdd;
}

}

std::optional<float> hash_list::get_value(int key) const { 
	node* start = head;
	node* temp = nullptr;

	if(head == nullptr){
		return std::nullopt;
	}

	while(start != nullptr){
		if(start->key == key){
			return start->value;
		}
		temp = start;
		start = start->next;
	}

	return std::nullopt; 
}

bool hash_list::remove(int key) { 
	if(head == nullptr){
		return false;
	}

	node* start = head;
	node* temp = nullptr;

	while(start != nullptr){
		if(start->key = key){
			if(temp == nullptr){
				head = start->next;
			}
			else{
				temp->next = start->next;
			}
			delete start;
			return true;
		}

		temp = start;
		start = start->next;
	}

	return false; }

size_t hash_list::get_size() const { 
	if(head == nullptr){
		return 0;
	}

	node* start = head;
	node* temp = nullptr;
	size_t count = 0;
	while(start != nullptr){
		count++;
		temp = start;
		start = start->next;
	}

	return count; }

hash_list::~hash_list() {

node* start = head;
while(start != nullptr){
	node* next = start->next;
	delete start;
	start = next;
}

}

/**-----------------------------------------------------------------------------------
 * END Part 1
 *------------------------------------------------------------------------------------*/


/**-----------------------------------------------------------------------------------
 * START Part 2
 *------------------------------------------------------------------------------------*/

hash_list::hash_list(const hash_list &other) {}

hash_list &hash_list::operator=(const hash_list &other) { return *this; }

void hash_list::reset_iter() {}


void hash_list::increment_iter() {}


std::optional<std::pair<const int *, float *>> hash_list::get_iter_value() { return std::nullopt; }


bool hash_list::iter_at_end() { return false; }
/**-----------------------------------------------------------------------------------
 * END Part 2
 *------------------------------------------------------------------------------------*/
