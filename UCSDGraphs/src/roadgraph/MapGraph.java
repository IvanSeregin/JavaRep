/**
 * @author UCSD MOOC development team and YOU
 * 
 * A class which reprsents a graph of geographic locations
 * Nodes in the graph are intersections between 
 *
 */
package roadgraph;


import java.util.ArrayList;
import java.util.HashMap;
import java.util.HashSet;
import java.util.Iterator;
import java.util.LinkedList;
import java.util.List;
import java.util.PriorityQueue;
import java.util.Queue;
import java.util.Set;
import java.util.Stack;
import java.util.function.Consumer;

import geography.GeographicPoint;
import util.GraphLoader;

/**
 * @author UCSD MOOC development team and YOU
 * 
 * A class which represents a graph of geographic locations
 * Nodes in the graph are intersections between 
 *
 */
public class MapGraph {
	// Maintain both nodes and edges as you will need to
	// be able to look up nodes by lat/lon or by roads
	// that contain those nodes.
	private HashMap<GeographicPoint,MapNode> pointNodeMap;
	private HashSet<MapEdge> edges;

	
	/** 
	 * Create a new empty MapGraph 
	 */
	public MapGraph()
	{
		pointNodeMap = new HashMap<GeographicPoint,MapNode>();
		edges = new HashSet<MapEdge>();
	}
	
	/**
	 * Get the number of vertices (road intersections) in the graph
	 * @return The number of vertices in the graph.
	 */
	public int getNumVertices()
	{
		return pointNodeMap.values().size();
	}
	
	/**
	 * Return the intersections, which are the vertices in this graph.
	 * @return The vertices in this graph as GeographicPoints
	 */
	public Set<GeographicPoint> getVertices()
	{
		return pointNodeMap.keySet();
	}
	
	/**
	 * Get the number of road segments in the graph
	 * @return The number of edges in the graph.
	 */
	public int getNumEdges()
	{
		return edges.size();
	}

	
	
	/** Add a node corresponding to an intersection at a Geographic Point
	 * If the location is already in the graph or null, this method does 
	 * not change the graph.
	 * @param location  The location of the intersection
	 * @return true if a node was added, false if it was not (the node
	 * was already in the graph, or the parameter is null).
	 */
	public boolean addVertex(GeographicPoint location)
	{
		if (location == null) {
			return false;
		}
		MapNode n = pointNodeMap.get(location);
		if (n == null) {
			n = new MapNode(location);
			pointNodeMap.put(location, n);
			return true;
		}
		else {
			return false;
		}
	}
	
	/**
	 * Adds a directed edge to the graph from pt1 to pt2.  
	 * Precondition: Both GeographicPoints have already been added to the graph
	 * @param from The starting point of the edge
	 * @param to The ending point of the edge
	 * @param roadName The name of the road
	 * @param roadType The type of the road
	 * @param length The length of the road, in km
	 * @throws IllegalArgumentException If the points have not already been
	 *   added as nodes to the graph, if any of the arguments is null,
	 *   or if the length is less than 0.
	 */
	public void addEdge(GeographicPoint from, GeographicPoint to, String roadName,
			String roadType, double length) throws IllegalArgumentException {

		MapNode n1 = pointNodeMap.get(from);
		MapNode n2 = pointNodeMap.get(to);

		// check nodes are valid
		if (n1 == null)
			throw new NullPointerException("addEdge: pt1:"+from+"is not in graph");
		if (n2 == null)
			throw new NullPointerException("addEdge: pt2:"+to+"is not in graph");

		MapEdge edge = new MapEdge(roadName, roadType, n1, n2, length);
		edges.add(edge);
		n1.addEdge(edge);
		
	}
		
	/** 
	 * Get a set of neighbor nodes from a mapNode
	 * @param node  The node to get the neighbors from
	 * @return A set containing the MapNode objects that are the neighbors 
	 * 	of node
	 */
	private Set<MapNode> getNeighbors(MapNode node) {
		return node.getNeighbors();
	}
	
	/** Find the path from start to goal using breadth first search
	 * 
	 * @param start The starting location
	 * @param goal The goal location
	 * @return The list of intersections that form the shortest (unweighted)
	 *   path from start to goal (including both start and goal).
	 */
	public List<GeographicPoint> bfs(GeographicPoint start, GeographicPoint goal) {
		// Dummy variable for calling the search algorithms
        Consumer<GeographicPoint> temp = (x) -> {};
        return bfs(start, goal, temp);
	}
	
