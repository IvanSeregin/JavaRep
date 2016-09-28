package model;

public class TextToPrint 
{
	String text;
	
	public TextToPrint()
	{
		text = new String("This is text");
	}
	
	public String getText()
	{
		return text;
	}
	
	public void setText(String str)
	{
		text = new String(str);
	}
}
