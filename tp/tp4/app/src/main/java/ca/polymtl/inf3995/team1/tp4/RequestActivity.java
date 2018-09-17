package ca.polymtl.inf3995.team1.tp4;

import android.content.Intent;
import android.os.AsyncTask;
import android.os.Bundle;
import android.app.Activity;
import android.view.View;
import android.view.View.OnClickListener;
import android.widget.Button;
import android.widget.TextView;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.net.HttpURLConnection;
import java.net.URL;
import java.util.concurrent.ExecutionException;

public class RequestActivity extends Activity {

    public String requestUrl;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_request);

        Button button = (Button) findViewById(R.id.retour);
        button.setOnClickListener(new OnClickListener() {
            public void onClick(View v){
                Intent i = new Intent(RequestActivity.this, MainActivity.class);
                startActivity(i);
            }
        });
        Intent intent = getIntent();
        requestUrl = intent.getExtras().getString("requestUrl");
        startRequest(requestUrl);
    }

    protected void startRequest(String url) {
        HttpGetRequest request = new HttpGetRequest();
        try {
            String response = request.execute(url).get();
            System.out.println("response: " + response);
            TextView text = findViewById(R.id.responseOut);
            text.setText(response);
        }
        catch (InterruptedException | ExecutionException e) {
            e.printStackTrace();
            // TODO
        }
    }

    /*
    Taken from https://medium.com/@JasonCromer/android-asynctask-http-request-tutorial-6b429d833e28
     */
    public static class HttpGetRequest extends AsyncTask<String, Void, String> {
        public static final String REQUEST_METHOD = "GET";
        public static final int READ_TIMEOUT = 15000;
        public static final int CONNECTION_TIMEOUT = 15000;
        @Override
        protected String doInBackground(String... params){
            String stringUrl = params[0];
            String result;
            String inputLine;
            try {
                //Create a URL object holding our url
                URL myUrl = new URL(stringUrl);
                //Create a connection
                HttpURLConnection connection =(HttpURLConnection) myUrl.openConnection();
                //Set methods and timeouts
                connection.setRequestMethod(REQUEST_METHOD);
                connection.setReadTimeout(READ_TIMEOUT);
                connection.setConnectTimeout(CONNECTION_TIMEOUT);

                //Connect to our url
                connection.connect();
                //Create a new InputStreamReader
                InputStreamReader streamReader = new
                        InputStreamReader(connection.getInputStream());
                //Create a new buffered reader and String Builder
                BufferedReader reader = new BufferedReader(streamReader);
                StringBuilder stringBuilder = new StringBuilder();
                //Check if the line we are reading is not null
                while((inputLine = reader.readLine()) != null){
                    stringBuilder.append(inputLine);
                }
                //Close our InputStream and Buffered reader
                reader.close();
                streamReader.close();
                //Set our result equal to our stringBuilder
                result = stringBuilder.toString();
            }
            catch(IOException e){
                e.printStackTrace();
                result = null;
            }
            return result;
        }
        protected void onPostExecute(String result){
            super.onPostExecute(result);
        }
    }

}
