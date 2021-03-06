package main;

import java.io.IOException;

import javafx.application.Application;
import javafx.fxml.FXMLLoader;
import javafx.scene.Scene;
import javafx.scene.layout.BorderPane;
import javafx.stage.Stage;
import view.*;

public class HelloWorld extends Application 
{
	
	private BorderPane rootLayout;
	private Stage primaryStage;
	RootLayoutController controller;
    
	@Override
    public void start(Stage primaryStage) 
    {
		this.primaryStage = primaryStage;
        this.primaryStage.setTitle("Hello World!!!");
        initRootLayout();
    }
	
    private void initRootLayout() 
    {
		try
		{
			FXMLLoader loader = new FXMLLoader();
			loader.setLocation(HelloWorld.class.getResource("../view/RootLayout.fxml"));
			rootLayout = (BorderPane) loader.load();
			
			Scene scene = new Scene(rootLayout);
			this.primaryStage.setScene(scene);
			
			/*
			controller = loader.getController();
			controller.setMainApp(this);
			*/
			
			primaryStage.show();
			
		} catch (IOException e) {
            e.printStackTrace();
        }
	}

	public static void main(String[] args) {
        launch(args);
    }
}
