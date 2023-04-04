//
// Created by UraraO on 2023/4/2.
//

#ifndef SKIPLIST_SKIPLIST_H
#define SKIPLIST_SKIPLIST_H

/*
// 对模板参数T类型做出要求，需要C++20
#include <vector>
#include <array>

template <class T>
concept Check = requires {
	T().clear();
};

template <Check T>
struct G {};

G<std::vector<char>> x;      // 成功
G<std::string> y;            // 成功
*/

#include <iostream>
#include <vector>
#include <unordered_map>
#include <cstdlib>
#include <cmath>
#include <cstring>
#include <fstream>
#include <string>
#include <memory>
#include <random>
#include <ctime>
#include "mutex_lock.h"

// path of dumpfile
// dumpfile create by skiplist::dump_file()
// and read by skiplist::load_file()
#define DUMP_FILE_PATH "store/dumpfile"

using std::string;
using std::vector;
using std::unordered_map;
using std::cin;
using std::cout;
using std::endl;
using std::shared_ptr;

// global delimiter, shown as key:balue,like 1:"hello world"
const string delimiter = ":";

// require T can be use like T(a) == T(b)
// and T(a) < T(b)
// in skiplist, it requires key_type K to implement == and <
template<class T>
concept CheckType = requires(T a, T b) {
	a == b;
	a < b;
};

/*
 * skiplist Node impl
 * */
template<class V, CheckType K = int>
class Node {
public:
	Node() = default;

	explicit Node(int level) : node_level(level) {
		next_p = std::make_shared<vector<Node<V, K> *> >();
	}

	Node(K k, V v, int level);

	~Node() = default;

	K get_key();

	V get_value();

	void set_value(const V &);

	int level();

public:
	shared_ptr<vector<Node<V, K> *> > next_p;
private:
	K key;
	V value;
	int node_level = 0;
};

template<class V, CheckType K>
K Node<V, K>::get_key() {
	return key;
}

template<class V, CheckType K>
V Node<V, K>::get_value() {
	return value;
}

template<class V, CheckType K>
void Node<V, K>::set_value(const V &val) {
	value = val;
}

template<class V, CheckType K>
int Node<V, K>::level() {
	return node_level;
}

template<class V, CheckType K>
Node<V, K>::Node(K k, V v, int level) :
		key(k), value(v), node_level(level) {
	next_p = std::make_shared<vector<Node<V, K> *> >();
	for(int i = 0; i < level; ++i) {
		next_p->push_back(nullptr);
	}
}

/*
 * skiplist impl
 *
 *
 * */
template<class V, CheckType K = int>
class skiplist {
public:
	explicit skiplist(int max_level);

	~skiplist();

	int random_level();

	int insert(K, V);

	void print();

	Node<V, K> *find(K);

	int erase(K);

	void dump_file();

	void load_file();

	int size() { return element_num; }

private:
	shared_ptr<Node<V, K> > create_node(K, V, int);
	void get_key_value_from_string(const string &str, string &key, string &value);

	bool is_valid_string(const string &str);

private:
	int _max_level;
	int _skiplist_level;
	shared_ptr<Node<V, K> > _header;
	shared_ptr<unordered_map<K, shared_ptr<Node<V, K> > > > _nodes;
	int element_num;

	std::ofstream _file_writer;
	std::ifstream _file_reader;

	mutex_lock m_mutex;

};

template<class V, CheckType K>
void skiplist<V, K>::get_key_value_from_string(const string &str, string &key, string &value) {
	if(!is_valid_string(str)) {
		return;
	}
	size_t sep = str.find(delimiter);
	key = str.substr(0, sep);
	value = str.substr(sep + 1, str.length());
}

template<class V, CheckType K>
bool skiplist<V, K>::is_valid_string(const string &str) {
	if(str.empty()) {
		return false;
	}
	if(str.find(delimiter) == string::npos) {
		return false;
	}
	return true;
}

template<class V, CheckType K>
skiplist<V, K>::skiplist(int max_level) {
	_max_level = max_level;
	_skiplist_level = 0;
	element_num = 0;
	_header = std::make_shared<Node<V, K> >(max_level);
	_nodes = std::make_shared<unordered_map<K, shared_ptr<Node<V, K> > > >();
	for(int i = 0; i < max_level; ++i) {
		_header->next_p->push_back(nullptr);
	}
}

template<class V, CheckType K>
skiplist<V, K>::~skiplist() {
	if (_file_writer.is_open()) {
		_file_writer.close();
	}
	if (_file_reader.is_open()) {
		_file_reader.close();
	}
}

template<class V, CheckType K>
int skiplist<V, K>::random_level() {
	int random_level = 1;
	time_t seed = time(nullptr);
	static std::default_random_engine enge(seed); // NOLINT(cert-msc51-cpp)
	static std::uniform_int_distribution<int> u(0, 1);

	while (u(enge) && random_level < _max_level)
	{
		++random_level;
	}

	return random_level;
}

