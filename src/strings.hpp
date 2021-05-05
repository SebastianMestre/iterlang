#pragma once

#include <vector>
#include <string>
#include <unordered_map>

namespace String {

struct Id {
	std::size_t idx;

	bool operator== (Id const& o) const {
		return idx == o.idx;
	}
};


struct Holder {
	std::vector<std::string> strings;
	std::unordered_map<std::string, Id> mapping;

	Id get_id(std::string const& str) {
		auto insert_result = mapping.emplace(str, Id {});

		if (insert_result.second) {
			auto id = Id {strings.size()};
			strings.push_back(str);
			insert_result.first->second = id;
			return id;
		}

		return insert_result.first->second;
	}

	std::string const& get(Id id) {
		return strings[id.idx];
	}
};

} // namespace String

template<>
struct std::hash<String::Id> {
	std::size_t operator() (String::Id const& id) const {
		return std::hash<std::size_t>{}(id.idx);
	}
};
