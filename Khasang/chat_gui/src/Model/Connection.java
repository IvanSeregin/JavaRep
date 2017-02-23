package Model;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.net.HttpURLConnection;
import java.net.MalformedURLException;
import java.net.URL;

/**
 * Created by ik34-admin on 20.02.2017.
 */
public class Connection {
    public Connection() throws IOException {
        String site = "https://yandex.ru/";
        URL url = new URL(site);
        HttpURLConnection httpURLConnection = (HttpURLConnection) url.openConnection();
        int response;
        response = httpURLConnection.getResponseCode();
        System.out.println(response);
        BufferedReader br = new BufferedReader(new InputStreamReader(httpURLConnection.getInputStream()));
        String str;
        while ((str = br.readLine()) != null) {
            System.out.println(str);
        }
    }
}
