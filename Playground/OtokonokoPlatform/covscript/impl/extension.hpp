#pragma once
/*
* Covariant Script Core Extension
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
#include "covscript/core/core.hpp"
#include "covscript/core/cni.hpp"

// Basic Type Support

namespace cs_impl {
// Detach
	template<>
	void detach<cs::pair>(cs::pair &val)
	{
		cs::copy_no_return(val.first);
		cs::copy_no_return(val.second);
	}

	template<>
	void detach<cs::list>(cs::list &val)
	{
		for (auto &it:val)
			cs::copy_no_return(it);
	}

	template<>
	void detach<cs::array>(cs::array &val)
	{
		for (auto &it:val)
			cs::copy_no_return(it);
	}

	template<>
	void detach<cs::hash_map>(cs::hash_map &val)
	{
		for (auto &it:val)
			cs::copy_no_return(it.second);
	}

// To String
	template<>
	std::string to_string<cs::number>(const cs::number &val)
	{
        std::string output(50, '\0');
        snprintf(&output[0], 50, "%Lf", val);
        return std::move(output);
	}

	template<>
	std::string to_string<char>(const char &c)
	{
		return std::string(1, c);
	}

	template<>
	std::string to_string<cs::type_id>(const cs::type_id &id)
	{
		if (id.type_hash != 0)
			return std::string(id.type_idx.name()) + "_" + to_string(id.type_hash);
		else
			return id.type_idx.name();
	}

	template<>
	std::string to_string<cs::range_type>(const cs::range_type &range)
	{
		if (range.empty())
			return "[]";
		std::string str = "[";
		for (cs::number it:range)
			str += to_string(it) + ", ";
		str.resize(str.size() - 2);
		str += "]";
		return std::move(str);
	}

// To Integer
	template<>
	long to_integer<std::string>(const std::string &str)
	{
		for (auto &ch:str) {
			if (!std::isdigit(ch))
				throw cs::runtime_error("Wrong literal format.");
		}
		return std::stol(str);
	}

// Type name

	template<>
	constexpr const char *get_name_of_type<cs::var>()
	{
		return "cs::var";
	}

	template<>
	constexpr const char *get_name_of_type<cs::number>()
	{
		return "cs::number";
	}

	template<>
	constexpr const char *get_name_of_type<cs::boolean>()
	{
		return "cs::boolean";
	}

	template<>
	constexpr const char *get_name_of_type<cs::pointer>()
	{
		return "cs::pointer";
	}

	template<>
	constexpr const char *get_name_of_type<char>()
	{
		return "cs::char";
	}

	template<>
	constexpr const char *get_name_of_type<cs::string>()
	{
		return "cs::string";
	}

	template<>
	constexpr const char *get_name_of_type<cs::list>()
	{
		return "cs::list";
	}

	template<>
	constexpr const char *get_name_of_type<cs::array>()
	{
		return "cs::array";
	}

	template<>
	constexpr const char *get_name_of_type<cs::pair>()
	{
		return "cs::pair";
	}

	template<>
	constexpr const char *get_name_of_type<cs::hash_map>()
	{
		return "cs::hash_map";
	}

	template<>
	constexpr const char *get_name_of_type<cs::type_t>()
	{
		return "cs::type";
	}

	template<>
	constexpr const char *get_name_of_type<cs::namespace_t>()
	{
		return "cs::namespace";
	}

	template<>
	constexpr const char *get_name_of_type<cs::callable>()
	{
		return "cs::function";
	}

	template<>
	constexpr const char *get_name_of_type<cs::range_type>()
	{
		return "cs::range";
	}

	template<>
	constexpr const char *get_name_of_type<cs::structure>()
	{
		return "cs::structure";
	}

	template<>
	constexpr const char *get_name_of_type<cs::lang_error>()
	{
		return "cs::exception";
	}



// Extended Type Support

	extern cs::namespace_t except_ext;
	extern cs::namespace_t array_ext;
	extern cs::namespace_t array_iterator_ext;
	extern cs::namespace_t char_ext;
	extern cs::namespace_t list_ext;
	extern cs::namespace_t list_iterator_ext;
	extern cs::namespace_t hash_map_ext;
	extern cs::namespace_t pair_ext;
	extern cs::namespace_t string_ext;

	template<>
	cs::namespace_t &get_ext<cs::lang_error>()
	{
		return except_ext;
	}

	template<>
	cs::namespace_t &get_ext<cs::array>()
	{
		return array_ext;
	}

	template<>
	cs::namespace_t &get_ext<cs::array::iterator>()
	{
		return array_iterator_ext;
	}

	template<>
	constexpr const char *get_name_of_type<cs::array::iterator>()
	{
		return "cs::array::iterator";
	}

	template<>
	cs::namespace_t &get_ext<char>()
	{
		return char_ext;
	}

	template<>
	cs::namespace_t &get_ext<cs::hash_map>()
	{
		return hash_map_ext;
	}

	template<>
	cs::namespace_t &get_ext<cs::list>()
	{
		return list_ext;
	}

	template<>
	cs::namespace_t &get_ext<cs::list::iterator>()
	{
		return list_iterator_ext;
	}

	template<>
	constexpr const char *get_name_of_type<cs::list::iterator>()
	{
		return "cs::list::iterator";
	}

	template<>
	cs::namespace_t &get_ext<cs::pair>()
	{
		return pair_ext;
	}

	template<>
	constexpr const char *get_name_of_type<cs::tree_type<cs::token_base *>>()
	{
		return "cs::expression";
	}

	template<>
	cs::namespace_t &get_ext<cs::string>()
	{
		return string_ext;
	}

	void init_extensions();
}