template<class V, CheckType K>
shared_ptr<Node<V, K> > skiplist<V, K>::create_node(K k, V v, int level) {
	return std::make_shared<Node<V, K> >(k, v, level);
}

template<class V, CheckType K>
void skiplist<V, K>::print() {
	for (int i = _skiplist_level - 1; i >= 0; --i) {
		auto node = _header->next_p->at(i);
		cout << "Level " << i << ": ";
		while (node != nullptr) {
			cout << node->get_key() << ":" << node->get_value() << ";";
			node = node->next_p->at(i);
		}
		cout << endl;
	}
}

template<class V, CheckType K>
int skiplist<V, K>::insert(K k, V v) {
	mutex_lock_guard lock(m_mutex);
	Node<V, K> *cur = _header.get();
	Node<V, K> *update[_max_level];
	for(int i = _skiplist_level - 1; i >= 0; --i) {
		while(cur->next_p->at(i) != nullptr && cur->next_p->at(i)->get_key() < k) {
			cur = cur->next_p->at(i);
		}
		update[i] = cur;
	}

	cur = cur->next_p->at(0);
	if (cur && cur->get_key() == k) {
		cout << "key: " << k << ", exists" << endl;
		return 1;
	}

	if (cur == nullptr || cur->get_key() != k) {

		int level = random_level();
		auto ins_node = create_node(k, v, level);
		if (level > _skiplist_level) {
			for (int i = level - 1; i != _skiplist_level - 1; --i) {
				update[i] = _header.get();
			}
			_skiplist_level = level;
		}

		for(int i = 0; i < level; ++i) {
			ins_node->next_p->at(i) = update[i]->next_p->at(i);
			update[i]->next_p->at(i) = ins_node.get();
		}
		_nodes->insert({k, ins_node});
		++element_num;

		cout << "Successfully inserted key:" << k << ", value:" << v << endl;
	}

	return 0;
}

template<class V, CheckType K>
Node<V, K> *skiplist<V, K>::find(K k) {

	mutex_lock_guard lock(m_mutex);
	Node<V, K> *cur = _header.get();
	for(int i = _skiplist_level - 1; i >= 0; --i) {
		while(cur->next_p->at(i) != nullptr && cur->next_p->at(i)->get_key() < k) {
			cur = cur->next_p->at(i);
		}
	}
	cur = cur->next_p->at(0);
	if(cur && cur->get_key() == k) {
		cout << "Found key: " << k << " , value: " << cur->get_value() << endl;
		return cur;
	}
	cout << "Not Found key: " << k << endl;
	return nullptr;
}

template<class V, CheckType K>
int skiplist<V, K>::erase(K k) {
	mutex_lock_guard lock(m_mutex);
	Node<V, K> *cur = _header.get();
	Node<V, K> *update[_max_level];
	memset(update, 0, sizeof(Node<V, K>*) * (_max_level));
	for(int i = _skiplist_level - 1; i >= 0; --i) {
		while(cur->next_p->at(i) && cur->next_p->at(i)->get_key() < k) {
			cur = cur->next_p->at(i);
		}
		update[i] = cur;
	}
	cur = cur->next_p->at(0);
	if (cur != nullptr && cur->get_key() == k) {

		for (int i = 0; i < _skiplist_level; ++i) {
			if (update[i]->next_p->at(i) != cur) {
				break;
			}
			update[i]->next_p->at(i) = cur->next_p->at(i);
		}

		// Remove levels which have no elements
		while (_skiplist_level > 0 && _header->next_p->at(_skiplist_level - 1) == nullptr) {
			--_skiplist_level;
		}
		--element_num;
		_nodes->erase(k);
		cout << "Successfully deleted key "<< k << endl;
	} else {
		cout << "delete key fail, key:"<< k << " not exist" << endl;
	}
	return 0;
}

template<class V, CheckType K>
void skiplist<V, K>::dump_file() {
	_file_writer.open(DUMP_FILE_PATH);
	Node<K, V> *node = _header->next_p[0];
	std::cout << "dump_file-----------------" << std::endl;

	while (node != nullptr) {
		_file_writer << node->get_key() << ":" << node->get_value() << "\n";
		std::cout << node->get_key() << ":" << node->get_value() << ";\n";
		node = node->next_p[0];
	}

	_file_writer.flush();
	_file_writer.close();
}

template<class V, CheckType K>
void skiplist<V, K>::load_file() {
	_file_reader.open(DUMP_FILE_PATH);
	cout << "load_file-----------------" << endl;
	string line;
	string key;
	string value;
	while (getline(_file_reader, line)) {
		get_key_value_from_string(line, key, value);
		if (key.empty() || value.empty()) {
			continue;
		}
		insert(key, value);
		cout << "key:" << key << "value:" << value << std::endl;
	}
	_file_reader.close();
}

#endif //SKIPLIST_SKIPLIST_H













