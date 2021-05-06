#pragma once

struct TypeChecker {
	enum class Type {
		Unknown,
		Int,
		Num,
	};

	std::vector<Type> ast_types;
	std::unordered_map<String::Id, Type> var_types;

	void set(String::Id id, Type value) {
		var_types[id] = value;
	}

	Type get(String::Id id) {
		auto it = var_types.find(id);
		assert(it != var_types.end());
		return it->second;
	}

	Type& type_of(Ast::Id i) {
		return ast_types[i.idx];
	}

	bool check(Frontend& frontend) {
		const size_t n = frontend.asts.size();

		ast_types.resize(n, Type::Unknown);

		for (size_t i {0}; i != n; ++i) {
			auto& ast = frontend.get(Ast::Id {i});
			switch (ast.tag) {
			case Ast::Tag::Decl:
				set(ast.as_decl.name, type_of(ast.as_decl.value));
				break;
			case Ast::Tag::Sum: {
				auto rhs = ast.as_sum.rhs;
				auto lhs = ast.as_sum.lhs;
				if (type_of(lhs) != type_of(rhs))
					return false;
				ast_types[i] = type_of(lhs);
				} break;
			case Ast::Tag::Int:
				ast_types[i] = Type::Int;
				break;
			case Ast::Tag::Num:
				ast_types[i] = Type::Num;
				break;
			case Ast::Tag::Var:
				ast_types[i] = get(ast.as_var.name);
				break;
			}
		}

		return true;
	}
};
 
