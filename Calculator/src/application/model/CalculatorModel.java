package application.model;

import java.util.LinkedList;
import java.util.List;
import java.util.Queue;

public class CalculatorModel 
{
	Queue<Double> operands = new LinkedList();
	Queue<String> operations = new LinkedList();
	String result;
	
	public void addOperand (Double operand)
	{
		operands.add(operand);
	}
	
	public void addOperation(String o)
	{
		operations.add(o);
	}
	
	public String calculate()
	{
		Double r=operands.remove();
		while(!operands.isEmpty())
		{
			r = OperationPerformer.calc(r, operands.remove(), operations.remove());
		}
		return r.toString();
	}
	

	
}
