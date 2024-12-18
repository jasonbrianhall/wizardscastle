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
cp ../../c/*.c app/src/main/assets/
cp ../../c/*.h app/src/main/assets/
#cp ../*.c app/src/main/assets/

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
import android.view.inputmethod.InputMethodManager;
import java.io.IOException;
import java.io.OutputStream;

public class TerminalView extends View {
    private Paint textPaint;
    private float charWidth;
    private float charHeight;
    private int cols;
    private int rows;
    private char[][] buffer;
    private int cursorX = 0;
    private int cursorY = 0;
    private OutputStream outputStream;
    private boolean cursorVisible = true;
    private float scaleFactor = 1.0f;
    private float baseTextSize;
    private boolean keyboardVisible = false;
    private InputMethodManager imm;

    public TerminalView(Context context) {
        super(context);
        init();
    }
    
    public TerminalView(Context context, AttributeSet attrs) {
        super(context, attrs);
        init();
    }
    
    private void init() {
        imm = (InputMethodManager) getContext().getSystemService(Context.INPUT_METHOD_SERVICE);
        textPaint = new Paint();
        textPaint.setColor(Color.GREEN);
        textPaint.setTypeface(Typeface.MONOSPACE);
        textPaint.setAntiAlias(true);
        
        // Initialize with dummy values, will be updated in onSizeChanged
        cols = 80;
        rows = 48;
        buffer = new char[rows][cols];
        
        calculateInitialTextSize();
        clearScreen();
        
        setFocusable(true);
        setFocusableInTouchMode(true);
        
        // Show keyboard on start
        post(() -> {
            requestFocus();
            showKeyboard();
        });
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
        cursorX = 0;
        cursorY = 0;
        invalidate();
    }

    @Override
    protected void onSizeChanged(int w, int h, int oldw, int oldh) {
        super.onSizeChanged(w, h, oldw, oldh);
        
        // Recalculate dimensions based on new size
        calculateDimensions(w, h);
        
        // Create new buffer with new dimensions
        char[][] newBuffer = new char[rows][cols];
        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < cols; j++) {
                newBuffer[i][j] = ' ';
            }
        }
        
        // Copy old buffer contents if they exist
        if (buffer != null) {
            int minRows = Math.min(rows, buffer.length);
            int minCols = Math.min(cols, buffer[0].length);
            for (int i = 0; i < minRows; i++) {
                System.arraycopy(buffer[i], 0, newBuffer[i], 0, minCols);
            }
        }
        
        buffer = newBuffer;
        invalidate();
    }
    
    private void calculateDimensions(int width, int height) {
        // Calculate text size that would fill the screen
        float testTextSize = 100f;  // Start with a large size
        textPaint.setTextSize(testTextSize);
        
        // Measure character dimensions at test size
        Paint.FontMetrics fm = textPaint.getFontMetrics();
        float testCharHeight = fm.bottom - fm.top;
        float testCharWidth = textPaint.measureText("M");
        
        // Calculate scaling factors
        float heightScale = height / testCharHeight;
        float widthScale = width / testCharWidth;
        
        // Calculate final text size
        baseTextSize = Math.min(testTextSize * heightScale / 48,  // Aim for at least 24 rows
                               testTextSize * widthScale / 80);    // Aim for at least 80 columns
        
        updateTextSize();
        
        // Calculate final dimensions
        fm = textPaint.getFontMetrics();
        charHeight = fm.bottom - fm.top;
        charWidth = textPaint.measureText("M");
        
        // Calculate rows and columns that will fit
        rows = Math.max(48, (int)(height / charHeight));
        cols = Math.max(80, (int)(width / charWidth));
    }
    
    private void calculateInitialTextSize() {
        android.util.DisplayMetrics metrics = getResources().getDisplayMetrics();
        baseTextSize = metrics.density * 12f;  // Start with 12dp
        updateTextSize();
    }

    private void updateTextSize() {
        float newTextSize = baseTextSize * scaleFactor;
        textPaint.setTextSize(newTextSize);
        
        Paint.FontMetrics fm = textPaint.getFontMetrics();
        charHeight = fm.bottom - fm.top;
        charWidth = textPaint.measureText("M");
        
        invalidate();
    }

    public void showKeyboard() {
        if (!keyboardVisible) {
            imm.showSoftInput(this, InputMethodManager.SHOW_IMPLICIT);
            keyboardVisible = true;
        }
    }

    public void hideKeyboard() {
        if (keyboardVisible) {
            imm.hideSoftInputFromWindow(getWindowToken(), 0);
            keyboardVisible = false;
        }
    }

    public void toggleKeyboard() {
        if (keyboardVisible) {
            hideKeyboard();
        } else {
            showKeyboard();
        }
    }

    @Override
    public boolean onKeyDown(int keyCode, KeyEvent event) {
        if (outputStream != null) {
            try {
                if (keyCode == KeyEvent.KEYCODE_ENTER) {
                    outputStream.write('\n');
                    outputStream.flush();
                    return true;
                } else if (keyCode == KeyEvent.KEYCODE_DEL) {
                    outputStream.write('\b');
                    outputStream.flush();
                    return true;
                } else if (keyCode == KeyEvent.KEYCODE_DPAD_UP) {
                    // Send escape sequence for up arrow
                    outputStream.write(new byte[] {27, '[', 'A'});
                    outputStream.flush();
                    return true;
                } else if (keyCode == KeyEvent.KEYCODE_DPAD_DOWN) {
                    // Send escape sequence for down arrow
                    outputStream.write(new byte[] {27, '[', 'B'});
                    outputStream.flush();
                    return true;
                } else {
                    int unicode = event.getUnicodeChar();
                    if (unicode != 0) {
                        outputStream.write(unicode);
                        outputStream.flush();
                        return true;
                    }
                }
            } catch (IOException e) {
                e.printStackTrace();
            }
        }
        return super.onKeyDown(keyCode, event);
    }


    public void write(byte[] data) {
        for (byte b : data) {
            if (b == '\n') {
                // Always move cursor down and to start of line on \n
                cursorY++;
                cursorX = 0;
                
                // Handle scrolling if needed
                if (cursorY >= rows) {
                    scrollUp();
                    cursorY = rows - 1;
                }
            } else if (b == '\r') {
                // Just move cursor to start of line
                cursorX = 0;
            } else if (b == '\b') {
                if (cursorX > 0) {
                    cursorX--;
                    buffer[cursorY][cursorX] = ' ';
                }
            } else {
                if (cursorX >= cols) {
                    cursorX = 0;
                    cursorY++;
                    if (cursorY >= rows) {
                        scrollUp();
                        cursorY = rows - 1;
                    }
                }
                buffer[cursorY][cursorX++] = (char)b;
            }
        }
        invalidate();
    }

    private void scrollUp() {
        // Move all lines up one position
        for (int i = 0; i < rows - 1; i++) {
            System.arraycopy(buffer[i + 1], 0, buffer[i], 0, cols);
        }
        
        // Clear the last line
        for (int j = 0; j < cols; j++) {
            buffer[rows - 1][j] = ' ';
        }
    }

    @Override
    protected void onDraw(Canvas canvas) {
        super.onDraw(canvas);
        
        canvas.drawColor(Color.BLACK);
        
        // Calculate baseline offset for proper text alignment
        Paint.FontMetrics fm = textPaint.getFontMetrics();
        float baselineOffset = -fm.top;
        
        for (int i = 0; i < rows; i++) {
            float y = i * charHeight + baselineOffset;
            for (int j = 0; j < cols; j++) {
                float x = j * charWidth;
                canvas.drawText(String.valueOf(buffer[i][j]), x, y, textPaint);
            }
        }
        
        // Draw cursor
        if (cursorVisible) {
            Paint cursorPaint = new Paint();
            cursorPaint.setColor(Color.GREEN);
            cursorPaint.setAlpha(160);  // Make cursor semi-transparent
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
    public InputConnection onCreateInputConnection(EditorInfo outAttrs) {
        outAttrs.inputType = EditorInfo.TYPE_CLASS_TEXT | EditorInfo.TYPE_TEXT_FLAG_NO_SUGGESTIONS;
        outAttrs.imeOptions = EditorInfo.IME_FLAG_NO_EXTRACT_UI | EditorInfo.IME_FLAG_NO_FULLSCREEN;
        return new BaseInputConnection(this, true) {
            @Override
            public boolean commitText(CharSequence text, int newCursorPosition) {
                if (outputStream != null) {
                    try {
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
