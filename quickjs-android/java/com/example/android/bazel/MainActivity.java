package com.example.android.bazel;

import android.os.Bundle;
import android.util.Log;
import android.widget.TextView;
import java.io.BufferedInputStream;
import java.io.ByteArrayOutputStream;
import java.io.InputStream;
import lukekaalim.quickjs.*;



public class MainActivity extends AppCompatActivity {
  
  protected void onCreate(Bundle savedInstanceState) {
      super.onCreate(savedInstanceState);
      setContentView(R.layout.activity_main);

      try {
          JSRuntime runtime = QuickjsJava.Lib("app").NewRuntime();
          JSContext context = runtime.CreateContext();
          Log.v("com.example.android.bazel", String.join(",", this.getAssets().list("scripts")));
          InputStream stream = this.getAssets().open("scripts/index.js");

          BufferedInputStream bis = new BufferedInputStream(stream);
          ByteArrayOutputStream buf = new ByteArrayOutputStream();
          for (int result = bis.read(); result != -1; result = bis.read()) {
              buf.write((byte) result);
          }
          
          String script = buf.toString("UTF-8");
          JSValue value = context.Eval(script);
          //context.RunModule(value);

          TextView tv = (TextView) findViewById(R.id.sample_text);
          tv.setText(value.String());
      } catch (Exception error) {

      }
  }
}
