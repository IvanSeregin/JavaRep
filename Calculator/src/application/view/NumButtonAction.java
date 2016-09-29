package application.view;

public enum NumButtonAction {
	 
    b1 {
                @Override
                public int doAction() {
                    return 1;
                }
            ;
    }, 
   b2 {
                @Override
                public int doAction() {
                	return 2;
                }
            ;
    },
    b3 {
                @Override
                public int doAction() {
                    return 3;
                }
            ;
    }, 
    b4 {
                @Override
                public int doAction() {
                    return 4;
                }
            ;
    }, 
    b5 
    {
	    @Override
	    public int doAction() 
	    {
	        return 5;
	    }; 
    },
    b6 
    {
	    @Override
	    public int doAction() 
	    {
	        return 6;
	    }; 
    },
    b7 
    {
	    @Override
	    public int doAction() 
	    {
	        return 8;
	    }; 
    },
    b8 
    {
	    @Override
	    public int doAction() 
	    {
	        return 8;
	    }; 
    },
    b9 
    {
	    @Override
	    public int doAction() 
	    {
	        return 9;
	    }; 
    },
    b0 
    {
	    @Override
	    public int doAction() 
	    {
	        return 0;
	    }; 
    }; 
   public abstract int doAction();
}