	/** Find the path from start to goal using breadth first search
	 * 
	 * @param start The starting location
	 * @param goal The goal location
	 * @param nodeSearched A hook for visualization.  See assignment instructions for how to use it.
	 * @return The list of intersections that form the shortest (unweighted)
	 *   path from start to goal (including both start and goal).
	 */
	public List<GeographicPoint> bfs(GeographicPoint start, 
			 					     GeographicPoint goal, 
			 					     Consumer<GeographicPoint> nodeSearched)
	{
		/* Note that this method is a little long and we might think
		 * about refactoring it to break it into shorter methods as we 
		 * did in the Maze search code in week 2 */
		
		// Setup - check validity of inputs
		if (start == null || goal == null)
			throw new NullPointerException("Cannot find route from or to null node");
		MapNode startNode = pointNodeMap.get(start);
		MapNode endNode = pointNodeMap.get(goal);
		if (startNode == null) {
			System.err.println("Start node " + start + " does not exist");
			return null;
		}
		if (endNode == null) {
			System.err.println("End node " + goal + " does not exist");
			return null;
		}

		// setup to begin BFS
		HashMap<MapNode,MapNode> parentMap = new HashMap<MapNode,MapNode>();
		Queue<MapNode> toExplore = new LinkedList<MapNode>();
		HashSet<MapNode> visited = new HashSet<MapNode>();
		toExplore.add(startNode);
		MapNode next = null;

		while (!toExplore.isEmpty()) {
			next = toExplore.remove();
			
			 // hook for visualization
			nodeSearched.accept(next.getLocation());
			
			if (next.equals(endNode)) break;
			Set<MapNode> neighbors = getNeighbors(next);
			for (MapNode neighbor : neighbors) {
				if (!visited.contains(neighbor)) {
					visited.add(neighbor);
					parentMap.put(neighbor, next);
					toExplore.add(neighbor);
				}
			}
		}
		if (!next.equals(endNode)) {
			System.out.println("No path found from " +start+ " to " + goal);
			return null;
		}
		// Reconstruct the parent path
		List<GeographicPoint> path =
				reconstructPath(parentMap, startNode, endNode);

		return path;
	
	}
	


	/** Reconstruct a path from start to goal using the parentMap
	 *
	 * @param parentMap the HashNode map of children and their parents
	 * @param start The starting location
	 * @param goal The goal location
	 * @return The list of intersections that form the shortest path from
	 *   start to goal (including both start and goal).
	 */
	private List<GeographicPoint>
	reconstructPath(HashMap<MapNode,MapNode> parentMap,
					MapNode start, MapNode goal)
	{
		LinkedList<GeographicPoint> path = new LinkedList<GeographicPoint>();
		MapNode current = goal;

		while (!current.equals(start)) {
			path.addFirst(current.getLocation());
			current = parentMap.get(current);
		}

		// add start
		path.addFirst(start.getLocation());
		return path;
	}


	/** Find the path from start to goal using Dijkstra's algorithm
	 * 
	 * @param start The starting location
	 * @param goal The goal location
	 * @return The list of intersections that form the shortest path from 
	 *   start to goal (including both start and goal).
	 */
	public List<GeographicPoint> dijkstra(GeographicPoint start, GeographicPoint goal) {
		// Dummy variable for calling the search algorithms
		// You do not need to change this method.
        Consumer<GeographicPoint> temp = (x) -> {};
        return dijkstra(start, goal, temp);
	}
	
