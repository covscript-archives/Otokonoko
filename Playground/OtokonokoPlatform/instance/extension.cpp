/*
* Covariant Script Extension
*
* Licensed under the Covariant Innovation General Public License,
* Version 1.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
*
* https://covariant.cn/licenses/LICENSE-1.0
*
* Unless required by applicable law or agreed to in writing, software
* distributed under the License is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
* See the License for the specific language governing permissions and
* limitations under the License.
*
* Copyright (C) 2019 Michael Lee(李登淳)
* Email: mikecovlee@163.com
* Github: https://github.com/mikecovlee
*/

#include "covscript/impl/impl.hpp"

namespace cs_impl {
	namespace array_cs_ext {
		using namespace cs;

// Element access
		var at(const array &arr, number posit)
		{
			return arr.at(posit);
		}

		var front(const array &arr)
		{
			return arr.front();
		}

		var back(const array &arr)
		{
			return arr.back();
		}

// Iterators
		array::iterator begin(array &arr)
		{
			return arr.begin();
		}

		array::iterator end(array &arr)
		{
			return arr.end();
		}

		array::iterator forward(array::iterator &it)
		{
			return ++it;
		}

		array::iterator backward(array::iterator &it)
		{
			return --it;
		}

		var data(array::iterator &it)
		{
			return *it;
		}

// Capacity
		bool empty(const array &arr)
		{
			return arr.empty();
		}

		number size(const array &arr)
		{
			return arr.size();
		}

// Modifiers
		void clear(array &arr)
		{
			arr.clear();
		}

		array::iterator insert(array &arr, array::iterator &pos,
		                       const var &val
		                      )
		{
			return arr.
			       insert(pos, copy(val)
			             );
		}

		array::iterator erase(array &arr, array::iterator &pos)
		{
			return arr.erase(pos);
		}

		void push_front(array &arr, const var &val)
		{
			arr.push_front(copy(val));
		}

		void pop_front(array &arr)
		{
			arr.pop_front();
		}

		void push_back(array &arr, const var &val)
		{
			arr.push_back(copy(val));
		}

		void pop_back(array &arr)
		{
			arr.pop_back();
		}

// Operations
		var to_hash_map(const array &arr)
		{
			hash_map map;
			for (auto &it:arr) {
				if (it.type() == typeid(pair)) {
					const auto &p = it.const_val<pair>();
					map[p.first] = copy(p.second);
				}
				else
					throw lang_error("Wrong syntax for hash map.");
			}
			return var::make<hash_map>(std::move(map));
		}

		var to_list(const array &arr)
		{
			var lst = var::make<list>(arr.begin(), arr.end());
			lst.detach();
			return std::move(lst);
		}

		void init()
		{
			(*array_iterator_ext)
			.add_var("forward", make_cni(forward, true))
			.add_var("backward", make_cni(backward, true))
			.add_var("data", make_cni(data, true));
			(*array_ext)
			.add_var("iterator", make_namespace(array_iterator_ext))
			.add_var("at", make_cni(at, true))
			.add_var("front", make_cni(front, true))
			.add_var("back", make_cni(back, true))
			.add_var("begin", make_cni(begin, true))
			.add_var("end", make_cni(end, true))
			.add_var("empty", make_cni(empty, true))
			.add_var("size", make_cni(size, true))
			.add_var("clear", make_cni(clear, true))
			.add_var("insert", make_cni(insert, true))
			.add_var("erase", make_cni(erase, true))
			.add_var("push_front", make_cni(push_front, true))
			.add_var("pop_front", make_cni(pop_front, true))
			.add_var("push_back", make_cni(push_back, true))
			.add_var("pop_back", make_cni(pop_back, true))
			.add_var("to_hash_map", make_cni(to_hash_map, true))
			.add_var("to_list", make_cni(to_list, true));
		}

	}
	namespace char_cs_ext {
		using namespace cs;

		bool isalnum(char c)
		{
			return std::isalnum(c);
		}

		bool isalpha(char c)
		{
			return std::isalpha(c);
		}

		bool islower(char c)
		{
			return std::islower(c);
		}

		bool isupper(char c)
		{
			return std::isupper(c);
		}

		bool isdigit(char c)
		{
			return std::isdigit(c);
		}

		bool iscntrl(char c)
		{
			return std::iscntrl(c);
		}

		bool isgraph(char c)
		{
			return std::isgraph(c);
		}

		bool isspace(char c)
		{
			return std::isspace(c);
		}

		bool isblank(char c)
		{
			return std::isblank(c);
		}

		bool isprint(char c)
		{
			return std::isprint(c);
		}

