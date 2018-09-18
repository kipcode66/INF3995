package ca.polymtl.inf3995.team1.tp4;

import android.content.Intent;
import android.os.AsyncTask;
import android.os.Bundle;
import android.app.Activity;
import android.view.View;
import android.view.View.OnClickListener;
import android.webkit.WebResourceRequest;
import android.webkit.WebView;
import android.webkit.WebViewClient;
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

        Button button = (Button) findViewById(R.id.Back);
        button.setOnClickListener(new OnClickListener() {
            public void onClick(View v){
                finish();
            }
        });
        Intent intent = getIntent();
        requestUrl = intent.getExtras().getString("requestUrl");
        startRequest(requestUrl);
    }

    protected void startRequest(String url) {
        WebView output = findViewById(R.id.responseOut);
        output.loadUrl(url);
        output.setWebViewClient(new WebViewClient() {
            @Override
            public boolean shouldOverrideUrlLoading(WebView view, WebResourceRequest request) {
                view.loadUrl(request.getUrl().toString());
                return false;
            }
        });
    }

}
