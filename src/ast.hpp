#pragma once

#include "strings.hpp"

namespace Ast {

enum struct Tag {
	Decl,
	Sum,
	Int,
	Num,
	Var,
};

struct Id {
	std::size_t idx;

	bool is_valid() {
		return idx != static_cast<std::size_t>(-1);
	}
};

struct Decl {
	String::Id name;
	Id value;
};

struct Sum {
	Id lhs;
	Id rhs;
};

struct Int {
	int value;
};

struct Num {
	float value;
};

struct Var {
	String::Id name;
};

struct Ast {
	Tag tag;
	union {
		Decl as_decl;
		Sum as_sum;
		Int as_int;
		Num as_num;
		Var as_var;
	};
};

} // namespace Ast
