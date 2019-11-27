#include "common.hpp"

using namespace std;

struct graph_t {

	typedef int edgeDescriptor;
	typedef int vertexDescriptor;

	enum direct_t { undirected, directed };
	enum weight_t { unweighted, weighted };

	struct edge_t {
		vertexDescriptor ep1, ep2;
		double weight;
		int visited;
		edge_t(vertexDescriptor _ep1, vertexDescriptor _ep2, double _weight = 0, int _visited = 0)
			:ep1(_ep1), ep2(_ep2), weight(_weight), visited(_visited) {}
	};

	unordered_set<vertexDescriptor> vertices;
	unordered_map<vertexDescriptor, vector<edgeDescriptor>> adj_edges;
	vector<edge_t> edges;

	map<int, vector<int>> adj_vertices;
	map<int, vector<int>> adj_vertices_in;
	// data structure for csr format
	vector<int> vertices_csr;
	vector<int> edges_csr;

	// return the other endpoint giving by a vertex and the adjacent edge
	vertexDescriptor adj_vertex(vertexDescriptor v1, edgeDescriptor e) {
		return edges[e].ep1 == v1 ? edges[e].ep2 : edges[e].ep1;
	}
	vertexDescriptor adj_vertex(vertexDescriptor v1, edge_t *e) {
		return e->ep1 == v1 ? e->ep2 : e->ep1;
	}

	// set all the adjacent edges of vertex v "visited"
	void set_adj_edges_visited_by_vertex(vertexDescriptor v) {
		for (auto e : adj_edges[v]) {
			edges[e].visited = 1;
		}
	}
	// set all the adjacent edges of edge e "visited"
	void set_adj_edges_visited_by_edge(edgeDescriptor e) {
		vertexDescriptor ep1 = edges[e].ep1, ep2 = edges[e].ep2;
		set_adj_edges_visited_by_vertex(ep1);
		set_adj_edges_visited_by_vertex(ep2);
	}
	// reset the visited mark
	void reset_visited() {
		for (auto e : edges) {
			e.visited = 0;
		}
	}


	// create the graph giving by the file path
	void create_graph(string path, direct_t dt = undirected, weight_t wt = unweighted) {
		ifstream fin(path);
		if (!fin) {
			cout << "fail to open the file" << endl;
			return;
		}
		string line;
		size_t ep1, ep2;
		double weight;
		size_t edgeID = 0;
		while (getline(fin, line)) {
			istringstream sin(line);
			sin >> ep1 >> ep2;
			if (ep1 == ep2)
				continue;
			vertices.insert(ep1);
			vertices.insert(ep2);
			if (dt == undirected) {
				if (wt == unweighted) {
					edges.emplace_back(ep1, ep2);
					adj_edges[ep1].push_back(edgeID);
					adj_edges[ep2].push_back(edgeID);

					adj_vertices[ep1].push_back(ep2);
					adj_vertices[ep2].push_back(ep1);
					
					adj_vertices_in[ep2].push_back(ep1);
					adj_vertices_in[ep1].push_back(ep2);
				}
				else { // weighted
					sin >> weight;
					edges.emplace_back(ep1, ep2, weight);
					adj_edges[ep1].push_back(edgeID);
					adj_edges[ep2].push_back(edgeID);

					adj_vertices[ep1].push_back(ep2);
					adj_vertices[ep2].push_back(ep1);
					
					adj_vertices_in[ep2].push_back(ep1);
					adj_vertices_in[ep1].push_back(ep2);
				}

			}
			else { //directed
				if (wt == unweighted) {
					edges.emplace_back(ep1, ep2);
					adj_edges[ep1].push_back(edgeID);

					adj_vertices[ep1].push_back(ep2);
					
					adj_vertices_in[ep2].push_back(ep1);
				}
				else { // weighted
					sin >> weight;
					edges.emplace_back(ep1, ep2, weight);
					adj_edges[ep1].push_back(edgeID);

					adj_vertices[ep1].push_back(ep2);
					
					adj_vertices_in[ep2].push_back(ep1);
				}
			}
			++edgeID;
		}

		// create the CSR format (vertices_csr, edges_csr)
		unordered_map<int, int> mapper;
		int IDX = 0;
		for (auto pr : adj_vertices) {
			mapper[pr.first] = IDX++;
		}

		int vIdx = 0;
		int eIdx = 0;
		for (auto pr : adj_vertices) {
			vertices_csr.push_back(eIdx);
			for (auto v : pr.second) {
				edges_csr.push_back(mapper[v]);
			}
			++vIdx;
			eIdx += pr.second.size();
		}
		vertices_csr.push_back(eIdx);

		cout << "graph created" << endl;
	}

};