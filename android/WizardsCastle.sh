#!/bin/bash
set -e

# Check for NDK
if [ -z "$ANDROID_NDK_HOME" ]; then
	echo "Error: ANDROID_NDK_HOME not set (download the Android NDK, extract it and set this variable)"
    exit 1
fi

# Check for NDK
if [ -z "$ANDROID_HOME" ]; then
    echo "Error: ANDROID_HOME not set (e.g. /home/user/Android/Sdk is using Android Studio)"
    exit 1
fi


# Check for ImageMagick
if ! command -v convert &> /dev/null; then
    echo "Error: ImageMagick not found (needed for icon conversion)"
    exit 1
fi

# Check for ImageMagick
if ! command -v gradle &> /dev/null; then
    echo -en "Warning: Command gradle not found (used to compile the embedded Java)\n\n"
fi


# Create project structure
mkdir -p WizardsCastle/app/src/main/{assets,java/org/wizardscastle/terminalwizcastle,res}
cd WizardsCastle

# Create resource directories
mkdir -p app/src/main/res/mipmap-{mdpi,hdpi,xhdpi,xxhdpi,xxxhdpi}
mkdir -p app/src/main/res/mipmap-anydpi-v26
mkdir -p app/src/main/res/drawable

# Set up ImageMagick quality settings
CONVERT_OPTS="-strip -quality 100 -define png:compression-level=9"

# Convert main icon for each density with high quality settings
convert $CONVERT_OPTS ../icon.png -resize 192x192 app/src/main/res/mipmap-mdpi/ic_launcher.png
convert $CONVERT_OPTS ../icon.png -resize 288x288 app/src/main/res/mipmap-hdpi/ic_launcher.png
convert $CONVERT_OPTS ../icon.png -resize 384x384 app/src/main/res/mipmap-xhdpi/ic_launcher.png
convert $CONVERT_OPTS ../icon.png -resize 576x576 app/src/main/res/mipmap-xxhdpi/ic_launcher.png
convert $CONVERT_OPTS ../icon.png -resize 768x768 app/src/main/res/mipmap-xxxhdpi/ic_launcher.png

# Create foreground layer icons (75% of main icon size for padding)
convert $CONVERT_OPTS ../icon.png -resize 144x144 app/src/main/res/mipmap-mdpi/ic_launcher_foreground.png
convert $CONVERT_OPTS ../icon.png -resize 216x216 app/src/main/res/mipmap-hdpi/ic_launcher_foreground.png
convert $CONVERT_OPTS ../icon.png -resize 288x288 app/src/main/res/mipmap-xhdpi/ic_launcher_foreground.png
convert $CONVERT_OPTS ../icon.png -resize 432x432 app/src/main/res/mipmap-xxhdpi/ic_launcher_foreground.png
convert $CONVERT_OPTS ../icon.png -resize 576x576 app/src/main/res/mipmap-xxxhdpi/ic_launcher_foreground.png

# Create round icons (same dimensions as regular icons)
convert $CONVERT_OPTS ../icon.png -resize 192x192 app/src/main/res/mipmap-mdpi/ic_launcher_round.png
convert $CONVERT_OPTS ../icon.png -resize 288x288 app/src/main/res/mipmap-hdpi/ic_launcher_round.png
convert $CONVERT_OPTS ../icon.png -resize 384x384 app/src/main/res/mipmap-xhdpi/ic_launcher_round.png
convert $CONVERT_OPTS ../icon.png -resize 576x576 app/src/main/res/mipmap-xxhdpi/ic_launcher_round.png
convert $CONVERT_OPTS ../icon.png -resize 768x768 app/src/main/res/mipmap-xxxhdpi/ic_launcher_round.png

# Create splash icon drawable from the main icon
convert $CONVERT_OPTS ../icon_splash.png -resize 512x512 app/src/main/res/drawable/splash_icon.png



# Create background resource
cat > app/src/main/res/drawable/ic_launcher_background.xml << 'EOL'
<?xml version="1.0" encoding="utf-8"?>
<shape xmlns:android="http://schemas.android.com/apk/res/android">
    <solid android:color="#FFFFFF"/>
</shape>
EOL

