﻿/*
* Covariant Script Instance
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
#include "covscript/covscript.hpp"

namespace cs {
	const std::string &statement_base::get_file_path() const noexcept
	{
		return context->file_path;
	}

	const std::string &statement_base::get_package_name() const noexcept
	{
		return context->package_name;
	}

	const std::string &statement_base::get_raw_code() const noexcept
	{
		return context->file_buff.at(line_num - 1);
	}

	void instance_type::interpret() {
		// Run the instruction
		for (auto &ptr:statements) {
			try {
				ptr->run();
			}
			catch (const lang_error &le) {
				throw fatal_error(std::string("Uncaught exception: ") + le.what());
			}
			catch (const cs::exception &e) {
				throw e;
			}
			catch (const std::exception &e) {
				throw exception(ptr->get_line_num(), ptr->get_file_path(), ptr->get_raw_code(), e.what());
			}
		}
	}

	void instance_type::dump_ast(std::ostream &stream) {
		stream << "< Covariant Script AST Dump >\n< BeginMetaData >\n< Version: " << current_process->version
		       << " >\n< Standard Version: "
		       << current_process->std_version
		       << " >\n< Import Path: \""
		       << current_process->import_path
		       << "\" >\n";
        stream << "< Platform: Otokonoko no Sekai >\n";
		stream << "< EndMetaData >\n";
		for (auto &ptr:statements)
			ptr->dump(stream);
		stream << std::flush;
	}

	void instance_type::check_declar_var(tree_type<token_base *>::iterator it, bool regist) {
		if (it.data() == nullptr)
			throw internal_error("Null pointer accessed.");
		if (it.data()->get_type() == token_types::parallel) {
			auto &parallel_list = static_cast<token_parallel *>(it.data())->get_parallel();
			for (auto &t:parallel_list)
				check_declar_var(t.root(), regist);
		}
		else {
			token_base *root = it.data();
			if (root == nullptr || root->get_type() != token_types::id)
				throw runtime_error("Wrong grammar for variable declaration.");
			if (regist)
				storage.add_record(static_cast<token_id *>(root)->get_id());
		}
	}

	void instance_type::check_define_var(tree_type<token_base *>::iterator it, bool regist, bool constant) {
		if (it.data() == nullptr)
			throw internal_error("Null pointer accessed.");
		if (it.data()->get_type() == token_types::parallel) {
			auto &parallel_list = static_cast<token_parallel *>(it.data())->get_parallel();
			for (auto &t:parallel_list)
				check_define_var(t.root(), regist, constant);
		}
		else {
			token_base *root = it.data();
			if (root == nullptr)
				throw internal_error("Null pointer accessed.");
			if (root->get_type() != token_types::signal)
				throw runtime_error("Wrong grammar for variable definition(1).");
			switch (static_cast<token_signal *>(root)->get_signal()) {
			case signal_types::asi_: {
				token_base *left = it.left().data();
				token_base *right = it.right().data();
				if (left == nullptr || right == nullptr || left->get_type() != token_types::id)
					throw runtime_error("Wrong grammar for variable definition(2).");
				if (constant && right->get_type() != token_types::value)
					throw runtime_error("Wrong grammar for constant variable definition(3).");
				if (regist)
					storage.add_record(static_cast<token_id *>(left)->get_id());
				break;
			}
			case signal_types::bind_: {
				token_base *right = it.right().data();
				if (constant && (right == nullptr || right->get_type() != token_types::value))
					throw runtime_error("Wrong grammar for constant variable definition(4).");
				check_define_structured_binding(it.left(), regist);
				break;
			}
			default:
				throw runtime_error("Wrong grammar for variable definition(5).");
			}
		}
	}

	void instance_type::parse_define_var(tree_type<token_base *>::iterator it, bool constant) {
		if (it.data()->get_type() == token_types::parallel) {
			auto &parallel_list = static_cast<token_parallel *>(it.data())->get_parallel();
			for (auto &t:parallel_list)
				parse_define_var(t.root(), constant);
		}
		else {
			token_base *root = it.data();
			switch (static_cast<token_signal *>(root)->get_signal()) {
			case signal_types::asi_: {
				const var &val = constant ? static_cast<token_value *>(it.right().data())->get_value() : parse_expr(
				                     it.right());
				storage.add_var(static_cast<token_id *>(it.left().data())->get_id(), constant ? val : copy(val),
				                constant);
				break;
			}
			case signal_types::bind_: {
				parse_define_structured_binding(it, constant);
				break;
			}
			default:
				throw runtime_error("Wrong grammar for variable definition(6).");
			}
		}
	}

	void
	instance_type::check_define_structured_binding(tree_type<token_base *>::iterator it, bool regist) {
		for (auto &p_it:static_cast<token_parallel *>(it.data())->get_parallel()) {
			token_base *root = p_it.root().data();
			if (root == nullptr)
				throw runtime_error("Wrong grammar for variable definition(7).");
			if (root->get_type() != token_types::id) {
				if (root->get_type() == token_types::parallel)
					check_define_structured_binding(p_it.root(), regist);
				else
					throw runtime_error("Wrong grammar for variable definition(8).");
			}
			else if (regist)
				storage.add_record(static_cast<token_id *>(root)->get_id());
		}
	}

	void instance_type::parse_define_structured_binding(tree_type<token_base *>::iterator it, bool constant) {
		std::function<void(tree_type<token_base *>::iterator, const var &)> process;
		process = [&process, this, constant](tree_type<token_base *>::iterator it, const var &val) {
			auto &pl = static_cast<token_parallel *>(it.data())->get_parallel();
			if (val.type() != typeid(array))
				throw runtime_error("Only support structured binding with array while variable definition.");
			auto &arr = val.const_val<array>();
			if (pl.size() != arr.size())
				throw runtime_error("Unmatched structured binding while variable definition.");
			for (std::size_t i = 0; i < pl.size(); ++i) {
				if (pl[i].root().data()->get_type() == token_types::parallel)
					process(pl[i].root(), arr[i]);
				else
					storage.add_var(static_cast<token_id *>(pl[i].root().data())->get_id(),
					                constant ? arr[i] : copy(arr[i]), constant);
			}
		};
		const var &val = constant ? static_cast<token_value *>(it.right().data())->get_value() : parse_expr(it.right());
		process(it.left(), val);
	}

	void repl::run(const string &code) {
		if (code.empty())
			return;
		std::deque<char> buff;
		for (auto &ch:code)
			buff.push_back(ch);
		statement_base *statement = nullptr;
		try {
			std::deque<token_base *> line;
			context->compiler->clear_metadata();
			context->compiler->build_line(buff, line);
			method_base *m = context->compiler->match_method(line);
			switch (m->get_type()) {
			case method_types::null:
				throw runtime_error("Null type of grammar.");
				break;
			case method_types::single: {
				if (level > 0) {
					if (m->get_target_type() == statement_types::end_) {
						context->instance->storage.remove_set();
						context->instance->storage.remove_domain();
						--level;
					}
					if (level == 0) {
						if (m->get_target_type() == statement_types::end_)
							statement = static_cast<method_end *>(m)->translate_end(method, context, tmp, line);
						else
							statement = method->translate(context, tmp);
						tmp.clear();
						method = nullptr;
					}
					else {
						m->preprocess(context, {line});
						tmp.push_back(line);
					}
				}
				else {
					if (m->get_target_type() == statement_types::end_)
						throw runtime_error("Hanging end statement.");
					else {
						m->preprocess(context, {line});
						statement = m->translate(context, {line});
					}
				}
			}
			break;
			case method_types::block: {
				if (level == 0)
					method = m;
				++level;
				context->instance->storage.add_domain();
				context->instance->storage.add_set();
				m->preprocess(context, {line});
				tmp.push_back(line);
			}
			break;
			case method_types::jit_command:
				m->translate(context, {line});
				break;
			}
			if (statement != nullptr)
				statement->repl_run();
		}
		catch (const lang_error &le) {
			reset_status();
			context->compiler->utilize_metadata();
			context->instance->storage.clear_set();
			throw fatal_error(std::string("Uncaught exception: ") + le.what());
		}
		catch (const cs::exception &e) {
			reset_status();
			context->compiler->utilize_metadata();
			context->instance->storage.clear_set();
			throw e;
		}
		catch (const std::exception &e) {
			reset_status();
			context->compiler->utilize_metadata();
			context->instance->storage.clear_set();
			throw exception(line_num, context->file_path, code, e.what());
		}
		context->compiler->utilize_metadata();
		context->instance->storage.clear_set();
	}

	void repl::exec(const string &code) {
		// Preprocess
		++line_num;
		int mode = 0;
		for (auto &ch:code) {
			if (mode == 0) {
				if (!std::isspace(ch)) {
					switch (ch) {
					case '#':
						context->file_buff.emplace_back();
						return;
					case '@':
						mode = 1;
						break;
					default:
						mode = -1;
					}
				}
			}
			else if (mode == 1) {
				if (!std::isspace(ch))
					cmd_buff.push_back(ch);
			}
			else
				break;
		}
		switch (mode) {
		default:
			break;
		case 0:
			return;
		case 1:
			if (cmd_buff == "begin" && !multi_line) {
				multi_line = true;
				context->file_buff.emplace_back();
			}
			else if (cmd_buff == "end" && multi_line) {
				multi_line = false;
				this->run(line_buff);
				line_buff.clear();
			}
			else
				throw exception(line_num, context->file_path, cmd_buff, "Wrong grammar for preprocessor command.");
			cmd_buff.clear();
			return;
		}
		if (multi_line) {
			context->file_buff.emplace_back();
			line_buff.append(code);
		}
		else {
			context->file_buff.push_back(code);
			this->run(code);
		}
	}
}
