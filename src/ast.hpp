#pragma once

#include "strings.hpp"

namespace Ast {

enum struct Tag {
	Decl,
	Sum,
	Lit,
	Var,
};

struct Id {
	int idx;
};

struct Decl {
	String::Id name;
	Id value;
};

struct Sum {
	Id lhs;
	Id rhs;
};

struct Lit {
	int value;
};

struct Var {
	String::Id name;
};

struct Ast {
	Tag tag;
	union {
		Decl as_decl;
		Sum as_sum;
		Lit as_lit;
		Var as_var;
	};
};

} // namespace Ast