	/** Find the path from start to goal using Dijkstra's algorithm
	 * 
	 * @param start The starting location
	 * @param goal The goal location
	 * @param nodeSearched A hook for visualization.  See assignment instructions for how to use it.
	 * @return The list of intersections that form the shortest path from 
	 *   start to goal (including both start and goal).
	 */
	public List<GeographicPoint> dijkstra(GeographicPoint start, 
										  GeographicPoint goal, Consumer<GeographicPoint> nodeSearched)
	{
		// TODO: Implement this method in WEEK 3

		// Hook for visualization.  See writeup.
		//nodeSearched.accept(next.getLocation());
		
		
		
		if (start == null || goal == null)
			throw new NullPointerException("Cannot find route from or to null node");
		
		HashMap<GeographicPoint,MapNode> nodes = new HashMap<GeographicPoint,MapNode>();
		nodes.putAll(pointNodeMap);
		MapNode startNode = nodes.get(start);
		MapNode endNode = nodes.get(goal);
		
		if (startNode == null) {
			System.err.println("Start node " + start + " does not exist");
			return null;
		}
		if (endNode == null) {
			System.err.println("End node " + goal + " does not exist");
			return null;
		}

		System.out.println("Dijkstra is in progress");
		
		// setup to begin BFS
		HashMap<MapNode,MapNode> parentMap = new HashMap<MapNode,MapNode>();
		PriorityQueue<MapNode> toExplore = new PriorityQueue<MapNode>();
		HashSet<MapNode> visited = new HashSet<MapNode>();
		startNode.setDistanceToNode(0);
		toExplore.add(startNode);
		MapNode curr = null;
		double d2=0.0;
		int i=0;

		while (!toExplore.isEmpty()) 
		{
			curr = toExplore.remove();
			System.out.println(curr);
			i++;
			if (!visited.contains(curr)) 
			{
				visited.add(curr);
				if (curr.equals(endNode)) break;
				// hook for visualization
				//nodeSearched.accept(next.getLocation());
				
				Set<MapNode> neighbors = getNeighbors(curr);
				
				
				for (MapNode n : neighbors) 
				{
					if (!visited.contains(n)) 
					{
						d2 = curr.getLocation().distance(n.getLocation());
						//if (d2<d1)
						{
							if (pointNodeMap.get(n.getLocation()).getDistanceToNode() > curr.getDistanceToNode() + d2)
							{
								n.setDistanceToNode(curr.getLocation().distance(n.getLocation()) + curr.getDistanceToNode());
								toExplore.add(n);
								parentMap.put(n, curr);
							}
						}
					}
				}
			}
		}
		
		
		System.out.println ("Number of nodes: " + i);
		if (!curr.equals(endNode)) {
			System.out.println("No path found from " +start+ " to " + goal);
			return null;
		}
		// Reconstruct the parent path
		List<GeographicPoint> path =
				reconstructPath(parentMap, startNode, endNode);

		return path;
	}

	/** Find the path from start to goal using A-Star search
	 * 
	 * @param start The starting location
	 * @param goal The goal location
	 * @return The list of intersections that form the shortest path from 
	 *   start to goal (including both start and goal).
	 */
	public List<GeographicPoint> aStarSearch(GeographicPoint start, GeographicPoint goal) {
		// Dummy variable for calling the search algorithms
        Consumer<GeographicPoint> temp = (x) -> {};
        return aStarSearch(start, goal, temp);
	}
	
	/** Find the path from start to goal using A-Star search
	 * 
	 * @param start The starting location
	 * @param goal The goal location
	 * @param nodeSearched A hook for visualization.  See assignment instructions for how to use it.
	 * @return The list of intersections that form the shortest path from 
	 *   start to goal (including both start and goal).
	 */
	public List<GeographicPoint> aStarSearch(GeographicPoint start, 
											 GeographicPoint goal, Consumer<GeographicPoint> nodeSearched)
	{
		// TODO: Implement this method in WEEK 3
		
		// Hook for visualization.  See writeup.
		
		System.out.println("A* is in progress");
		if (start == null || goal == null)
			throw new NullPointerException("Cannot find route from or to null node");
		
		HashMap<GeographicPoint,MapNode> nodes = new HashMap<GeographicPoint,MapNode>();
		nodes.putAll(pointNodeMap);
		MapNode startNode = nodes.get(start);
		MapNode endNode = nodes.get(goal);
		
		if (startNode == null) {
			System.err.println("Start node " + start + " does not exist");
			return null;
		}
		if (endNode == null) {
			System.err.println("End node " + goal + " does not exist");
			return null;
		}
		
		// setup to begin BFS
		HashMap<MapNode,MapNode> parentMap = new HashMap<MapNode,MapNode>();
		PriorityQueue<MapNode> toExplore = new PriorityQueue<MapNode>();
		HashSet<MapNode> visited = new HashSet<MapNode>();
		startNode.setDistanceToNode(0);
		toExplore.add(startNode);
		MapNode curr = null;
		double d2=0.0;
		//nodeSearched.accept(curr.getLocation());
		int i=0;
		while (!toExplore.isEmpty()) 
		{
			curr = toExplore.remove();
			System.out.println(curr);
			i++;
			if (!visited.contains(curr)) 
			{
				visited.add(curr);
				if (curr.equals(endNode)) break;
				// hook for visualization
				nodeSearched.accept(curr.getLocation());
				
				Set<MapNode> neighbors = getNeighbors(curr);
				
				
				for (MapNode n : neighbors) 
				{
					if (!visited.contains(n)) 
					{
						{
							if ((pointNodeMap.get(n.getLocation()).getDistanceToNode() > curr.getDistanceToNode() + d2))
							{
								n.setDistanceToNode(n.getLocation().distance(goal));
								toExplore.add(n);
								parentMap.put(n, curr);
							}
						}
					}
				}
			}
		}
		
		System.out.println ("Number of nodes: " + i);
		if (!curr.equals(endNode)) {
			System.out.println("No path found from " +start+ " to " + goal);
			return null;
		}
		// Reconstruct the parent path
		List<GeographicPoint> path =
				reconstructPath(parentMap, startNode, endNode);

		return path;
	}
	
