#include <iostream>
#include <vector>
#include <map>
#include <string>
#include <cmath>
#include <set>
#include <algorithm>
using namespace std;

struct Node;

struct Edge
{
  Node *node_a, *node_b;
  bool revComplement_a = false, revComplement_b = false;
  int overlap = 0;

  Edge(Node *_node_a, bool _revComplement_a, Node *_node_b, bool _revComplement_b, int _overlap)
  {
    node_a = _node_a;
    revComplement_a = _revComplement_a;
    node_b = _node_b;
    revComplement_b = _revComplement_b;
    overlap = _overlap;
  }

  Edge(Node *_node_a, bool _revComplement_a, Node *_node_b, bool _revComplement_b)
  {
    node_a = _node_a;
    revComplement_a = _revComplement_a;
    node_b = _node_b;
    revComplement_b = _revComplement_b;
  }
};

struct Node
{
  vector<Edge *> edges;
  string name;
  string sequence;
  Node(string _name, string _sequence)
  {
    name = _name;
    sequence = _sequence;
  }
};

struct Graph
{
  vector<Node *> nodes;
};

map<string, Node *> nodeMap;

/*
  Split string by delimiter
*/
vector<string> split(string s, string delimiter)
{
  size_t pos_start = 0, pos_end, delim_len = delimiter.length();
  string token;
  vector<string> res;

  while ((pos_end = s.find(delimiter, pos_start)) != string::npos)
  {
    token = s.substr(pos_start, pos_end - pos_start);
    pos_start = pos_end + delim_len;
    res.push_back(token);
  }

  res.push_back(s.substr(pos_start));
  return res;
}

/*
  Decompose nodes into from old graph into one char sequences in the new graph
*/
void decompose(Graph *oldGraph, Graph *newGraph)
{
  for (auto n : oldGraph->nodes)
  {
    Node *oldNode;
    for (int i = 0; i < n->sequence.size(); i++)
    {
      string newName = n->name + "_" + to_string(i);
      string oneCharSequence = "";
      oneCharSequence += n->sequence[i];

      Node *node = new Node(newName, oneCharSequence);
      newGraph->nodes.push_back(node);
      nodeMap[newName] = node;

      if (i > 0)
      {
        oldNode->edges.push_back(new Edge(oldNode, false, node, false, 0));
      }
      oldNode = node;
    }
  }

  for (auto n : oldGraph->nodes)
  {
    for (auto e : n->edges)
    {
      Node *node1, *node2;
      node1 = nodeMap[e->node_a->name + "_" + to_string(e->node_a->sequence.size() - 1 - e->overlap)];
      node2 = nodeMap[e->node_b->name + "_0"];
      node1->edges.push_back(new Edge(node1, false, node2, false, 0));
    }
  }
}

int main()
{
  Graph *graph = new Graph();
  vector<vector<string>> edges;

  string line;
  while (getline(cin, line))
  {
    if (line == "")
      break;
    vector<string> splittedInput = split(line, "\t");

    if (splittedInput[0] == "S")
    {
      Node *node = new Node(
          splittedInput[1],
          splittedInput[2]);

      nodeMap[splittedInput[1]] = node;
      graph->nodes.push_back(node);
    }
    else if (splittedInput[0] == "L")
    {
      edges.push_back(splittedInput);
    }
  }
  for (auto splittedInput : edges)
  {
    Edge *edge = new Edge(
        nodeMap[splittedInput[1]],
        (splittedInput[2] == "-"),
        nodeMap[splittedInput[3]],
        (splittedInput[4] == "-"),
        stoi(splittedInput[5].substr(0, splittedInput[5].size() - 1)));

    nodeMap[splittedInput[1]]->edges.push_back(edge);
  }

  Graph *oneCharGraph = new Graph();
  decompose(graph, oneCharGraph);

  /*
    for (auto i : oneCharGraph->nodes)
    {
      for (auto j : i->edges)
      {
        cout << j->node_a->name << " " << j->node_a->sequence << " / ";
        cout << j->revComplement_a << endl;
        cout << j->node_b->name << " " << j->node_b->sequence << " / ";
        cout << j->revComplement_b << endl;
        cout << j->overlap << endl;
      }
    }
    */
}