		bool ispunct(char c)
		{
			return std::ispunct(c);
		}

		char tolower(char c)
		{
			return std::tolower(c);
		}

		char toupper(char c)
		{
			return std::toupper(c);
		}

		char from_ascii(number ascii)
		{
			if (ascii < 0 || ascii > 255)
				throw lang_error("Out of range.");
			return static_cast<char>(ascii);
		}

		void init()
		{
			(*char_ext)
			.add_var("isalnum", make_cni(isalnum, true))
			.add_var("isalpha", make_cni(isalpha, true))
			.add_var("islower", make_cni(islower, true))
			.add_var("isupper", make_cni(isupper, true))
			.add_var("isdigit", make_cni(isdigit, true))
			.add_var("iscntrl", make_cni(iscntrl, true))
			.add_var("isgraph", make_cni(isgraph, true))
			.add_var("isspace", make_cni(isspace, true))
			.add_var("isblank", make_cni(isblank, true))
			.add_var("isprint", make_cni(isprint, true))
			.add_var("ispunct", make_cni(ispunct, true))
			.add_var("tolower", make_cni(tolower, true))
			.add_var("toupper", make_cni(toupper, true))
			.add_var("from_ascii", make_cni(from_ascii, true));
		}
	}
	namespace except_cs_ext {
		using namespace cs;

		string what(const lang_error &le)
		{
			return le.what();
		}

		void init()
		{
			except_ext->add_var("what", make_cni(what));
		}
	}
	namespace hash_map_cs_ext {
		using namespace cs;

// Capacity
		bool empty(const hash_map &map)
		{
			return map.empty();
		}

		number size(const hash_map &map)
		{
			return map.size();
		}

// Modifiers
		void clear(hash_map &map)
		{
			map.clear();
		}

		void insert(hash_map &map, const var &key, const var &val)
		{
			if (map.count(key) > 0)
				map.at(key).swap(copy(val), true);
			else
				map.emplace(copy(key), copy(val));
		}

		void erase(hash_map &map, const var &key)
		{
			map.erase(key);
		}

// Lookup
		var at(hash_map &map, const var &key)
		{
			return map.at(key);
		}

		bool exist(hash_map &map, const var &key)
		{
			return map.count(key) > 0;
		}

		void init()
		{
			(*hash_map_ext)
			.add_var("empty", make_cni(empty, true))
			.add_var("size", make_cni(size, true))
			.add_var("clear", make_cni(clear, true))
			.add_var("insert", make_cni(insert, true))
			.add_var("erase", make_cni(erase, true))
			.add_var("at", make_cni(at, true))
			.add_var("exist", make_cni(exist, true));
		}
	}
	namespace list_cs_ext {
		using namespace cs;

// Element access
		var front(const list &lst)
		{
			return lst.front();
		}

		var back(const list &lst)
		{
			return lst.back();
		}

// Iterators
		list::iterator begin(list &lst)
		{
			return lst.begin();
		}

		list::iterator end(list &lst)
		{
			return lst.end();
		}

		list::iterator forward(list::iterator &it)
		{
			return ++it;
		}

		list::iterator backward(list::iterator &it)
		{
			return --it;
		}

		var data(list::iterator &it)
		{
			return *it;
		}

// Capacity
		bool empty(const list &lst)
		{
			return lst.empty();
		}

		number size(const list &lst)
		{
			return lst.size();
		}

// Modifiers
		void clear(list &lst)
		{
			lst.clear();
		}

		list::iterator insert(list &lst, list::iterator &pos, const var &val)
		{
			return lst.insert(pos, copy(val));
		}

		list::iterator erase(list &lst, list::iterator &pos)
		{
			return lst.erase(pos);
		}

		void push_front(list &lst, const var &val)
		{
			lst.push_front(copy(val));
		}

		void pop_front(list &lst)
		{
			lst.pop_front();
		}

		void push_back(list &lst, const var &val)
		{
			lst.push_back(copy(val));
		}

		void pop_back(list &lst)
		{
			lst.pop_back();
		}

// Operations
		void remove(list &lst, const var &val)
		{
			lst.remove(val);
		}

		void reverse(list &lst)
		{
			lst.reverse();
		}

		void unique(list &lst)
		{
			lst.unique();
		}

