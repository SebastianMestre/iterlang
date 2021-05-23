#include "ast.hpp"

#include <cctype>
#include <cassert>

struct Parser {
	Frontend& frontend;
	std::string source;
	int source_idx {0};

	Parser(Frontend& frontend, std::string source)
		: frontend {frontend}
		, source {std::move(source)}
		{}
	
	void eat_whitespace() {
		while (1) {
			char c = peek();
			if (c == ' ' || c == '\t' || c == '\n') {
				advance();
			} else {
				break;
			}
		}
	}

	void eat_spaces() {
		while (1) {
			char c = peek();
			if (c == ' ' || c == '\t') {
				advance();
			} else {
				break;
			}
		}
	}

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

	Ast::Id parse_numeric() {
		if (!isdigit(peek()) || peek() == '.')
			return {static_cast<std::size_t>(-1)};

		bool leading_zero = peek() == '0';
		bool dot = false;

		int start_idx = source_idx;
		while (1) {
			if (match('.')) {
				if (dot)
					return {static_cast<std::size_t>(-1)};
				dot = true;
				advance();
				continue;
			}

			if (!isdigit(peek()))
				break;

			advance();
		}
		int end_idx = source_idx;

		Ast::Ast ast;

		if (dot) {
			float value = std::stof(std::string(&source[start_idx], &source[end_idx]));
			ast.tag = Ast::Tag::Num;
			ast.as_num = {value};
		} else {
			if (leading_zero)
				return {static_cast<std::size_t>(-1)};
			int value = std::stoi(std::string(&source[start_idx], &source[end_idx]));
			ast.tag = Ast::Tag::Int;
			ast.as_int = {value};
		}

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
		if (isdigit(peek()))
			return parse_numeric();

		if (isalpha(peek()))
			return parse_var();

		return {static_cast<std::size_t>(-1)};
	}

	Ast::Id parse_expression() {
		auto lhs = parse_terminal();

		if (!lhs.is_valid())
			return {static_cast<std::size_t>(-1)};

		while (1) {
			eat_spaces();

			if (match('\n') || match(EOF))
				break;

			if (!match('+'))
				return {static_cast<std::size_t>(-1)};
			advance();

			eat_spaces();
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

		eat_whitespace();
		if (!match('='))
			return {static_cast<std::size_t>(-1)};
		advance();

		eat_whitespace();
		auto expr = parse_expression();

		if (!expr.is_valid())
			return {static_cast<std::size_t>(-1)};

		auto str_id = frontend.strings.get_id(name);

		Ast::Ast ast;
		ast.tag = Ast::Tag::Decl;
		ast.as_decl = {str_id, expr};
		return frontend.push(ast);
	}

	void parse_program() {
		while (1) {
			eat_whitespace();

			if (match(EOF)) break;

			auto decl = parse_decl();
			assert(decl.is_valid());

			eat_spaces();
			if (match('\n')) {
				advance();
				continue;
			}
		}
	}
};
