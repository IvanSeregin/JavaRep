package application.view;

public enum OperButtonAction 
{
	plus 
	{
	    @Override
	    public String getOperation() 
	    {
	        return "+";
	    }; 
	},
	minus 
	{
	    @Override
	    public String getOperation() 
	    {
	        return "-";
	    }; 
	},
	div 
	{
	    @Override
	    public String getOperation()  
	    {
	        return "/";
	    }; 
	},
	mult 
	{
	    @Override
	    public String getOperation()  
	    {
	        return "*";
	    }; 
	},
	compute 
	{
	    @Override
	    public String getOperation()  
	    {
	        return "=";
	    }; 
	};
   public abstract String getOperation();
}


 