		void init()
		{
			(*list_iterator_ext)
			.add_var("forward", make_cni(forward, true))
			.add_var("backward", make_cni(backward, true))
			.add_var("data", make_cni(data, true));
			(*list_ext)
			.add_var("iterator", make_namespace(list_iterator_ext))
			.add_var("front", make_cni(front, true))
			.add_var("back", make_cni(back, true))
			.add_var("begin", make_cni(begin, true))
			.add_var("end", make_cni(end, true))
			.add_var("empty", make_cni(empty, true))
			.add_var("size", make_cni(size, true))
			.add_var("clear", make_cni(clear, true))
			.add_var("insert", make_cni(insert, true))
			.add_var("erase", make_cni(erase, true))
			.add_var("push_front", make_cni(push_front, true))
			.add_var("pop_front", make_cni(pop_front, true))
			.add_var("push_back", make_cni(push_back, true))
			.add_var("pop_back", make_cni(pop_back, true))
			.add_var("remove", make_cni(remove, true))
			.add_var("reverse", make_cni(reverse, true))
			.add_var("unique", make_cni(unique, true));
		}
	}
	namespace pair_cs_ext {
		using namespace cs;

		var first(const pair &p)
		{
			return p.first;
		}

		var second(const pair &p)
		{
			return p.second;
		}

		void init()
		{
			(*pair_ext)
			.add_var("first", make_cni(first, true))
			.add_var("second", make_cni(second, true));
		}
	}
	namespace string_cs_ext {
		using namespace cs;

		string append(string &str, const var &val)
		{
			str.append(val.to_string());
			return str;
		}

		string insert(string &str, number posit, const var &val)
		{
			str.insert(posit, val.to_string());
			return str;
		}

		string erase(string &str, number b, number e)
		{
			str.erase(b, e);
			return str;
		}

		string replace(string &str, number posit, number count, const var &val)
		{
			str.replace(posit, count, val.to_string());
			return str;
		}

		string substr(string &str, number b, number e)
		{
			return str.substr(b, e);
		}

		number find(string &str, const string &s, number posit)
		{
			auto pos = str.find(s, posit);
			if (pos == std::string::npos)
				return -1;
			else
				return pos;
		}

		number rfind(string &str, const string &s, number posit)
		{
			std::size_t pos = 0;
			if (posit == -1)
				pos = str.rfind(s, std::string::npos);
			else
				pos = str.rfind(s, posit);
			if (pos == std::string::npos)
				return -1;
			else
				return pos;
		}

		string cut(string &str, number n)
		{
			for (std::size_t i = 0; i < n; ++i)
				str.pop_back();
			return str;
		}

		bool empty(const string &str)
		{
			return str.empty();
		}

		void clear(string &str)
		{
			str.clear();
		}

		number size(const string &str)
		{
			return str.size();
		}

		string tolower(const string &str)
		{
			string s;
			for (auto &ch:str)
				s.push_back(std::tolower(ch));
			return std::move(s);
		}

		string toupper(const string &str)
		{
			string s;
			for (auto &ch:str)
				s.push_back(std::toupper(ch));
			return std::move(s);
		}

		number to_number(const string &str)
		{
			return parse_number(str);
		}

		array split(const string &str, const array &signals)
		{
			array
			arr;
			string buf;
			bool found = false;
			for (auto &ch:str) {
				for (auto &sig:signals) {
					if (ch == sig.const_val<char>()) {
						if (!buf.empty()) {
							arr.push_back(buf);
							buf.clear();
						}
						found = true;
						break;
					}
				}
				if (found)
					found = false;
				else
					buf.push_back(ch);
			}
			if (!buf.empty())
				arr.push_back(buf);
			return std::move(arr);
		}

		void init()
		{
			(*string_ext)
			.add_var("append", make_cni(append, true))
			.add_var("insert", make_cni(insert, true))
			.add_var("erase", make_cni(erase, true))
			.add_var("replace", make_cni(replace, true))
			.add_var("substr", make_cni(substr, true))
			.add_var("find", make_cni(find, true))
			.add_var("rfind", make_cni(rfind, true))
			.add_var("cut", make_cni(cut, true))
			.add_var("empty", make_cni(empty, true))
			.add_var("clear", make_cni(clear, true))
			.add_var("size", make_cni(size, true))
			.add_var("tolower", make_cni(tolower, true))
			.add_var("toupper", make_cni(toupper, true))
			.add_var("to_number", make_cni(to_number, true))
			.add_var("split", make_cni(split, true));
		}
	}

	static bool extensions_initiator = true;

	void init_extensions()
	{
		if (extensions_initiator) {
			extensions_initiator = false;

			except_cs_ext::init();
			char_cs_ext::init();
			string_cs_ext::init();
			list_cs_ext::init();
			array_cs_ext::init();
			pair_cs_ext::init();
			hash_map_cs_ext::init();
		}
	}
}