# Create adaptive icon config
cat > app/src/main/res/mipmap-anydpi-v26/ic_launcher.xml << 'EOL'
<?xml version="1.0" encoding="utf-8"?>
<adaptive-icon xmlns:android="http://schemas.android.com/apk/res/android">
    <background android:drawable="@drawable/ic_launcher_background"/>
    <foreground android:drawable="@mipmap/ic_launcher_foreground"/>
</adaptive-icon>
EOL

# Create round adaptive icon config
cat > app/src/main/res/mipmap-anydpi-v26/ic_launcher_round.xml << 'EOL'
<?xml version="1.0" encoding="utf-8"?>
<adaptive-icon xmlns:android="http://schemas.android.com/apk/res/android">
    <background android:drawable="@drawable/ic_launcher_background"/>
    <foreground android:drawable="@mipmap/ic_launcher_foreground"/>
</adaptive-icon>
EOL

# Copy and compile the C program for each architecture
cp ../../c/*.c app/src/main/assets/
cp ../../c/*.h app/src/main/assets/
#cp ../*.c app/src/main/assets/

for arch in arm64-v8a x86_64; do
    mkdir -p app/src/main/jniLibs/$arch
    case $arch in
        "arm64-v8a") 
            target="aarch64-linux-android21"
            ;;
        "x86_64") 
            target="x86_64-linux-android21"
            ;;
    esac
    
    $ANDROID_NDK_HOME/toolchains/llvm/prebuilt/linux-x86_64/bin/clang \
        --target=$target \
        -fPIE \
        -fPIC \
        -static \
        -Wl,--icf=all \
        -Wl,-z,max-page-size=4096 \
        -Wl,-z,relro \
        -Wl,--build-id \
        -Wl,--no-undefined \
        -Wl,-z,noexecstack \
        -Wl,--gc-sections \
        -o app/src/main/jniLibs/$arch/libwizcastle.so \
        app/src/main/assets/*.c
done

rm app/src/main/assets/*.c
rm app/src/main/assets/*.h

# Create SplashActivity.java
cat > app/src/main/java/org/wizardscastle/terminalwizcastle/SplashActivity.java << 'EOL'
package org.wizardscastle.terminalwizcastle;

import android.app.Activity;
import android.content.Intent;
import android.os.Bundle;
import android.os.Handler;
import android.view.Window;
import android.view.WindowManager;

public class SplashActivity extends Activity {
    private static final int SPLASH_DURATION = 2000; // 2 seconds

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        
        // Remove title bar
        requestWindowFeature(Window.FEATURE_NO_TITLE);
        getWindow().setFlags(WindowManager.LayoutParams.FLAG_FULLSCREEN,
                           WindowManager.LayoutParams.FLAG_FULLSCREEN);
        
        setContentView(R.layout.activity_splash);

        new Handler().postDelayed(() -> {
            Intent intent = new Intent(SplashActivity.this, MainActivity.class);
            startActivity(intent);
            finish();
        }, SPLASH_DURATION);
    }
}
EOL

# Create splash screen layout
mkdir -p app/src/main/res/layout
cat > app/src/main/res/layout/activity_splash.xml << 'EOL'
<?xml version="1.0" encoding="utf-8"?>
<RelativeLayout xmlns:android="http://schemas.android.com/apk/res/android"
    android:layout_width="match_parent"
    android:layout_height="match_parent"
    android:background="@android:color/black">

    <ImageView
        android:layout_width="wrap_content"
        android:layout_height="wrap_content"
        android:layout_centerInParent="true"
        android:src="@drawable/splash_icon"
        android:contentDescription="@string/app_name"/>
</RelativeLayout>
EOL

# Create TerminalView.java
cat > app/src/main/java/org/wizardscastle/terminalwizcastle/TerminalView.java << 'EOL'
package org.wizardscastle.terminalwizcastle;
import android.content.Context;
import android.graphics.Canvas;
import android.graphics.Color;
import android.graphics.Paint;
import android.graphics.Typeface;
import android.util.AttributeSet;
import android.view.GestureDetector;
import android.view.KeyEvent;
import android.view.MotionEvent;
import android.view.ScaleGestureDetector;
import android.view.View;
import android.view.inputmethod.BaseInputConnection;
import android.view.inputmethod.EditorInfo;
import android.view.inputmethod.InputConnection;
import android.view.inputmethod.InputMethodManager;
import android.widget.Scroller;
import java.io.IOException;
import java.io.OutputStream;
import android.util.Log;

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
    private ScaleGestureDetector scaleDetector;
    private static final float MIN_SCALE = 1.0f;
    private static final float MAX_SCALE = 4.0f;
    private GestureDetector gestureDetector;
    private Scroller scroller;
    private float scrollX = 0;
    private float scrollY = 0;
    private float maxScrollX = 0;
    private float maxScrollY = 0;

    // First, define the ColorScheme inner class
    private static class ColorScheme {
        final int textColor;
        final int backgroundColor;

        ColorScheme(int textColor, int backgroundColor) {
            this.textColor = textColor;
            this.backgroundColor = backgroundColor;
        }
    }

    private static final ColorScheme[] COLOR_SCHEMES = {
        new ColorScheme(Color.GREEN, Color.BLACK),      // Classic green on black
        new ColorScheme(Color.WHITE, Color.BLACK),      // White on black
        new ColorScheme(Color.BLACK, Color.WHITE),      // Black on white
        new ColorScheme(Color.CYAN, Color.BLACK),       // Cyan on black
        new ColorScheme(Color.rgb(255, 165, 0), Color.BLACK), // Orange on black
        new ColorScheme(Color.rgb(170, 170, 170), Color.rgb(0, 0, 85)), // Grey on navy
    };
    private int currentColorScheme = 0;

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
        
        cols = 80;
        rows = 48;
        buffer = new char[rows][cols];
        
        calculateInitialTextSize();
        clearScreen();
        
        setFocusable(true);
        setFocusableInTouchMode(true);

        scroller = new Scroller(getContext());
        gestureDetector = new GestureDetector(getContext(), new GestureDetector.SimpleOnGestureListener() {
            @Override
            public boolean onScroll(MotionEvent e1, MotionEvent e2, float distanceX, float distanceY) {
                if (!scaleDetector.isInProgress()) {
                    scrollBy(distanceX, distanceY);
                    return true;
                }
                return false;
            }

            @Override
            public boolean onFling(MotionEvent e1, MotionEvent e2, float velocityX, float velocityY) {
                if (!scaleDetector.isInProgress()) {
                    scroller.fling(
                        (int)scrollX, (int)scrollY,
                        -(int)velocityX, -(int)velocityY,
                        0, (int)maxScrollX,
                        0, (int)maxScrollY
                    );
                    postInvalidateOnAnimation();
                    return true;
                }
                return false;
            }

            @Override
            public boolean onDoubleTap(MotionEvent e) {
                cycleColorScheme();
                return true;
            }
        });
        
        scaleDetector = new ScaleGestureDetector(getContext(), new ScaleGestureDetector.SimpleOnScaleGestureListener() {
            @Override
            public boolean onScale(ScaleGestureDetector detector) {
                float oldScale = scaleFactor;
                scaleFactor *= detector.getScaleFactor();
                scaleFactor = Math.max(MIN_SCALE, Math.min(scaleFactor, MAX_SCALE));
                
                if (oldScale != scaleFactor) {
                    updateTextSize();
                }
                return true;
            }
        });
        
        post(() -> {
            requestFocus();
            showKeyboard();
        });
    }

    private void cycleColorScheme() {
        currentColorScheme = (currentColorScheme + 1) % COLOR_SCHEMES.length;
        ColorScheme scheme = COLOR_SCHEMES[currentColorScheme];
        textPaint.setColor(scheme.textColor);
        invalidate();
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

    private void scrollBy(float distanceX, float distanceY) {
        scrollX = Math.max(0, Math.min(scrollX + distanceX, maxScrollX));
        scrollY = Math.max(0, Math.min(scrollY + distanceY, maxScrollY));
        updateScrollLimits();
        invalidate();
    }

    private void updateScrollLimits() {
        maxScrollX = Math.max(0, cols * charWidth - getWidth());
        maxScrollY = Math.max(0, rows * charHeight - getHeight());
        scrollX = Math.max(0, Math.min(scrollX, maxScrollX));
        scrollY = Math.max(0, Math.min(scrollY, maxScrollY));
    }

    public void write(byte[] data) {
        for (byte b : data) {
            if (b == '\n') {
                cursorY++;
                cursorX = 0;
                
                if (cursorY >= rows) {
                    scrollUp();
                    cursorY = rows - 1;
                }
            } else if (b == '\r') {
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
        for (int i = 0; i < rows - 1; i++) {
            System.arraycopy(buffer[i + 1], 0, buffer[i], 0, cols);
        }
        
        for (int j = 0; j < cols; j++) {
            buffer[rows - 1][j] = ' ';
        }
    }

    @Override
    public void computeScroll() {
        if (scroller.computeScrollOffset()) {
            scrollX = scroller.getCurrX();
            scrollY = scroller.getCurrY();
            updateScrollLimits();
            postInvalidateOnAnimation();
        }
    }

    @Override
    public boolean onTouchEvent(MotionEvent event) {
        boolean handled = scaleDetector.onTouchEvent(event);
        handled |= gestureDetector.onTouchEvent(event);
        
        if (event.getAction() == MotionEvent.ACTION_UP) {
            if (!scroller.isFinished()) {
                scroller.abortAnimation();
            }
        }
        
        return handled || super.onTouchEvent(event);
    }
    
    @Override
    public boolean onGenericMotionEvent(MotionEvent event) {
        if ((event.getSource() & 8194) != 0) {
            switch (event.getAction()) {
                case MotionEvent.ACTION_SCROLL:
                    float scrollDelta = event.getAxisValue(MotionEvent.AXIS_VSCROLL);
                    if (scrollDelta != 0) {
                        if ((event.getMetaState() & KeyEvent.META_CTRL_ON) != 0) {
                            float oldScale = scaleFactor;
                            scaleFactor *= (1.0f + (scrollDelta * 0.1f));
                            scaleFactor = Math.max(MIN_SCALE, Math.min(scaleFactor, MAX_SCALE));
                            if (oldScale != scaleFactor) {
                                updateTextSize();
                            }
                        } else {
                            scrollBy(0, -scrollDelta * 50);
                        }
                        return true;
                    }
                    break;
            }
        }
        return super.onGenericMotionEvent(event);
    }

    private void calculateInitialTextSize() {
        android.util.DisplayMetrics metrics = getResources().getDisplayMetrics();
        baseTextSize = metrics.density * 12f;
        updateTextSize();
    }

    private void updateTextSize() {
        float newTextSize = baseTextSize * scaleFactor;
        textPaint.setTextSize(newTextSize);
        
        Paint.FontMetrics fm = textPaint.getFontMetrics();
        charHeight = fm.bottom - fm.top;
        charWidth = textPaint.measureText("M");
        
        calculateDimensionsPreserveBuffer(getWidth(), getHeight());
        updateScrollLimits();
        
        invalidate();
    }
    
    private void calculateDimensionsPreserveBuffer(int width, int height) {
        Paint.FontMetrics fm = textPaint.getFontMetrics();
        charHeight = fm.bottom - fm.top;
        charWidth = textPaint.measureText("M");
        
        rows = Math.max(48, (int)(height / charHeight));
        cols = Math.max(80, (int)(width / charWidth));
    }

    @Override
    protected void onSizeChanged(int w, int h, int oldw, int oldh) {
        super.onSizeChanged(w, h, oldw, oldh);
        
        calculateDimensions(w, h);
        
        char[][] newBuffer = new char[rows][cols];
        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < cols; j++) {
                newBuffer[i][j] = ' ';
            }
        }
        
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
        float testTextSize = 100f;
        textPaint.setTextSize(testTextSize);
        
        Paint.FontMetrics fm = textPaint.getFontMetrics();
        float testCharHeight = fm.bottom - fm.top;
        float testCharWidth = textPaint.measureText("M");
        
        float heightScale = height / testCharHeight;
        float widthScale = width / testCharWidth;
        
        baseTextSize = Math.min(testTextSize * heightScale / 48,
                               testTextSize * widthScale / 80);
        
        updateTextSize();
        
        fm = textPaint.getFontMetrics();
        charHeight = fm.bottom - fm.top;
        charWidth = textPaint.measureText("M");
        
        rows = Math.max(48, (int)(height / charHeight));
        cols = Math.max(80, (int)(width / charWidth));
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
    protected void onDraw(Canvas canvas) {
        super.onDraw(canvas);
        
        //canvas.drawColor(Color.BLACK);
        canvas.drawColor(COLOR_SCHEMES[currentColorScheme].backgroundColor);

        canvas.save();
        canvas.translate(-scrollX, -scrollY);
        
        Paint.FontMetrics fm = textPaint.getFontMetrics();
        float baselineOffset = -fm.top;
        
        int startRow = Math.max(0, (int)(scrollY / charHeight));
        int endRow = Math.min(rows, (int)((scrollY + getHeight()) / charHeight) + 1);
        int startCol = Math.max(0, (int)(scrollX / charWidth));
        int endCol = Math.min(cols, (int)((scrollX + getWidth()) / charWidth) + 1);
        
        for (int i = startRow; i < endRow; i++) {
            float y = i * charHeight + baselineOffset;
            for (int j = startCol; j < endCol; j++) {
                float x = j * charWidth;
                canvas.drawText(String.valueOf(buffer[i][j]), x, y, textPaint);
            }
        }
        
        if (cursorVisible &&
            cursorX >= startCol && cursorX < endCol &&
            cursorY >= startRow && cursorY < endRow) {
            Paint cursorPaint = new Paint();
            cursorPaint.setColor(Color.GREEN);
            cursorPaint.setAlpha(160);
            canvas.drawRect(
                cursorX * charWidth,
                cursorY * charHeight,
                (cursorX + 1) * charWidth,
                (cursorY + 1) * charHeight,
                cursorPaint
            );
        }
        
        canvas.restore();
    }

// Add at class level
private StringBuilder inputBuffer = new StringBuilder();

@Override
public InputConnection onCreateInputConnection(EditorInfo outAttrs) {
    outAttrs.inputType = EditorInfo.TYPE_CLASS_TEXT | EditorInfo.TYPE_TEXT_FLAG_NO_SUGGESTIONS;
    outAttrs.imeOptions = EditorInfo.IME_FLAG_NO_EXTRACT_UI | EditorInfo.IME_FLAG_NO_FULLSCREEN;
    return new BaseInputConnection(this, true) {
        @Override
        public boolean commitText(CharSequence text, int newCursorPosition) {
            if (outputStream != null) {
                try {
                    String str = text.toString();
                    inputBuffer.append(str);
                    write(str.getBytes());  // Show the character
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
    public boolean onKeyDown(int keyCode, KeyEvent event) {
        if (outputStream != null) {
            try {
                switch (keyCode) {
                    case KeyEvent.KEYCODE_ENTER:
                        write("\n".getBytes());  // Show newline
                        outputStream.write((inputBuffer.toString() + "\n").getBytes());
                        inputBuffer.setLength(0);  // Clear buffer
                        outputStream.flush();
                        return true;

                    case KeyEvent.KEYCODE_DEL:
                        if (inputBuffer.length() > 0) {
                            inputBuffer.setLength(inputBuffer.length() - 1);
                            write("\b".getBytes());
                        }
                        outputStream.flush();
                        return true;

                    case KeyEvent.KEYCODE_DPAD_UP:
                        // Send escape sequence for up arrow
                        outputStream.write("\033[A".getBytes());
                        outputStream.flush();
                        return true;

                    case KeyEvent.KEYCODE_DPAD_DOWN:
                        // Send escape sequence for down arrow
                        outputStream.write("\033[B".getBytes());
                        outputStream.flush();
                        return true;

                    default:
                        int unicode = event.getUnicodeChar();
                        if (unicode != 0) {
                            inputBuffer.append((char)unicode);
                            write(new byte[]{(byte)unicode});
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

}
EOL

# Create MainActivity.java
cat > app/src/main/java/org/wizardscastle/terminalwizcastle/MainActivity.java << 'EOL'
package org.wizardscastle.terminalwizcastle;

import android.app.Activity;
import android.os.Bundle;
import android.util.Log;
import android.view.Gravity;
import android.widget.FrameLayout;
import android.widget.ImageButton;
import android.view.View;
import java.io.File;
import java.io.InputStream;
import java.io.OutputStream;

public class MainActivity extends Activity {
    private static final String TAG = "WizardsCastle";
    private TerminalView terminalView;
    private Process process;
    private Thread outputThread;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        
        // Create main layout
        FrameLayout layout = new FrameLayout(this);
        
        // Create terminal view
        terminalView = new TerminalView(this);
        layout.addView(terminalView);
        
        // Create keyboard toggle button
        ImageButton keyboardButton = new ImageButton(this);
        keyboardButton.setImageResource(R.drawable.ic_keyboard);
        keyboardButton.setBackgroundColor(0x80000000);  // Semi-transparent black
        keyboardButton.setPadding(20, 20, 20, 20);
        
        // Set button layout parameters
        FrameLayout.LayoutParams params = new FrameLayout.LayoutParams(
            FrameLayout.LayoutParams.WRAP_CONTENT,
            FrameLayout.LayoutParams.WRAP_CONTENT,
            Gravity.BOTTOM | Gravity.END
        );
        params.setMargins(20, 20, 20, 20);
        keyboardButton.setLayoutParams(params);
        
        // Add button click listener
        keyboardButton.setOnClickListener(v -> terminalView.toggleKeyboard());
        
        // Add button to layout
        layout.addView(keyboardButton);
        
        setContentView(layout);
        
        // Initialize terminal
        terminalView.post(() -> {
            terminalView.requestFocus();
            terminalView.showKeyboard();
        });
        
        startProcess();
    }
    
    private void startProcess() {
        try {
            String abi = getabi();
            Log.d(TAG, "Using ABI: " + abi);
            
            File nativeLibDir = new File(getApplicationInfo().nativeLibraryDir);
            File binFile = new File(nativeLibDir, "libwizcastle.so");
            Log.d(TAG, "Binary path: " + binFile.getAbsolutePath());
            
            ProcessBuilder pb = new ProcessBuilder(binFile.getAbsolutePath());
            pb.directory(getFilesDir());
            pb.environment().put("TERM", "dumb");
            pb.environment().put("HOME", getFilesDir().getAbsolutePath());
            pb.environment().put("TMPDIR", getCacheDir().getAbsolutePath());
            pb.redirectErrorStream(true);
            
            Log.d(TAG, "Starting process...");
            process = pb.start();
            
            final OutputStream processInput = process.getOutputStream();
            final InputStream processOutput = process.getInputStream();
            
            terminalView.setOutputStream(processInput);
            
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
                        
                        try {
                            int exitCode = process.exitValue();
                            runOnUiThread(() -> terminalView.write(
                                ("\nProcess exited with code " + exitCode + "\n").getBytes()));
                            break;
                        } catch (IllegalThreadStateException e) {
                        }
                        
                        Thread.sleep(10);
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

cat > app/src/main/res/drawable/ic_keyboard.xml << 'EOL'
<?xml version="1.0" encoding="utf-8"?>
<vector xmlns:android="http://schemas.android.com/apk/res/android"
    android:width="24dp"
    android:height="24dp"
    android:viewportWidth="24"
    android:viewportHeight="24">
    <path
        android:fillColor="#FFFFFF"
        android:pathData="M20,5L4,5c-1.1,0 -1.99,0.9 -1.99,2L2,17c0,1.1 0.9,2 2,2h16c1.1,0 2,-0.9 2,-2L22,7c0,-1.1 -0.9,-2 -2,-2zM11,8h2v2h-2L11,8zM11,11h2v2h-2v-2zM8,8h2v2L8,10L8,8zM8,11h2v2L8,13v-2zM7,13L5,13v-2h2v2zM7,10L5,10L5,8h2v2zM16,17L8,17v-2h8v2zM16,13h-2v-2h2v2zM16,10h-2L14,8h2v2zM19,13h-2v-2h2v2zM19,10h-2L17,8h2v2z"/>
</vector>
EOL

# Create empty proguard-rules.pro
touch app/proguard-rules.pro

# Create strings.xml
mkdir -p app/src/main/res/values
cat > app/src/main/res/values/strings.xml << 'EOL'
<?xml version="1.0" encoding="utf-8"?>
<resources>
    <string name="app_name">Wizard\'s Castle</string>
    <string name="app_description">Wizards Castle is a classic text adventure game where you explore a mysterious castle filled with treasures and dangers. Navigate through rooms, battle monsters, and collect treasures as you try to survive this perilous dungeon crawl in order to collect the Orb of Zot.  Based on the 1980 BASIC game of the same name with creative differences.</string>
    <string name="developer">Jason Hall</string>
    <string name="developer_email">jasonbrianhall@gmail.com</string>
    <string name="website_url">https://github.com/jasonbrianhall/wizardscastle</string>
    <string name="source_license">MIT License</string>
    <string name="game_features">Randomly generated 512-room castles, Multiple character races (Human, Elf, Hobbit, Dwarf, Dark Elf), Strategic combat system, Magic spells, Vendor interactions, ASCII map display</string>
    <string name="original_concept">Joseph R. Power (1980)</string> 
    <string name="app_icon">App Icon</string>
</resources>
EOL

# Update AndroidManifest.xml
cat > app/src/main/AndroidManifest.xml << 'EOL'
<?xml version="1.0" encoding="utf-8"?>
<manifest xmlns:android="http://schemas.android.com/apk/res/android">
    <application 
        android:allowBackup="true"
        android:icon="@mipmap/ic_launcher"
        android:roundIcon="@mipmap/ic_launcher_round"
        android:label="@string/app_name">
        
        <!-- Meta-data using string resources -->
        <meta-data
            android:name="developer"
            android:value="@string/developer" />

	<meta-data
            android:name="developer_email"
            android:value="@string/developer_email" />
            
        <meta-data
            android:name="website_url"
            android:value="@string/website_url" />
            
        <meta-data
            android:name="game_description"
            android:value="@string/app_description" />

	<meta-data
            android:name="source_license"
            android:value="@string/source_license" />

        <meta-data
            android:name="game_features"
            android:value="@string/game_features" />

        <meta-data
            android:name="original_concept"
            android:value="@string/original_concept" />

        <activity
            android:name=".SplashActivity"
            android:theme="@android:style/Theme.NoTitleBar.Fullscreen"
            android:exported="true">
           <intent-filter>
               <action android:name="android.intent.action.MAIN" />
               <category android:name="android.intent.category.LAUNCHER" />
               </intent-filter>
        </activity>

        <activity 
            android:name=".MainActivity" 
            android:exported="true"
            android:theme="@android:style/Theme.NoTitleBar.Fullscreen"
            android:windowSoftInputMode="adjustResize">
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
    namespace 'org.wizardscastle.terminalwizcastle'
    compileSdkVersion 33
    
    defaultConfig {
        applicationId "org.wizardscastle.terminalwizcastle"
        minSdkVersion 21
        targetSdkVersion 33
        versionCode CHANGEME1
        versionName CHANGEME2
    }
    
    buildTypes {
        release {
            minifyEnabled true
            shrinkResources true
	    signingConfig signingConfigs.debug  // Use debug signing for testing
        }
    }   
 
    applicationVariants.all { variant ->
        variant.outputs.all {
            outputFileName = "wizards-castle.apk"
        }
    }
}
EOL

VERSION_CODE=$(date +%Y%m%d)
VERSION_NAME=$(date +%Y.%m.%d)

sed "s/CHANGEME1/$VERSION_CODE/g" app/build.gradle -i
sed "s/CHANGEME2/\"$VERSION_NAME\"/g" app/build.gradle -i

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
echo "1. Make sure ANDROID_NDK_HOME and ANDROID_HOME is set and correct"
echo "2. cd WizardsCastle"
echo "3. gradle assembleDebug (Debug) or gradle assemble"
echo "The APK will be in app/build/outputs/apk/debug/wizards-castle.apk"
echo "\n\nFor bundleRelease for the Google Playstore, run the command ./gradlew bundleRelease -PKEYSTORE_PASSWORD=xxx -PKEY_ALIAS=googleplay -PKEY_PASSWORD=xxx" and place the release-key.jks in that directory
