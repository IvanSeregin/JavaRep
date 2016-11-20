/**
 * @author Ivan Seregin
 * 
 * A class which holds information about each edge
 * There is no setters or getters yet
 */
package roadgraph;

import geography.GeographicPoint;

public class Edge implements Comparable <Edge>{
	private GeographicPoint from;
	private GeographicPoint to;
	private String roadName;
	private String roadType;
	private double length;
	
	
	//The following two methods are necessary if you want to use Edge with hash maps
	@Override
	public int hashCode() 
	{
		return roadName.hashCode();
	}
	
	@Override
    public boolean equals(Object obj)
    {
		if (!(obj instanceof Edge))
			return false;
		if (obj == this)
			return true;
		
		if (((Edge)obj).roadName.equals(this.roadName))
		{
			return true;
		}
		
		return false;
    }
	
	//Constructor
	Edge(GeographicPoint from, GeographicPoint to, String roadName,
			String roadType, double length)
	{
		this.from = from;
		this.to = to;
		this.roadName = roadName;
		this.roadType = roadType;
		this.length = length;
	}

	@Override
	public int compareTo(Edge arg0) {
		if (this.length<arg0.length) return -1;
		if (this.length>arg0.length) return 1;
		return 0;
	}
}
