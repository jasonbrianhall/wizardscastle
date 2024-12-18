#!/bin/bash
set -e

# Check for NDK
if [ -z "$ANDROID_NDK_HOME" ]; then
    echo "Error: ANDROID_NDK_HOME not set"
    exit 1
fi

# Create project structure
mkdir -p WizardsCastle/app/src/main/{assets,java/com/example/terminalwizcastle}
cd WizardsCastle

# Copy and compile the C program for each architecture
#cp ../../c/*.c app/src/main/assets/
#cp ../../c/*.h app/src/main/assets/
cp ../*.c app/src/main/assets/

for arch in arm64-v8a x86_64; do
    mkdir -p app/src/main/assets/$arch
    case $arch in
        "arm64-v8a") target="aarch64-linux-android21";;
        "x86_64") target="x86_64-linux-android21";;
    esac
    
    $ANDROID_NDK_HOME/toolchains/llvm/prebuilt/linux-x86_64/bin/clang \
        --target=$target \
        -static \
        -o app/src/main/assets/$arch/wizcastle \
        app/src/main/assets/*.c
done

# Create TerminalView.java
cat > app/src/main/java/com/example/terminalwizcastle/TerminalView.java << 'EOL'
package com.example.terminalwizcastle;

import android.content.Context;
import android.graphics.Canvas;
import android.graphics.Color;
import android.graphics.Paint;
import android.graphics.Typeface;
import android.util.AttributeSet;
import android.view.KeyEvent;
import android.view.View;
import android.view.inputmethod.BaseInputConnection;
import android.view.inputmethod.EditorInfo;
import android.view.inputmethod.InputConnection;
import java.io.IOException;
import java.io.OutputStream;

public class TerminalView extends View {
    private Paint textPaint;
    private float charWidth;
    private float charHeight;
    private int cols = 80;
    private int rows = 24;
    private char[][] buffer;
    private int cursorX = 0;
    private int cursorY = 0;
    private OutputStream outputStream;
    private boolean cursorVisible = true;
    
    public TerminalView(Context context) {
        super(context);
        init();
    }
    
    public TerminalView(Context context, AttributeSet attrs) {
        super(context, attrs);
        init();
    }
    
    private void init() {
        textPaint = new Paint();
        textPaint.setColor(Color.GREEN);
        textPaint.setTypeface(Typeface.MONOSPACE);
        textPaint.setTextSize(40);
        
        Paint.FontMetrics fm = textPaint.getFontMetrics();
        charHeight = fm.bottom - fm.top;
        charWidth = textPaint.measureText("M");
        
        buffer = new char[rows][cols];
        clearScreen();
        
        setFocusable(true);
        setFocusableInTouchMode(true);
    }
    
    public void setOutputStream(OutputStream os) {
        this.outputStream = os;
    }
    
    private void clearScreen() {
        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < cols; j++) {
                buffer[i][j] = ' ';
            }
        }
        invalidate();
    }
    
    public void write(byte[] data) {
        for (byte b : data) {
            if (b == '\n') {
                cursorY++;
                cursorX = 0;
            } else if (b == '\r') {
                cursorX = 0;
            } else if (b == '\b') {
                if (cursorX > 0) cursorX--;
            } else {
                if (cursorX >= cols) {
                    cursorX = 0;
                    cursorY++;
                }
                if (cursorY >= rows) {
                    scrollUp();
                    cursorY = rows - 1;
                }
                buffer[cursorY][cursorX++] = (char)b;
            }
        }
        invalidate();
    }
    
    private void scrollUp() {
        for (int i = 0; i < rows - 1; i++) {
            System.arraycopy(buffer[i + 1], 0, buffer[i], 0, cols);
        }
        for (int j = 0; j < cols; j++) {
            buffer[rows - 1][j] = ' ';
        }
    }
    
    @Override
    protected void onDraw(Canvas canvas) {
        super.onDraw(canvas);
        
        canvas.drawColor(Color.BLACK);
        
        float y = charHeight;
        for (int i = 0; i < rows; i++) {
            float x = 0;
            for (int j = 0; j < cols; j++) {
                canvas.drawText(String.valueOf(buffer[i][j]), x, y, textPaint);
                x += charWidth;
            }
            y += charHeight;
        }
        
        if (cursorVisible) {
            Paint cursorPaint = new Paint();
            cursorPaint.setColor(Color.GREEN);
            canvas.drawRect(
                cursorX * charWidth,
                cursorY * charHeight,
                (cursorX + 1) * charWidth,
                (cursorY + 1) * charHeight,
                cursorPaint
            );
        }
    }
   
    @Override
    public boolean onKeyDown(int keyCode, KeyEvent event) {
        if (outputStream != null) {
            try {
                switch (keyCode) {
                    case KeyEvent.KEYCODE_DEL:
                        // Send ASCII backspace (0x08) followed by space and another backspace
                        // This sequence ensures proper character erasure
                        outputStream.write(new byte[]{0x08, ' ', 0x08});
                        outputStream.flush();
                        return true;

                    case KeyEvent.KEYCODE_ENTER:
                        write("\n".getBytes());
                        outputStream.write('\n');
                        outputStream.flush();
                        return true;

                    case KeyEvent.KEYCODE_DPAD_UP:
                        // Send VT100 up arrow sequence
                        outputStream.write(new byte[]{0x1B, '[', 'A'});
                        outputStream.flush();
                        return true;

                    case KeyEvent.KEYCODE_DPAD_DOWN:
                        // Send VT100 down arrow sequence
                        outputStream.write(new byte[]{0x1B, '[', 'B'});
                        outputStream.flush();
                        return true;

                    case KeyEvent.KEYCODE_DPAD_LEFT:
                        // Send VT100 left arrow sequence
                        outputStream.write(new byte[]{0x1B, '[', 'D'});
                        outputStream.flush();
                        return true;

                    case KeyEvent.KEYCODE_DPAD_RIGHT:
                        // Send VT100 right arrow sequence
                        outputStream.write(new byte[]{0x1B, '[', 'C'});
                        outputStream.flush();
                        return true;

                    default:
                        int unicode = event.getUnicodeChar();
                        if (unicode != 0) {
                            write(String.valueOf((char)unicode).getBytes());
                            outputStream.write(unicode);
                            outputStream.flush();
                            return true;
                        }
                        break;
                }
            } catch (IOException e) {
                Log.e(TAG, "Error handling key input", e);
            }
        }
        return super.onKeyDown(keyCode, event);
    }


    @Override
    public InputConnection onCreateInputConnection(EditorInfo outAttrs) {
        outAttrs.inputType = EditorInfo.TYPE_CLASS_TEXT;
        outAttrs.imeOptions = EditorInfo.IME_ACTION_NONE;
        return new BaseInputConnection(this, true) {
            @Override
            public boolean commitText(CharSequence text, int newCursorPosition) {
                if (outputStream != null) {
                    try {
                        // Echo text locally
                        write(text.toString().getBytes());
                        // Send to process
                        outputStream.write(text.toString().getBytes());
                        outputStream.flush();
                    } catch (IOException e) {
                        e.printStackTrace();
                    }
                }
                return true;
            }
        };
    }

    @Override
    protected void onMeasure(int widthMeasureSpec, int heightMeasureSpec) {
        int width = (int)(cols * charWidth);
        int height = (int)(rows * charHeight);
        setMeasuredDimension(width, height);
    }
}
EOL

# Create MainActivity.java
cat > app/src/main/java/com/example/terminalwizcastle/MainActivity.java << 'EOL'
package com.example.terminalwizcastle;

import android.app.Activity;
import android.os.Bundle;
import android.util.Log;
import android.widget.FrameLayout;
import java.io.File;
import java.io.FileOutputStream;
import java.io.InputStream;
import java.io.OutputStream;
import java.io.BufferedReader;
import java.io.InputStreamReader;

public class MainActivity extends Activity {
    private static final String TAG = "WizardsCastle";
    private TerminalView terminalView;
    private Process process;
    private Thread outputThread;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        
        FrameLayout layout = new FrameLayout(this);
        terminalView = new TerminalView(this);
        layout.addView(terminalView);
        setContentView(layout);
        
        // Test write to terminal
        terminalView.write("Initializing...\n".getBytes());
        
        startProcess();
    }
    
private void startProcess() {
        try {
            String abi = getabi();
            Log.d(TAG, "Using ABI: " + abi);
            
            File binFile = new File(getFilesDir(), "wizcastle");
            Log.d(TAG, "Binary path: " + binFile.getAbsolutePath());
            
            // Copy binary
            copyAsset(abi + "/wizcastle", binFile);
            binFile.setExecutable(true);
            Log.d(TAG, "Binary copied and made executable, size: " + binFile.length());
            
            // Start process
            ProcessBuilder pb = new ProcessBuilder(binFile.getAbsolutePath());
            pb.directory(getFilesDir());
            pb.environment().put("TERM", "dumb");
            pb.environment().put("HOME", getFilesDir().getAbsolutePath());
            pb.environment().put("TMPDIR", getCacheDir().getAbsolutePath());
            pb.redirectErrorStream(true);
            
            Log.d(TAG, "Starting process...");
            process = pb.start();
            terminalView.write("Process started\n".getBytes());
            
            // Get streams
            final OutputStream processInput = process.getOutputStream();
            final InputStream processOutput = process.getInputStream();
            
            // Set up process input
            terminalView.setOutputStream(processInput);
            
            // Read process output
            outputThread = new Thread(() -> {
                byte[] buffer = new byte[4096];
                try {
                    while (true) {
                        int available = processOutput.available();
                        if (available > 0) {
                            int read = processOutput.read(buffer, 0, Math.min(available, buffer.length));
                            if (read > 0) {
                                final byte[] data = new byte[read];
                                System.arraycopy(buffer, 0, data, 0, read);
                                runOnUiThread(() -> terminalView.write(data));
                            }
                        }
                        
                        // Check if process is still running
                        try {
                            int exitCode = process.exitValue();
                            // If we get here, process has ended
                            runOnUiThread(() -> terminalView.write(
                                ("\nProcess exited with code " + exitCode + "\n").getBytes()));
                            break;
                        } catch (IllegalThreadStateException e) {
                            // Process is still running
                        }
                        
                        Thread.sleep(10); // Small delay to prevent busy waiting
                    }
                } catch (Exception e) {
                    final String error = "Error reading output: " + e.getMessage() + "\n";
                    runOnUiThread(() -> terminalView.write(error.getBytes()));
                }
            });
            outputThread.start();
            
        } catch (Exception e) {
            Log.e(TAG, "Error in startProcess", e);
            String error = "Error: " + e.getMessage() + "\n";
            terminalView.write(error.getBytes());
        }
    }   
 
    private void copyAsset(String assetPath, File destFile) throws Exception {
        try (InputStream in = getAssets().open(assetPath);
             OutputStream out = new FileOutputStream(destFile)) {
            byte[] buffer = new byte[1024];
            int read;
            long total = 0;
            while ((read = in.read(buffer)) != -1) {
                out.write(buffer, 0, read);
                total += read;
            }
            Log.d(TAG, "Copied " + total + " bytes from " + assetPath);
        }
    }
    
    private String getabi() {
        String[] abis = android.os.Build.SUPPORTED_ABIS;
        StringBuilder abiList = new StringBuilder("Available ABIs: ");
        for (String abi : abis) {
            abiList.append(abi).append(" ");
        }
        Log.d(TAG, abiList.toString());
        
        for (String abi : abis) {
            if (abi.equals("x86_64")) return "x86_64";
            if (abi.equals("arm64-v8a")) return "arm64-v8a";
        }
        return "x86_64";
    }
    
    @Override
    protected void onDestroy() {
        super.onDestroy();
        if (process != null) {
            process.destroy();
        }
        if (outputThread != null) {
            outputThread.interrupt();
        }
    }
}
EOL

# Create AndroidManifest.xml
cat > app/src/main/AndroidManifest.xml << 'EOL'
<?xml version="1.0" encoding="utf-8"?>
<manifest xmlns:android="http://schemas.android.com/apk/res/android">
    <application android:label="Wizards Castle">
        <activity 
            android:name=".MainActivity" 
            android:exported="true"
            android:theme="@android:style/Theme.NoTitleBar.Fullscreen"
            android:windowSoftInputMode="stateAlwaysVisible|adjustResize">
            <intent-filter>
                <action android:name="android.intent.action.MAIN" />
                <category android:name="android.intent.category.LAUNCHER" />
            </intent-filter>
        </activity>
    </application>
</manifest>
EOL

# Create build.gradle
cat > app/build.gradle << 'EOL'
plugins {
    id 'com.android.application'
}

android {
    namespace 'com.example.terminalwizcastle'
    compileSdkVersion 33
    
    defaultConfig {
        applicationId "com.example.terminalwizcastle"
        minSdkVersion 21
        targetSdkVersion 33
        versionCode 1
        versionName "1.0"
    }
}
EOL

# Create settings.gradle
cat > settings.gradle << 'EOL'
pluginManagement {
    repositories {
        google()
        mavenCentral()
        gradlePluginPortal()
    }
}

dependencyResolutionManagement {
    repositoriesMode.set(RepositoriesMode.PREFER_SETTINGS)
    repositories {
        google()
        mavenCentral()
    }
}

rootProject.name = 'WizardsCastle'
include ':app'
EOL

# Create root build.gradle
cat > build.gradle << 'EOL'
buildscript {
    repositories {
        google()
        mavenCentral()
    }
    dependencies {
        classpath 'com.android.tools.build:gradle:8.1.0'
    }
}
EOL

# Create gradle.properties
cat > gradle.properties << 'EOL'
org.gradle.jvmargs=-Xmx2048m -Dfile.encoding=UTF-8
android.useAndroidX=true
android.nonTransactiveRClass=true
android.suppressUnsupportedCompileSdk=34
EOL

echo "Project created. To build:"
echo "1. Make sure ANDROID_NDK_HOME is set"
echo "2. ./gradlew assembleDebug"
echo "The APK will be in app/build/outputs/apk/debug/app-debug.apk"
