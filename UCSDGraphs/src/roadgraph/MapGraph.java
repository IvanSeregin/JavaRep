/**
 * @author UCSD MOOC development team and YOU
 * 
 * A class which reprsents a graph of geographic locations
 * Nodes in the graph are intersections between 
 *
 */
package roadgraph;


import java.util.AbstractQueue;
import java.util.ArrayList;
import java.util.Collections;
import java.util.HashMap;
import java.util.HashSet;
import java.util.LinkedList;
import java.util.List;
import java.util.Map;
import java.util.PriorityQueue;
import java.util.Queue;
import java.util.Set;
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
	//TODO: Add your member variables here in WEEK 3
	
	private List<GeographicPoint> vertices; //Holds vertices
	private HashMap<GeographicPoint, List<GeographicPoint>> edges; //Holds edges (an adjacency list)   
	private Map<String, Edge> edgeInfo; //Holds info about edges (street name, street type, etc.)
	
	/** 
	 * Create a new empty MapGraph 
	 */
	//The constructor inits members
	public MapGraph()
	{
		// TODO: Implement in this constructor in WEEK 3
		vertices = new ArrayList<GeographicPoint> ();
		edges = new HashMap<GeographicPoint, List<GeographicPoint>>();  
		edgeInfo = new HashMap<String, Edge> ();
	}
	
	/**
	 * Get the number of vertices (road intersections) in the graph
	 * @return The number of vertices in the graph.
	 */
	public int getNumVertices()
	{
		//TODO: Implement this method in WEEK 3
		return vertices.size();
	}
	
	/**
	 * Return the intersections, which are the vertices in this graph.
	 * @return The vertices in this graph as GeographicPoints
	 */
	public Set<GeographicPoint> getVertices()
	{
		//TODO: Implement this method in WEEK 3
		return new HashSet<GeographicPoint> (edges.keySet());
	}
	
	/**
	 * Get the number of road segments in the graph
	 * @return The number of edges in the graph.
	 */
	public int getNumEdges()
	{
		//TODO: Implement this method in WEEK 3
		int numEdges=0;
		for (int i=0; i<vertices.size(); i++)
		{
			numEdges += (edges.get(vertices.get(i))).size(); 
		}
		return numEdges;
	}

	/**
	 * This my method. It returns the list of neighbors of the specified vertex
	 * @param vertex We find neighbors of this vertex
	 * @return
	 */
	private List<GeographicPoint> getNeighbors(GeographicPoint vertex)
	{
		return new ArrayList<GeographicPoint> (edges.get(vertex));
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
		// TODO: Implement this method in WEEK 3
		//trying to add the specified location as a vertex.
		//if it's success, we put this vertex as a key value onto hashmap of edges
		if (!vertices.add(location)) return false;
		edges.put(location, new ArrayList<GeographicPoint> ());
		
		return true;
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

		//TODO: Implement this method in WEEK 3
		if (!vertices.contains(from) || !vertices.contains(to))
		{
			throw (new IllegalArgumentException());
		}
		
		if (from==null || to==null)
			throw (new IllegalArgumentException());
		
		if (length < 0)
			throw (new IllegalArgumentException());
		
		//add edge from "from" to "to"
		(edges.get(from)).add(to);
		
		//System.out.println(edges.get(from));
		//System.out.println(from + " --> " + to);
		
		//add info about the current edge in edgeInfo
		Edge e = new Edge(from, to, roadName, roadType, length);
		edgeInfo.put(roadName, e);
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
			 					     GeographicPoint goal, Consumer<GeographicPoint> nodeSearched)
	{
		// TODO: Implement this method in WEEK 3
		
		// Hook for visualization.  See writeup.
		
		System.out.println("BFS is in progress...");
		
		//I think there is no comment are needed because this algorithm is from the lectures
		//Everything is obvious here.
		
		if (!vertices.contains(start) || !vertices.contains(goal))
		{
			return null;
		}
		
		LinkedList<GeographicPoint> queue = new LinkedList <GeographicPoint>();
		GeographicPoint curr = new GeographicPoint(start.x, start.y);
		Set<GeographicPoint> visited = new HashSet<GeographicPoint>();
		Map<GeographicPoint, List<GeographicPoint>> parent = new HashMap<GeographicPoint, List<GeographicPoint>>();
		for(GeographicPoint pt : getVertices())
		{
			parent.put(pt, new ArrayList<GeographicPoint> ());
		}
		
		queue.add(start);
		visited.add(start);
		
		while (!queue.isEmpty())
		{
			curr = queue.remove();
			
			if(curr.equals(goal)) 
			{
				System.out.println("Goal is "+curr);
				System.out.println("Parent map is "+parent);
				List<GeographicPoint> path = new ArrayList<GeographicPoint>();
				
				while (!curr.equals(start))
				{
					path.add(curr);
					curr = parent.get(curr).get(0);
				}
				path.add(curr);
				Collections.reverse(path);
				return path;
			}
			
			for (GeographicPoint n: getNeighbors(curr))
			{
				if (!visited.contains(n)) 
				{
					visited.add(n);
					(parent.get(n)).add(curr);
					queue.add(n);
					nodeSearched.accept(n);
				}
			}			
		}
		System.out.println("BFS is done.");
		return null;
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
		// TODO: Implement this method in WEEK 4

		// Hook for visualization.  See writeup.
		//nodeSearched.accept(next.getLocation());
		
		return null;
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
		// TODO: Implement this method in WEEK 4
		
		// Hook for visualization.  See writeup.
		//nodeSearched.accept(next.getLocation());
		
		return null;
	}

	
	
	public static void main(String[] args)
	{
		System.out.print("Making a new map...");
		MapGraph firstMap = new MapGraph();
		System.out.print("DONE. \nLoading the map...");
		GraphLoader.loadRoadMap("data/testdata/simpletest.map", firstMap);
		System.out.println("DONE.");
		
		System.out.println("Number of vertices: " + firstMap.getNumVertices());
		System.out.println("Number of edges: " + firstMap.getNumEdges());
		System.out.println("List of vertices: " + firstMap.getVertices());
		
		GeographicPoint start = new GeographicPoint(4.0, -1.0);
		GeographicPoint end= new GeographicPoint(4.0, 1.0);
		
		List<GeographicPoint> list = new ArrayList<GeographicPoint>();
		list = firstMap.bfs(start, end);
		System.out.println("Path from {---" + start +"---} to {---" + end + "---} is "+list);
		
		// You can use this method for testing.  
		
		
		/* Here are some test cases you should try before you attempt 
		 * the Week 3 End of Week Quiz, EVEN IF you score 100% on the 
		 * programming assignment.
		 */
		/*
		MapGraph simpleTestMap = new MapGraph();
		GraphLoader.loadRoadMap("data/testdata/simpletest.map", simpleTestMap);
		
		GeographicPoint testStart = new GeographicPoint(1.0, 1.0);
		GeographicPoint testEnd = new GeographicPoint(8.0, -1.0);
		
		System.out.println("Test 1 using simpletest: Dijkstra should be 9 and AStar should be 5");
		List<GeographicPoint> testroute = simpleTestMap.dijkstra(testStart,testEnd);
		List<GeographicPoint> testroute2 = simpleTestMap.aStarSearch(testStart,testEnd);
		
		
		MapGraph testMap = new MapGraph();
		GraphLoader.loadRoadMap("data/maps/utc.map", testMap);
		
		// A very simple test using real data
		testStart = new GeographicPoint(32.869423, -117.220917);
		testEnd = new GeographicPoint(32.869255, -117.216927);
		System.out.println("Test 2 using utc: Dijkstra should be 13 and AStar should be 5");
		testroute = testMap.dijkstra(testStart,testEnd);
		testroute2 = testMap.aStarSearch(testStart,testEnd);
		
		
		// A slightly more complex test using real data
		testStart = new GeographicPoint(32.8674388, -117.2190213);
		testEnd = new GeographicPoint(32.8697828, -117.2244506);
		System.out.println("Test 3 using utc: Dijkstra should be 37 and AStar should be 10");
		testroute = testMap.dijkstra(testStart,testEnd);
		testroute2 = testMap.aStarSearch(testStart,testEnd);
		*/
		
		
		/* Use this code in Week 3 End of Week Quiz */
		/*MapGraph theMap = new MapGraph();
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
