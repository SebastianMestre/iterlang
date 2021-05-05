#pragma once

#include "frontend.hpp"

#include <cassert>
#include <iostream>

struct Value {
	enum struct Tag {
		Int,
		Num,
	};
	Tag tag;
	union {
		int as_int;
		float as_num;
	};
};

Value make_value(int int_value) {
	Value result;
	result.tag = Value::Tag::Int;
	result.as_int = int_value;
	return result;
}

Value make_value(float num_value) {
	Value result;
	result.tag = Value::Tag::Num;
	result.as_num = num_value;
	return result;
}

Value operator+ (Value lhs, Value rhs) {
	assert(lhs.tag == rhs.tag);
	switch (lhs.tag) {
	case Value::Tag::Int:
		return make_value(lhs.as_int + rhs.as_int);
	case Value::Tag::Num:
		return make_value(lhs.as_num + rhs.as_num);
	}
}

std::ostream& operator<< (std::ostream& o, Value value) {
	switch (value.tag) {
	case Value::Tag::Int:
		return o << value.as_int;
	case Value::Tag::Num:
		return o << value.as_num;
	}
}

struct Interpreter {
	std::unordered_map<String::Id, Value> vars;
	std::vector<Value> stack;

	Value get(String::Id id) {
		auto it = vars.find(id);
		assert(it != vars.end());
		return it->second;
	}


	void set(String::Id id, Value value) {
		vars[id] = value;
	}

	void set(String::Id id, int int_value) {
		set(id, make_value(int_value));
	}

	void set(String::Id id, float num_value) {
		set(id, make_value(num_value));
	}

	void push(Value value) {
		stack.push_back(value);
	}

	void push(int int_value) {
		push(make_value(int_value));
	}

	void push(float num_value) {
		push(make_value(num_value));
	}

	Value pop() {
		Value result = stack.back();
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
				Value rhs = pop();
				Value lhs = pop();
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
