#include <iostream>
#include <vector>
#include <map>
#include <string>
#include <cmath>
#include <set>
#include <algorithm>
#include <deque>
#include <fstream>
#include <chrono>

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
  vector<Edge *> outgoing, incoming;
  string name;
  string sequence;
  int dp1 = 0;
  int dp2 = 0;
  int order = 0;
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
        Edge *newEdge = new Edge(oldNode, false, node, false, 0);
        oldNode->outgoing.push_back(newEdge);
        node->incoming.push_back(newEdge);
      }
      oldNode = node;
    }
  }

  for (auto n : oldGraph->nodes)
  {
    for (auto e : n->outgoing)
    {
      Node *node1, *node2;
      node1 = nodeMap[e->node_a->name + "_" + to_string(e->node_a->sequence.size() - 1 - e->overlap)];
      node2 = nodeMap[e->node_b->name + "_0"];

      Edge *newEdge = new Edge(node1, false, node2, false, 0);
      node1->outgoing.push_back(newEdge);
      node2->incoming.push_back(newEdge);
    }
  }
}

/*
  Sorts the nodes topologically so the dp matrix could be initialized
*/
vector<Node *> topoSort(Graph *graph)
{
  map<Edge *, int> bio;
  vector<Node *> ret;
  deque<Node *> deque;

  for (auto n : graph->nodes)
  {
    if (n->incoming.size() == 0)
    {
      deque.push_back(n);
    }
  }

  while (!deque.empty())
  {
    Node *node = deque.front();
    deque.pop_front();

    ret.push_back(node);
    for (auto e : node->outgoing)
    {
      bio[e] = 1;
      bool noIncomingRemaining = true;
      for (auto inc : e->node_b->incoming)
      {
        if (bio[inc] == 0)
        {
          noIncomingRemaining = false;
        }
      }
      if (noIncomingRemaining)
        deque.push_back(e->node_b);
    }
  }

  return ret;
}

/*
  Returns the graph from .gfa file
  Graph is decomposed into nodes of sequence size 1
*/
Graph *getGraphFromFile(string &graphFile)
{
  Graph *graph = new Graph();
  vector<vector<string>> edges;

  ifstream file(graphFile);

  string line;
  while (getline(file, line))
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

    nodeMap[splittedInput[1]]->outgoing.push_back(edge);
    nodeMap[splittedInput[3]]->incoming.push_back(edge);
  }

  Graph *oneCharGraph = new Graph();
  decompose(graph, oneCharGraph);

  vector<Node *> sorted = topoSort(oneCharGraph);
  for (int i = 0; i < oneCharGraph->nodes.size(); i++)
  {
    oneCharGraph->nodes[i] = sorted[i];
  }

  return oneCharGraph;
}

/*
  Load patterns from fastq file
*/
vector<string> loadPatterns(const string &patternsFile)
{
  ifstream file(patternsFile);

  vector<string> patterns;
  string line;

  while (getline(file, line))
  {
    if (line[0] != '@')
    {
      continue;
    }

    if (getline(file, line))
    {
      patterns.push_back(line);
    }
  }

  return patterns;
}

/*
  Function g from the navarro paper
*/
int g(Node *node, char c, int i)
{
  if (node->sequence[0] == c)
  {
    int ret = i - 1;
    for (auto e : node->incoming)
    {
      ret = min(ret, e->node_a->dp1);
    }
    return ret;
  }
  else
  {
    int ret = node->dp1;
    for (auto e : node->incoming)
    {
      ret = min(ret, e->node_a->dp1);
    }
    return ret + 1;
  }
}

/*
  Function propagate from the navarro paper
*/
void propagate(Edge *edge)
{
  if (edge->node_b->dp1 > edge->node_a->dp1 + 1)
  {
    edge->node_b->dp1 = edge->node_a->dp1 + 1;
    for (auto e : edge->node_b->outgoing)
    {
      propagate(e);
    }
  }
}

/*
  Navarro algorithm implemented
*/
void navarro(Graph *graph, string pattern)
{
  for (int i = 0; i < pattern.size(); i++)
  {
    for (auto n : graph->nodes)
    {
      n->dp2 = g(n, pattern[i], i + 1);
    }
    for (auto n : graph->nodes)
    {
      n->dp1 = n->dp2;
    }
    for (auto n : graph->nodes)
    {
      for (auto e : n->outgoing)
      {
        propagate(e);
      }
    }
  }

  int minDp1 = graph->nodes[0]->dp1;
  for (auto n : graph->nodes)
  {
    minDp1 = min(minDp1, n->dp1);
  }
  cout << "score: " << minDp1 << endl;
}

int main(int argc, char *argv[])
{
  string graphFile;
  string patternsFile;

  for (int i = 1; i < argc; i++)
  {
    if (string(argv[i]) == "-g" && i + 1 < argc)
    {
      graphFile = argv[i + 1];
    }
    else if (string(argv[i]) == "-p" && i + 1 < argc)
    {
      patternsFile = argv[i + 1];
    }
  }

  if (graphFile.empty() || patternsFile.empty())
  {
    cout << "Usage: algo -g <graph_file> -p <patterns_file>" << endl;
    return 1;
  }

  Graph *graph = getGraphFromFile(graphFile);
  vector<string> patterns = loadPatterns(patternsFile);

  auto startTime = chrono::high_resolution_clock::now();

  for (auto i = 0; i < patterns.size(); i++)
  {
    cout << "Pattern " << i << ":" << endl;
    navarro(graph, patterns[i]);
    cout << endl;
  }

  auto duration = chrono::duration_cast<chrono::milliseconds>(chrono::high_resolution_clock::now() - startTime);
  cout << "Time: " << duration.count() << "ms" << endl;
}
