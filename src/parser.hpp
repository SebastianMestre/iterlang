#include "ast.hpp"

#include <cctype>

#include <cstdio>

struct Parser {
	Frontend& frontend;
	std::string source;
	int source_idx {0};

	Parser(Frontend& frontend, std::string source)
		: frontend {frontend}
		, source {std::move(source)}
		{}

	char peek(int d = 0) {
		return source[source_idx + d];
	}

	void advance(int d = 1) {
		source_idx += d;
	}

	bool match(char c) {
		return peek() == c;
	}

	std::string parse_name() {
		if (!isalpha(peek()))
			return "";

		int start_idx = source_idx;
		while (isalpha(peek()))
			advance();
		int end_idx = source_idx;

		std::string str;
		str.reserve(end_idx - start_idx);
		for (int i = end_idx - start_idx; i-- > 0;)
			str.push_back(peek(-i-1));

		return str;
	}

	Ast::Id parse_lit() {
		if (!isdigit(peek()) || peek() == '0')
			return {static_cast<std::size_t>(-1)};

		int value = 0;
		while (isdigit(peek())) {
			value *= 10;
			value += peek() - '0';
			advance();
		}

		Ast::Ast ast;
		ast.tag = Ast::Tag::Lit;
		ast.as_lit = {value};
		return frontend.push(ast);
	}

	Ast::Id parse_var() {
		auto name = parse_name();

		if (name.empty())
			return {static_cast<std::size_t>(-1)};

		auto str_id = frontend.strings.get_id(name);

		Ast::Ast ast;
		ast.tag = Ast::Tag::Var;
		ast.as_var.name = str_id;

		return frontend.push(ast); 
	}

	Ast::Id parse_terminal() {
		if (isdigit(peek()) && !match('0'))
			return parse_lit();

		if (isalpha(peek()))
			return parse_var();

		return {static_cast<std::size_t>(-1)};
	}

	Ast::Id parse_expression() {
		auto lhs = parse_terminal();

		if (!lhs.is_valid())
			return {static_cast<std::size_t>(-1)};

		while (1) {
			if (match('\n') || match(EOF))
				break;

			if (!match('+'))
				return {static_cast<std::size_t>(-1)};
			advance();

			auto rhs = parse_terminal();

			if (!rhs.is_valid())
				return {static_cast<std::size_t>(-1)};

			Ast::Ast ast;
			ast.tag = Ast::Tag::Sum;
			ast.as_sum = {lhs, rhs};

			lhs = frontend.push(ast);
		}

		return lhs;
	}

	Ast::Id parse_decl() {
		auto name = parse_name();

		if (name.empty())
			return {static_cast<std::size_t>(-1)};

		if (!match('='))
			return {static_cast<std::size_t>(-1)};
		advance();

		auto expr = parse_expression();

		if (!expr.is_valid())
			return {static_cast<std::size_t>(-1)};

		auto str_id = frontend.strings.get_id(name);

		Ast::Ast ast;
		ast.tag = Ast::Tag::Decl;
		ast.as_decl = {str_id, expr};
		return frontend.push(ast);
	}
};