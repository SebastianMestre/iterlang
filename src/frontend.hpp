#pragma once

#include "ast.hpp"
#include "strings.hpp"

struct Frontend {
	String::Holder strings;
	std::vector<Ast::Ast> asts;

	Ast::Ast& get(Ast::Id id) {
		return asts[id.idx];
	}

	Ast::Id push(Ast::Ast const& ast) {
		size_t id {asts.size()};
		asts.push_back(ast);
		return {id};
	}
};
