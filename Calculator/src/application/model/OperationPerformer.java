package application.model;

import java.util.function.Predicate;

public class OperationPerformer 
{
	public static Double calc(Double o1, Double o2, String operation)
	{
		Double r = 0.0;
		if (operation.equals("+"))
		{
			r = o1+o2;
		}
		else
		if (operation.equals("-"))
		{
			r = o1-o2;
		}
		else
		if (operation.equals("*"))
		{
			r = o1*o2;
		}
		else
		if (operation.equals("/"))
		{
			r = o1/o2;
		}
		return r;
	}
}
