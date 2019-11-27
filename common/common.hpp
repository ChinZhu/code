#include<string>
#include<vector>
#include<queue>
#include<stack>
#include<set>
#include<map>
#include <unordered_set>
#include <unordered_map>
#include<iterator>

#include<iostream>
#include<fstream>
#include<sstream>

#include<time.h>

namespace std {
	template <typename T>
	ostream& operator<<(ostream& os, vector<T> &ctn) {
		for (auto elm : ctn)
			cout << elm << " ";
		return os;
	}


	template <typename T>
	ostream& operator<<(ostream& os, set<T> &ctn) {
		for (auto elm : ctn)
			cout << elm << " ";
		return os;
	}

	template <typename T>
	ostream& operator<<(ostream& os, unordered_set<T> &ctn) {
		for (auto elm : ctn)
			cout << elm << " ";
		return os;
	}


	template <typename T>
	ostream& operator<<(ostream& os, multiset<T> &ctn) {
		for (auto elm : ctn)
			cout << elm << " ";
		return os;
	}


	template <typename T1, typename T2>
	ostream& operator<<(ostream& os, map<T1, T2> &ctn) {
		for (auto elm : ctn)
			cout << "(" << elm.first << "," << elm.second << ") ";
		return os;
	}


	template <typename T1, typename T2>
	ostream& operator<<(ostream& os, unordered_map<T1, T2> &ctn) {
		for (auto elm : ctn)
			cout << "(" << elm.first << "," << elm.second << ") ";
		return os;
	}
}

namespace common {
	template <typename T>
	class disjointSetForest {
	public:
		struct properties {
			T parent;
			int rank;
			properties() = default;
			properties(T _parent, int _rank) :parent(_parent), rank(_rank) {};
		};
		
		std::map<int, properties> nodes;
		
		void link_set(T x, T y) {
			if (nodes[x].rank > nodes[y].rank) // union by rank
				nodes[y].parent = x;
			else {
				nodes[x].parent = y;
				if (nodes[x].rank == nodes[y].rank) {
					++nodes[y].rank;
				}
			}
		}
		void make_set(T x) {
			nodes.insert(make_pair(x, properties(x, 0)));
		}
		void union_set(T x, T y) {
			link_set(find_set(x), find_set(y));
		}
		T find_set(T x) {
			if (x != nodes[x].parent) // path compression
				nodes[x].parent = find_set(nodes[x].parent);
			return nodes[x].parent;
		}

	};

}