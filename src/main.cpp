#include "frontend.hpp"
#include "interpreter.hpp"

#include <iostream>

int main () {
	Frontend frontend;
	Interpreter interpreter;

	Ast::Ast ast;

	ast.tag = Ast::Tag::Lit;
	ast.as_lit = {1};
	frontend.asts.push_back(ast);

	auto x = frontend.strings.get_id("x");

	ast.tag = Ast::Tag::Decl;
	ast.as_decl = {x, {0}};
	frontend.asts.push_back(ast);

	ast.tag = Ast::Tag::Lit;
	ast.as_lit = {2};
	frontend.asts.push_back(ast);

	auto y = frontend.strings.get_id("y");

	ast.tag = Ast::Tag::Decl;
	ast.as_decl = {y, {2}};
	frontend.asts.push_back(ast);

	ast.tag = Ast::Tag::Var;
	ast.as_var = {x};
	frontend.asts.push_back(ast);

	ast.tag = Ast::Tag::Var;
	ast.as_var = {y};
	frontend.asts.push_back(ast);

	ast.tag = Ast::Tag::Sum;
	ast.as_sum = {{4}, {5}};
	frontend.asts.push_back(ast);

	auto z = frontend.strings.get_id("z");

	ast.tag = Ast::Tag::Decl;
	ast.as_decl = {z, {6}};
	frontend.asts.push_back(ast);

	ast.tag = Ast::Tag::Lit;
	ast.as_lit = {1};
	frontend.asts.push_back(ast);

	ast.tag = Ast::Tag::Lit;
	ast.as_lit = {2};
	frontend.asts.push_back(ast);

	ast.tag = Ast::Tag::Sum;
	ast.as_sum = {{7}, {8}};
	frontend.asts.push_back(ast);

	ast.tag = Ast::Tag::Var;
	ast.as_var = {z};
	frontend.asts.push_back(ast);

	ast.tag = Ast::Tag::Sum;
	ast.as_sum = {{9}, {10}};
	frontend.asts.push_back(ast);

	auto a = frontend.strings.get_id("a");

	ast.tag = Ast::Tag::Decl;
	ast.as_decl = {a, {11}};
	frontend.asts.push_back(ast);

	interpreter.interpret(frontend);

	std::cout << interpreter.get(a) << '\n';
}