	private boolean dirak()
	{
		List<MapNode> nodes = new ArrayList<MapNode>(pointNodeMap.values());
		int v = getVertices().size(); 
	
		//1st check
		for (MapNode n : nodes)
		{
			if (this.getNeighbors(n).size() < v/2) 
				 return false; 
		}
		return true;
	}
	
	private boolean ore()
	{
		List<MapNode> nodes = new ArrayList<MapNode>(pointNodeMap.values());
		Set<MapNode> neighborsI;
		Set<MapNode> neighborsJ;
		int v = getVertices().size(); 
		for (MapNode i : nodes)
		{
			for (MapNode j : nodes)
			{
				//is J reachable form I???
				neighborsJ = this.getNeighbors(j);
				if (!neighborsJ.contains(i))
				{
					neighborsI = this.getNeighbors(i);
					if ((neighborsI.size() + neighborsJ.size()) < v)
					{
						 return false; 
					}
				}
			}
		}
		
		return true;
	}
	
	private boolean isHamiltonian()
	{
		/*
		Теорема 19. (Дирак). Граф гамильтонов, если степень любой
				его вершины удовлетворяет неравенству deg v >= n/2.
		Теорема 20. (Оре О.).Граф гамильтонов, если степени любых 
				двух его несмежных вершин v и u удовлетворяет неравенству
				deg v + deg u >= n.
		*/
		if (ore()) return true;
		if (dirak()) return true;
		
		
		return false;
	}
	
	private MapNode searchNearest(MapNode node, Set<MapNode> nodes)
	{
		//search nearest element for a node among other nodes
		double distance=0;
		MapNode nearest = null; 
		Iterator<MapNode> iter = nodes.iterator();
		if (!nodes.isEmpty())
		{
			MapNode n = iter.next();
			distance = node.getLocation().distance(n.getLocation());
			nearest = n;
		    while(iter.hasNext()) 
		    {
		        n = iter.next();
		        double newDist = node.getLocation().distance(n.getLocation());
		        if (newDist<distance)
		        {
		        	distance = newDist;
		        	nearest = n;
		        }
		    }
		}
		return nearest;
	}
	
	private Set<MapNode> minDegreeNodes(Set<MapNode> neighbors, Set<MapNode> visited)
	{
		//find the maximum degree of edges which are in neighbors
		Iterator<MapNode> iter = neighbors.iterator();
		Set<MapNode> notVisitedNeighbors = new HashSet<MapNode>();
		Set<MapNode> neighborsOfCurr = new HashSet<MapNode>();
		if (!neighbors.isEmpty())
		{
			MapNode n = iter.next();
			int edgesNum = 0;
			neighborsOfCurr = n.getNeighbors();
			neighborsOfCurr.removeAll(visited);
			edgesNum = neighborsOfCurr.size();
			
			while (iter.hasNext())
			{
				n = iter.next();
				neighborsOfCurr = n.getNeighbors();
				neighborsOfCurr.removeAll(visited);
				if (edgesNum>neighborsOfCurr.size())
				{
					edgesNum = neighborsOfCurr.size();
				}
			}
			//once maximum degree is know remove all vertices from 
			//neighbors with other degrees
			iter = neighbors.iterator();
			while (iter.hasNext())
			{
				n = iter.next();
				neighborsOfCurr = n.getNeighbors();
				neighborsOfCurr.removeAll(visited);
				if (neighborsOfCurr.size() == edgesNum)
				{
					notVisitedNeighbors.add(n);
				}
			}
		}
		return notVisitedNeighbors;
	}
	
