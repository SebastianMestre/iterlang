#pragma once

#include "ast.hpp"
#include "strings.hpp"

#include <cassert>

#include <cstdio>

struct Frontend {
	String::Holder strings;
	std::vector<Ast::Ast> asts;

	Ast::Ast& get(Ast::Id id) {
		return asts[id.idx];
	}
};

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
		std::printf("pushing %d\n", value);
		stack.push_back(value);
	}

	int pop() {
		int result = stack.back();
		std::printf("popping %d\n", result);
		stack.pop_back();
		return result;
	}
};

void interpret(Interpreter& interpreter, Frontend& frontend) {
	// we expect the ast nodes in the frontend to be in post-order

	for (int i = 0; i < frontend.asts.size(); ++i) {
		auto& ast = frontend.get(Ast::Id {i});
		std::puts("");

		switch (ast.tag) {
		case Ast::Tag::Decl:
			interpreter.set(ast.as_decl.name, interpreter.pop());
			break;
		case Ast::Tag::Sum: {
			int lhs = interpreter.pop();
			int rhs = interpreter.pop();
			interpreter.push(lhs + rhs);
			} break;
		case Ast::Tag::Lit:
			interpreter.push(ast.as_lit.value);
			break;
		case Ast::Tag::Var:
			interpreter.push(interpreter.get(ast.as_var.name));
			break;
		}
	}
}
