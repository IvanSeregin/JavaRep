package roadgraph;

import java.util.List;

import geography.GeographicPoint;

public class Node {
	private GeographicPoint pt;
	private int distance;
	
	public Node(GeographicPoint pt, int distance)
	{
		this.pt = pt;
		this.distance = distance;
	}
	
	public Node(GeographicPoint pt)
	{
		this.pt = pt;
		this.distance = 1000000;
	}	
	
	static public boolean contains(List<Node> nodes,  GeographicPoint loc)
	{
		for (Node n: nodes)
		{
			if (n.pt.equals(loc)) return true;
		}
		return false;
	}
	
	public GeographicPoint getLocation()
	{
		return this.pt;
	}
}