	private List<GeographicPoint> greedy(GeographicPoint start)
	{
		HashMap<GeographicPoint,MapNode> nodes = new HashMap<GeographicPoint,MapNode>();
		nodes.putAll(pointNodeMap);
		MapNode curr;
		LinkedList<GeographicPoint> path = new LinkedList<GeographicPoint>();
		Queue<MapNode> toExplore = new LinkedList<MapNode>();
		HashSet<MapNode> visited = new HashSet<MapNode>();
		Set<MapNode> neighbors;
		
		toExplore.add(nodes.get(start));
		while (!toExplore.isEmpty()) 
		{
			curr = toExplore.remove();
			visited.add(curr);
			path.add(curr.getLocation());
			neighbors = curr.getNeighbors();
			
			neighbors.removeAll(visited);
			neighbors = minDegreeNodes(neighbors, visited);
			
			if (!neighbors.isEmpty())
				toExplore.add(searchNearest (curr, neighbors));
		}
		
		return path;
	}
	
	private List<GeographicPoint> nonGreedy(GeographicPoint start) 
	{
		/* Note that this method is a little long and we might think
		 * about refactoring it to break it into shorter methods as we 
		 * did in the Maze search code in week 2 */
		
		// Setup - check validity of inputs
		HashMap<GeographicPoint,MapNode> nodes = new HashMap<GeographicPoint,MapNode>();
		nodes.putAll(pointNodeMap);
		
		MapNode curr;
		LinkedList<GeographicPoint> path = new LinkedList<GeographicPoint>();
		Stack<MapNode> toExplore = new Stack<MapNode>();
		HashSet<MapNode> visited = new HashSet<MapNode>();
		Set<MapNode> neighbors;	
		
		toExplore.add(nodes.get(start));

		while (!toExplore.isEmpty()) 
		{
			curr = toExplore.pop();
			path.add
		}

		return path;
	}	
	
	/*
	 * This method traverses all nodes in a graph. It tries to
	 * traverse nodes just once if it's possible. It's possible if
	 * the graph is a hamiltonian graph.
	 * Otherwise, it tries to traverse all the nodes in the graph
	 * visiting them more than once.
	 * Firstly, we check whether the graph is a Hamiltonian one or not.
	 * If yes, greedy algorithm starts. This algorithm find nodes that have minimum +- degree
	 * and find the nearest node among these nodes.
	 */
	public List<GeographicPoint> salsemanPath(GeographicPoint start)
	{
		if (this.isHamiltonian())
		{
			System.out.println("The map is hamiltonian, I'm trying to find a greedy path");
			System.out.println (greedy(start));
		}
		else
		{
			System.out.println("Maybe the map is not hamiltonian");
			nonGreedy(start);
		}
		return null;
	}
	
	public static void main(String[] args)
	{
		System.out.print("Making a new map...");
		MapGraph firstMap = new MapGraph();
		System.out.print("DONE. \nLoading the map...");
		GraphLoader.loadRoadMap("data/testdata/hamilton.map", firstMap);
		System.out.println("DONE.");
		
		System.out.println("Number of vertices: " + firstMap.getNumVertices());
		GeographicPoint start = new GeographicPoint(4.0, 1.0);
		firstMap.salsemanPath(start);
		
		/*
		System.out.println("Number of vertices: " + firstMap.getNumVertices());
		System.out.println("Number of edges: " + firstMap.getNumEdges());
		System.out.println("List of vertices: " + firstMap.getVertices());
		
		GeographicPoint start = new GeographicPoint(1.0, 1.0);
		GeographicPoint end= new GeographicPoint(8.0, -1.0);
		
		
		List<GeographicPoint> list = new ArrayList<GeographicPoint>();
		list = firstMap.aStarSearch(start, end);
		System.out.println("Path from {---" + start +"---} to {---" + end + "---} is "+list);
		*/
		// You can use this method for testing.  
		
		/* Use this code in Week 3 End of Week Quiz
		MapGraph theMap = new MapGraph();
		System.out.print("DONE. \nLoading the map...");
		GraphLoader.loadRoadMap("data/maps/utc.map", theMap);
		System.out.println("DONE.");
		GeographicPoint start = new GeographicPoint(32.8648772, -117.2254046);
		GeographicPoint end = new GeographicPoint(32.8660691, -117.217393);
		
		
		List<GeographicPoint> route = theMap.dijkstra(start,end);
		List<GeographicPoint> route2 = theMap.aStarSearch(start,end);
		*/
		
	}
	
}