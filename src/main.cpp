#include "frontend.hpp"
#include "interpreter.hpp"
#include "parser.hpp"

#include <iostream>

void test1() {
	Frontend frontend;
	Interpreter interpreter;

	Ast::Ast ast;

	ast.tag = Ast::Tag::Int;
	ast.as_int = {1};
	frontend.asts.push_back(ast);

	auto x = frontend.strings.get_id("x");

	ast.tag = Ast::Tag::Decl;
	ast.as_decl = {x, {0}};
	frontend.asts.push_back(ast);

	ast.tag = Ast::Tag::Int;
	ast.as_int = {2};
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

	ast.tag = Ast::Tag::Int;
	ast.as_int = {1};
	frontend.asts.push_back(ast);

	ast.tag = Ast::Tag::Int;
	ast.as_int = {2};
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

	std::cout << "Expected: 6 -- Result: " << interpreter.get(a) << '\n';
}

void test2() {
	Frontend frontend;
	Parser parser {frontend, "variable=10+5"};
	parser.source.push_back(EOF);
	auto ast_id = parser.parse_decl();

	Interpreter interpreter;
	interpreter.interpret(frontend);

	auto var = frontend.strings.get_id("variable");

	std::cout << "Expected: 15 -- Result: " << interpreter.get(var) << '\n';
}

void test3() {
	Frontend frontend;

	std::string source =
R"(x=1
y=2
z=x+y
a=1+2+z)";
	source.push_back(EOF);

	Parser parser {frontend, std::move(source)};
	parser.parse_program();

	Interpreter interpreter;
	interpreter.interpret(frontend);

	auto a = frontend.strings.get_id("a");

	std::cout << "Expected: 6 -- Result: " << interpreter.get(a) << '\n';
}

void test4 () {
	Frontend frontend;

	std::string source = "a=3.0+3.5";
	source.push_back(EOF);

	Parser parser {frontend, std::move(source)};
	parser.parse_program();

	Interpreter interpreter;
	interpreter.interpret(frontend);

	auto a = frontend.strings.get_id("a");

	std::cout << "Expected: 6.5 -- Result: " << interpreter.get(a) << '\n';
}

int main () {
	test1();
	test2();
	test3();
	test4();
}
