package view;

import javafx.fxml.FXML;
import main.HelloWorld;
import model.TextToPrint;

public class RootLayoutController 
{
	private HelloWorld mainApp;
	TextToPrint textToPrint = new TextToPrint();
	
	public void setMainApp(HelloWorld mainApp)
	{
		this.mainApp = mainApp;
	}
	
	@FXML
	private void handlePrintText()
	{
		System.out.print(textToPrint.getText());
	}
}
