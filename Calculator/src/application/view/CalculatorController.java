package application.view;

import application.model.CalculatorModel;
import javafx.beans.value.ObservableValue;
import javafx.event.ActionEvent;
import javafx.fxml.FXML;
import javafx.scene.control.*;
import javafx.scene.control.Alert.AlertType;

public class CalculatorController 
{
	CalculatorModel calcModel = new CalculatorModel();
	private boolean isCalculated = false;

	@FXML
	private TextField numInput;
	@FXML
	private Label numLabel;

	
	
	@FXML
	private void onNumButtonClick(ActionEvent event)
	{
		if (isCalculated)
		{
			numLabel.setText("");
			isCalculated = false;
		}
		NumButtonAction ba = NumButtonAction.valueOf(((Button) event.getSource()).idProperty().get());
		numInput.setText(numInput.getText() + ba.doAction());
	}
	
	@FXML
	private void onOperBtnClick(ActionEvent event)
	{
		try
		{
			calcModel.addOperand(Double.parseDouble(numInput.getText()));
			OperButtonAction ba = OperButtonAction.valueOf(((Button) event.getSource()).idProperty().get());
			numLabel.setText(numLabel.getText() + numInput.getText() + ba.getOperation());
			calcModel.addOperation(ba.getOperation());
			numInput.clear();
		}
		catch (Exception e)
		{
            Alert alert = new Alert(AlertType.ERROR);
            alert.setTitle("Error");
            alert.setHeaderText("Attention!");
            alert.setContentText("There is no number or a number format is incorrect");
            alert.showAndWait();
            numInput.clear();
		}
		
	}
	
	@FXML
	private void onCalculate(ActionEvent event)
	{
		calcModel.addOperand(Double.parseDouble(numInput.getText()));
		OperButtonAction ba = OperButtonAction.valueOf(((Button) event.getSource()).idProperty().get());
		numLabel.setText(numLabel.getText() + numInput.getText() + ba.getOperation());
		numLabel.setText(numLabel.getText() + calcModel.calculate());
		numInput.clear();
		//System.out.println(ba.getOperation());
		isCalculated = true;
	}
}
