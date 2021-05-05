#pragma once

#include "frontend.hpp"

#include <cassert>

struct Interpreter {
	std::unordered_map<String::Id, int> vars;
	std::vector<int> stack;

	int get(String::Id id) {
		auto it = vars.find(id);
		assert(it != vars.end());
		return it->second;
	}

	void set(String::Id id, int value) {
		vars[id] = value;
	}

	void push(int value) {
		stack.push_back(value);
	}

	int pop() {
		int result = stack.back();
		stack.pop_back();
		return result;
	}

	void interpret(Frontend& frontend) {
		// we expect the ast nodes in the frontend to be in post-order

		for (auto& ast : frontend.asts) {
			switch (ast.tag) {
			case Ast::Tag::Decl:
				set(ast.as_decl.name, pop());
				break;
			case Ast::Tag::Sum: {
				int lhs = pop();
				int rhs = pop();
				push(lhs + rhs);
				} break;
			case Ast::Tag::Int:
				push(ast.as_int.value);
				break;
			case Ast::Tag::Var:
				push(get(ast.as_var.name));
				break;
			}
		}
	}
};
